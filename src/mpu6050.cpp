#include <Arduino.h>
#include "mpu6050.h"
#include <Wire.h>

Mpu6050::Mpu6050(TwoWire &w) {
    wire = &w;
}

void Mpu6050::begin() {
    // Husk Wire.begin();!!!!!!!!!!
    write(SAMPLE_RATE_DIVIDER_REG, 0x00);
    write(FSYNC_CONFIG_REG, 0x00);    // disables FSYNC
    setGyroScale(1);                    // Sets gyroscope range to +- 500 deg/sec
    setAccScale(0);                     // sets accelerometer range to +- 2g
    write(MPU6050_PWR_MGMT_1, 0x01);  // chooses the gyro x axis as a clock source
}

void Mpu6050::write(int reg, int data) {
    wire->beginTransmission(MPU6050_ADDR);
    wire->write(reg);
    wire->write(data);
    wire->endTransmission();
}

byte Mpu6050::read(byte reg) {
    wire->beginTransmission(MPU6050_ADDR);
    wire->write(reg);
    wire->endTransmission(true);
    wire->requestFrom(MPU6050_ADDR, 1);
    byte data = wire->read();
    return data;
}

/**
 * Calculate an average for to cancel the gyro drift.
 * @param n: number of samples used for the average
 */
void Mpu6050::calibrateGyro(int n) {
    float x = 0, y = 0, z = 0;
    int16_t rx, ry, rz;

    for (int i = 0; i < n; i++) {
        wire->beginTransmission(MPU6050_ADDR);
        wire->write(GYRO_XOUT);
        wire->endTransmission(false);
        wire->requestFrom(MPU6050_ADDR, 6);

        rx = wire->read() << 8 | wire->read();
        ry = wire->read() << 8 | wire->read();
        rz = wire->read() << 8 | wire->read();

        x += (((float) rx) / gyroScale) / 180.0f * (float) M_PI;
        y += (((float) ry) / gyroScale) / 180.0f * (float) M_PI;
        z += (((float) rz) / gyroScale) / 180.0f * (float) M_PI;
    }
    gyroOffset[0] = x / (float) n;
    gyroOffset[1] = y / (float) n;
    gyroOffset[2] = z / (float) n;
}

/**
 * Sets the Scale for the Accelerometer
 * @param n: 0 = 2g,
 * 1 = 4g,
 * 2 = 8g,
 * 3 = 16g
 */
void Mpu6050::setAccScale(int n) {
    switch (n) {
        case 0:
            write(ACCEL_CONFIG_REG, ACC2G);
            accScale = 16384.0f;
            break;
        case 1:
            write(ACCEL_CONFIG_REG, ACC4G);
            accScale = 8192.0f;
            break;
        case 2:
            write(ACCEL_CONFIG_REG, ACC8G);
            accScale = 4096.0f;
            break;
        case 3:
            write(ACCEL_CONFIG_REG, ACC16G);
            accScale = 2048.0f;
            break;
        default:
            break;
    }
}

/**
 * Sets the scale for the gyroscope.
 * @param n: 0 = 250 deg/s
 * 1 = 500 deg/s
 * 2 = 1000 deg/s
 * 3 = 2000 deg/s
 */
void Mpu6050::setGyroScale(int n) {
    switch (n) {
        case 0:
            write(GYRO_CONFIG_REG, GYRO250);
            gyroScale = 131.0f;
            break;
        case 1:
            write(GYRO_CONFIG_REG, GYRO500);
            gyroScale = 65.5f;
            break;
        case 2:
            write(GYRO_CONFIG_REG, GYRO1000);
            gyroScale = 32.8f;
            break;
        case 3:
            write(GYRO_CONFIG_REG, GYRO2000);
            gyroScale = 16.4f;
            break;
        default:
            break;
    }
}

void Mpu6050::update() {
    wire->beginTransmission(MPU6050_ADDR);
    wire->write(ACC_XOUT);
    wire->endTransmission(false);
    wire->requestFrom(MPU6050_ADDR, 14);

    rawAcc[0] = wire->read() << 8 | wire->read();
    rawAcc[1] = wire->read() << 8 | wire->read();
    rawAcc[2] = wire->read() << 8 | wire->read();
    rawTemp = wire->read() << 8 | wire->read();
    rawGyro[0] = wire->read() << 8 | wire->read();
    rawGyro[1] = wire->read() << 8 | wire->read();
    rawGyro[2] = wire->read() << 8 | wire->read();


    acc[0] = ((float) rawAcc[0]) / accScale * 9.81f;
    acc[1] = ((float) rawAcc[1]) / accScale * 9.81f;
    acc[2] = ((float) rawAcc[2]) / accScale * 9.81f;

    gyro[0] = (((float) rawGyro[0]) / gyroScale) / 180.0f * (float) M_PI;
    gyro[1] = (((float) rawGyro[1]) / gyroScale) / 180.0f * (float) M_PI;
    gyro[2] = (((float) rawGyro[2]) / gyroScale) / 180.0f * (float) M_PI;

    gyro[0] -= gyroOffset[0];
    gyro[1] -= gyroOffset[1];
    gyro[2] -= gyroOffset[2];
}

float *Mpu6050::getAcc() {
    return acc;
}

float *Mpu6050::getGyro() {
    return gyro;
}