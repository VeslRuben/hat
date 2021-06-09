#include <Arduino.h>
#include "mpu6050.h"
#include "complementary_filter.h"
#include "Wire.h"
#include "matrix.h"
#include "rotate.h"


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

    Matrix::Matrix rotMat = rotate(cFilt.getRoll(), cFilt.getPitch());
//    rotate(cFilt.getRoll(), cFilt.getPitch());

    Matrix::Matrix acc(3, 1, mpu.getAcc());

    Matrix::Matrix result = Matrix::multiply(rotMat,acc);
//    Matrix::multiply(rotMat, acc, result);


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
