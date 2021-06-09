#include <Arduino.h>
#include "mpu6050.h"
#include "complementary_filter.h"
#include "Wire.h"
#include "matrix.h"
#include "Filter.h"

Mpu6050 mpu(Wire);


double fs = 1;
double dt = 1 / fs;

ComplementaryFilter cFilt(0.0, fs);

unsigned long t0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.begin();
    mpu.calibrateGyro(2000);

}

void loop() {
    mpu.update();

    cFilt.calculate(mpu.getAcc(), mpu.getGyro());
    Serial.print("roll/pitch: ");
    Serial.print(cFilt.getRoll());
    Serial.print(", ");
    Serial.println(cFilt.getPitch());

    double roll = cFilt.getRoll();
    double vRoll[9] = {1, 0, 0,
                      0, cos(roll), -sin(roll),
                      0, sin(roll), cos(roll)};
    Matrix::Matrix mRoll(3, 3, vRoll);

    double pitch = cFilt.getPitch();
    double vPitch[9] = {cos(pitch), 0, sin(pitch),
                       0, 1, 0,
                       -sin(pitch), 0, cos(pitch)};
    Matrix::Matrix mPitch(3, 3, vPitch);

    Matrix::Matrix acc(3, 1, mpu.getAcc());

    Matrix::Matrix rpResult;
    Matrix::multiply(mPitch, mRoll, rpResult);

    Matrix::Matrix result;
    Matrix::multiply(rpResult, acc, result);

    Serial.print("raw: ");
    Serial.print(mpu.getAcc()[0]);
    Serial.print(", ");
    Serial.print(mpu.getAcc()[1]);
    Serial.print(", ");
    Serial.print(mpu.getAcc()[2]);
    Serial.println();

    Serial.print("rotated: ");
    Serial.print(result.get(0, 0));
    Serial.print(", ");
    Serial.print(result.get(1, 0));
    Serial.print(", ");
    Serial.print(result.get(2, 0));
    Serial.println();
    Serial.println();

    while (millis() - t0 < (int) (dt * 1000));
    t0 = millis();
}
