#include <Arduino.h>
#include "mpu6050.h"
#include "Wire.h"
#include "TimeLib.h"
#include "messages.h"
#include "uartHost.h"
#include "flash.h"
#include "matrix.h"
#include "complementary_filter.h"

Mpu6050 mpu(Wire);
Matrix::Matrix rotMatrix = Matrix::getRotationMatrix(0, 0, 0);

char operationMode = 0;  // 0 is not sending anything
char *sendBuffer;
char sendBufferSize = 0;

void sampleMpu();

void handelmessage(HostCom *host);


int fs = 10;
double dt = (double) 1 / fs;

unsigned long t0;


void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.begin();
    hostCom.setMessageHandler(handelmessage);

    flash.loadFromFlash();

    float *gyroOffset = new float[3];  // mpu class takes car of deleting the pointer
    gyroOffset[0] = flash.data.gyroOffsetX;
    gyroOffset[1] = flash.data.gyroOffsetY;
    gyroOffset[2] = flash.data.gyroOffsetZ;
    mpu.setGyroOffset(gyroOffset);

}


void loop() {
    hostCom.receve();

    if (millis() - t0 > (dt * 1000)) { // Sample and send imu data
        t0 = millis();
        if (operationMode == 1) {
            sampleMpu();
            hostCom.sendMessage(sendBuffer, sendBufferSize, 0xdd);
            delete[] sendBuffer;
        }

//        double acc[] = {10., 0., 0.};
//        Matrix::Matrix accMat(3, 2, acc);
//        Matrix::Matrix accRotated = Matrix::multiply(rotMatrix, accMat);
//        Serial.println(accRotated.get(0, 0));
//        Serial.println(accRotated.get(1, 0));
//        Serial.println(accRotated.get(2, 0));
//        Serial.println();
    }
}


void sampleMpu() {
    mpu.update();

    time_t now = TimeLib::now();

    double accValues[] = {(double) mpu.getAcc()[0], (double) mpu.getAcc()[1], (double) mpu.getAcc()[2]};
    Matrix::Matrix accMat(3, 1, accValues);
    Matrix::Matrix accRotated = Matrix::multiply(rotMatrix, accMat);

    Messages::Acceleration acc = {Messages::AccelerationId, Messages::AccStructSize, (float) accRotated.get(0, 0),
                                  (float) accRotated.get(1, 0), (float) accRotated.get(2, 0)};

    double gyroValues[] = {(double) mpu.getGyro()[0], (double) mpu.getGyro()[1], (double) mpu.getGyro()[2]};
    Matrix::Matrix gyroMat(3, 1, gyroValues);
    Matrix::Matrix gyroRotated = Matrix::multiply(rotMatrix, gyroMat);

    Messages::Gyro gyro = {Messages::GyroId, Messages::GyroStructSize, (float) gyroRotated.get(0, 0),
                           (float) gyroRotated.get(1, 0), (float) gyroRotated.get(2, 0)};


    char *result;
    hostCom.mergeArray((char *) &acc, sizeof(acc), (char *) &gyro, sizeof(gyro), result);


    Messages::Time time = {Messages::TimeId, Messages::TimeStructSize, (unsigned short) TimeLib::year(now),
                           (char) TimeLib::month(now), (char) TimeLib::day(now), (char) TimeLib::hour(now),
                           (char) TimeLib::minute(now), (char) TimeLib::second(now),
                           (unsigned short) TimeLib::milliseconds(now)};


    char *result2;
    hostCom.mergeArray((char *) &time, sizeof(time), result, sizeof(acc) + sizeof(gyro), result2);
    delete[] result;

    sendBuffer = result2;
    sendBufferSize = sizeof(acc) + sizeof(gyro) + sizeof(time);

}

void handelmessage(HostCom *host) {
    switch (host->id) {
        case Messages::SetTimeId: {
            Messages::SetTime time{};
            memcpy(&time, host->data, sizeof(time));
            TimeLib::setTime(time.hour, time.minute, time.sec, time.mSec, time.day, time.month, time.year);
            char ack[0]{};
            host->sendMessage(ack, 0, 0x02);
            break;
        }
        case Messages::SetOperationModeId:
            operationMode = host->data[0];
            if (operationMode > 1) operationMode = 1;        // only 0 and 1 are implemented yet
            host->sendMessage(host->data, host->len, host->id); // echo the message back as an ack
            break;
        case Messages::CalibrateGyroOffsetId: {
            mpu.calibrateGyro(2000);
            float *offset = mpu.getGyroOffset();
            flash.data.gyroOffsetX = offset[0];
            flash.data.gyroOffsetY = offset[1];
            flash.data.gyroOffsetZ = offset[2];
            flash.saveToFlash();
            host->sendMessage((char *) offset, host->len, host->id); // send back the offset values calculated
            break;
        }
        case Messages::CalibrateOrientationId: {
            Messages::CalibrateOrientation data{};
            memcpy(&data, host->data, host->len);
            switch (data.mode) {
                case 0: { // calibrate the currant orientation of the imu to be flat
                    ComplementaryFilter filter(0, fs);
                    double roll = 0;
                    double pitch = 0;
                    for (int i = 0; i < fs; ++i) {
                        mpu.update();
                        filter.calculate(mpu.getAcc(), mpu.getGyro());
                        roll += filter.getRoll();
                        pitch += filter.getPitch();
                    }
                    roll = roll / (double) fs;
                    pitch = pitch / (double) fs;
                    rotMatrix = Matrix::getRotationMatrix(roll, pitch, 0.);
                    data.roll = (float) roll;
                    data.pitch = (float) pitch;
                    memcpy(host->data, &data, host->len);
                    host->sendMessage(host->data,host->len, host->id);
                    break;
                }
                case 1:  //  add r, p, y to the current orientation

                    break;
                case 2:  //  set orientation to r, p, y

                    break;
            }
            break;
        }
        default:
            break;
    }
}