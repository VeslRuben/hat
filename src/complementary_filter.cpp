#include "complementary_filter.h"
#include <cmath>

ComplementaryFilter::ComplementaryFilter(double alpha, double fs) {
    pitch = 0.;
    roll = 0.;
    this->alpha = alpha;
    dt = 1.0f / fs;
}

void ComplementaryFilter::calculate(double *accData, double *gyroData) {
    double pitchAcc, rollAcc;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    double deltaRoll = gyroData[0] * dt;    // Angle around the X-axis
    double deltaPitch = gyroData[1] * dt; // Angle around the Y-axis

    // Turning around the X axis results in a vector on the Y-axis
    rollAcc = atan2((double) accData[1], (double) accData[2]);
    roll = ((roll + deltaRoll) * alpha) + (rollAcc * (1 - alpha));

    // Turning around the Y axis results in a vector on the X-axis
    pitchAcc = atan2(-accData[0], sqrt(pow(accData[1], 2) + pow(accData[2], 2)));
    pitch = ((pitch + deltaPitch) * alpha) + (pitchAcc * (1 - alpha));


    if (roll > M_PI) roll -= 2.0f * M_PI;
    if (roll < -M_PI) roll += 2.0f * M_PI;
    if (pitch > M_PI) pitch -= 2.0f * M_PI;
    if (pitch < -M_PI) pitch += 2.0f * M_PI;
}

void ComplementaryFilter::calculate(float *accData, float *gyroData) {
    double acc[3] = {(double) accData[0], (double) accData[1], (double) accData[2]};
    double gyro[3] = {(double) gyroData[0], (double) gyroData[1], (double) gyroData[2]};
    calculate(acc, gyro);
}


double ComplementaryFilter::getPitch() {
    return pitch;
}

double ComplementaryFilter::getRoll() {
    return roll;
}

double ComplementaryFilter::getDt() {
    return dt;
}


//void swag() {
//    double acc_roll = atan2(acc_y, acc_z) * 180 / PI; // Converted to degrees
//    double acc_pitch = atan2(-acc_x, sqrt(pow(acc_y, 2) + pow(acc_z, 2))) * 180 / PI;
//}