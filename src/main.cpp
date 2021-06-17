#include <Arduino.h>
#include "mpu6050.h"
#include "Wire.h"
#include "TimeLib.h"
#include "messages.h"
#include "uartHost.h"


Mpu6050 mpu(Wire);

char operationMode = 1;
char *sendBuffer;
char sendBufferSize = 0;

void digitalClockDisplay();

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
//    mpu.calibrateGyro(2000);


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

    Messages::Acceleration acc = {Messages::AccelerationId, Messages::AccStructSize, (float) mpu.getAcc()[0],
                                  (float) mpu.getAcc()[1], (float) mpu.getAcc()[2]};

    Messages::Gyro gyro = {Messages::GyroId, Messages::GyroStructSize, (float) mpu.getGyro()[0],
                           (float) mpu.getGyro()[1], (float) mpu.getGyro()[2]};


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
    }
}


