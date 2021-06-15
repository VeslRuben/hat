#include <Arduino.h>
#include "mpu6050.h"
#include "complementary_filter.h"
#include "Wire.h"
#include "matrix.h"
#include "rotate.h"
#include "TimeLib.h"


Mpu6050 mpu(Wire);


double fs = 2;
double dt = 1 / fs;

ComplementaryFilter cFilt(0.0, fs);

unsigned long t0;

void setup() {
    Serial.begin(115200);
//    Wire.begin();
//    mpu.begin();
//    mpu.calibrateGyro(2000);
    setTime(15, 42, 0, 22, 15, 6, 2021);

}


void printDigits(int digits) {
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if (digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

void loop() {
//    mpu.update();
//
//    cFilt.calculate(mpu.getAcc(), mpu.getGyro());
//    Serial.print("roll/pitch: ");
//    Serial.print(cFilt.getRoll());
//    Serial.print(", ");
//    Serial.println(cFilt.getPitch());
//
//    Matrix::Matrix rotMat = rotate(cFilt.getRoll(), cFilt.getPitch());
////    rotate(cFilt.getRoll(), cFilt.getPitch());
//
//    Matrix::Matrix acc(3, 1, mpu.getAcc());
//
//    Matrix::Matrix result = Matrix::multiply(rotMat,acc);
////    Matrix::multiply(rotMat, acc, result);
//
//
//    Serial.print("raw: ");
//    Serial.print(mpu.getAcc()[0]);
//    Serial.print(", ");
//    Serial.print(mpu.getAcc()[1]);
//    Serial.print(", ");
//    Serial.print(mpu.getAcc()[2]);
//    Serial.println();
//
//    Serial.print("rotated: ");
//    Serial.print(result.get(0, 0));
//    Serial.print(", ");
//    Serial.print(result.get(1, 0));
//    Serial.print(", ");
//    Serial.print(result.get(2, 0));
//    Serial.println();
//    Serial.println();

    time_t t = now();
    Serial.print(hour(t));
    printDigits(minute(t));
    printDigits(second(t));
    Serial.print(" ");
    Serial.print(milliseconds(t));
    Serial.print(" ");
    Serial.print(day(t));
    Serial.print(" ");
    Serial.print(month(t));
    Serial.print(" ");
    Serial.print(year(t));
    Serial.println();

    while (millis() - t0 < (int) (dt * 1000));
    t0 = millis();
}
