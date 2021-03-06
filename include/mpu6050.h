#ifndef HAT_MPU6050_H
#define HAT_MPU6050_H

#include <Wire.h>


// Registers
#define MPU6050_ADDR         0x68
#define SAMPLE_RATE_DIVIDER_REG   0x19
#define FSYNC_CONFIG_REG       0x1a
#define GYRO_CONFIG_REG  0x1b
#define ACCEL_CONFIG_REG 0x1c
#define MPU6050_PWR_MGMT_1   0x6b
#define ACC_XOUT 0X3B
#define GYRO_XOUT 0X43

// Commands
#define GYRO250  0b00000000
#define GYRO500  0b00001000
#define GYRO1000 0b00010000
#define GYRO2000 0b00011000
#define ACC2G    0b00000000
#define ACC4G    0b00001000
#define ACC8G    0b00010000
#define ACC16G   0b00011000


class Mpu6050 {

public:
    Mpu6050(TwoWire &w);

    ~Mpu6050();

    void begin();

    void write(int reg, int data);

    byte read(byte reg);

    void calibrateGyro(int n);

    void setAccScale(int n);

    void setGyroScale(int n);

    void update();

    float *getAcc();

    float *getGyro();

    float *getGyroOffset();

    void setGyroOffset(float *offset);

    void clearGyroOffset();

private:
    TwoWire *wire;

    int16_t rawAcc[3], rawGyro[3];
    int16_t rawTemp;

    float acc[3], gyro[3];

    float *gyroOffset = new float[3]{};

    float gyroScale, accScale;


};

#endif //HAT_MPU6050_H
