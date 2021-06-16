#include <Arduino.h>
#include "mpu6050.h"
#include "Wire.h"
#include "TimeLib.h"
#include "com.h"
#include "messages.h"


Mpu6050 mpu(Wire);

char *sendBuffer;
char sendBufferSize = 0;

void digitalClockDisplay();

void sampleMpu();


double fs = 10;
double dt = 1 / fs;

unsigned long t0;


void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.begin();
//    mpu.calibrateGyro(2000);


}


void loop() {
    com::recive();
    com::handelmessage();

//    digitalClockDisplay();

    sampleMpu();
    com::sendMessage(sendBuffer, sendBufferSize, 0xdd);
    delete[] sendBuffer;

    while (millis() - t0 < (int) (dt * 1000));
    t0 = millis();
}

void printDigits(int digits) {
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if (digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

void digitalClockDisplay() {
    // digital clock display of the time
    Serial.print(TimeLib::hour());
    printDigits(TimeLib::minute());
    printDigits(TimeLib::second());
    Serial.print(":");
    Serial.print(TimeLib::milliseconds());
    Serial.print(" ");
    Serial.print(TimeLib::day());
    Serial.print(" ");
    Serial.print(TimeLib::month());
    Serial.print(" ");
    Serial.print(TimeLib::year());
    Serial.println();
}

void sampleMpu() {
    mpu.update();

    time_t now = TimeLib::now();

    Messages::Acceleration acc = {Messages::AccelerationId, Messages::AccStructSize, (float) mpu.getAcc()[0],
                                  (float) mpu.getAcc()[1], (float) mpu.getAcc()[2]};

    Messages::Gyro gyro = {Messages::GyroId, Messages::GyroStructSize, (float) mpu.getGyro()[0],
                           (float) mpu.getGyro()[1], (float) mpu.getGyro()[2]};

    char *accArray = new char[sizeof(acc)];
    memcpy(accArray, &acc, sizeof(acc));

    char *gyroArray = new char[sizeof(gyro)];
    memcpy(gyroArray, &gyro, sizeof(gyro));

    char *result;
    com::mergeArray(accArray, sizeof(acc), gyroArray, sizeof(gyro), result);
    delete[] accArray;
    delete[] gyroArray;

    Messages::Time time = {Messages::TimeId, Messages::TimeStructSize, (unsigned short) TimeLib::year(now),
                           (char) TimeLib::month(now), (char) TimeLib::day(now), (char) TimeLib::hour(now),
                           (char) TimeLib::minute(now), (char) TimeLib::second(now),
                           (unsigned short) TimeLib::milliseconds(now)};
    char *timeArray = new char[sizeof(time)];
    memcpy(timeArray, &time, sizeof(time));

    char *result2;
    com::mergeArray(timeArray, sizeof(time), result, sizeof(acc) + sizeof(gyro), result2);
    delete[] timeArray;
    delete[] result;

    sendBuffer = result2;
    sendBufferSize = sizeof(acc) + sizeof(gyro) + sizeof(time);

}

