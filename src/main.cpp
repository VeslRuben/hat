#include <Arduino.h>
#include "mpu6050.h"
#include "Wire.h"
#include "TimeLib.h"
#include "messages.h"
#include "uartHost.h"
#include "flash.h"

Mpu6050 mpu(Wire);

char operationMode = 0;  // 0 is not sending anything
char *sendBuffer;
char sendBufferSize = 0;

void sampleMpu();

void handelmessage(HostCom *host);


double fs = 10;
double dt = 1 / fs;

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

    if (operationMode == 1) {
        sampleMpu();
        hostCom.sendMessage(sendBuffer, sendBufferSize, 0xdd);
        delete[] sendBuffer;
    }

    while (millis() - t0 < (int) (dt * 1000));
    t0 = millis();
}


void sampleMpu() {
    mpu.update();

    time_t now = TimeLib::now();

    Messages::Acceleration acc = {Messages::AccelerationId, Messages::AccStructSize, mpu.getAcc()[0],
                                  mpu.getAcc()[1], mpu.getAcc()[2]};

    Messages::Gyro gyro = {Messages::GyroId, Messages::GyroStructSize, mpu.getGyro()[0],
                           mpu.getGyro()[1], mpu.getGyro()[2]};


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
        default:
            break;
    }
}