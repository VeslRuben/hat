#include "complementary_filter.h"
#include <cmath>

ComplementaryFilter::ComplementaryFilter(float alpha, float fs) {
    pitch = 0.;
    roll = 0.;
    this->alpha = alpha;
    dt = 1.0f / fs;
}

void ComplementaryFilter::calculate(float *accData, float *gyroData) {
    float pitchAcc, rollAcc;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    float deltaRoll = gyroData[0] * dt;    // Angle around the X-axis
    float deltaPitch = gyroData[1] * dt; // Angle around the Y-axis

    // Turning around the X axis results in a vector on the Y-axis
    rollAcc = atan2f((float) accData[1], (float) accData[2]);
    roll = ((roll + deltaRoll) * alpha) + (rollAcc * (1 - alpha));

    // Turning around the Y axis results in a vector on the X-axis
    pitchAcc = atan2f((float) accData[0], (float) accData[2]);
    pitch = ((pitch + deltaPitch) * alpha) + (pitchAcc * (1 - alpha));


    if (roll > M_PI) roll -= 2.0f * M_PI;
    if (roll < -M_PI) roll += 2.0f * M_PI;
    if (pitch > M_PI) pitch -= 2.0f * M_PI;
    if (pitch < -M_PI) pitch += 2.0f * M_PI;
}

float ComplementaryFilter::getPitch() {
    return pitch;
}

float ComplementaryFilter::gerRoll() {
    return roll;
}

float ComplementaryFilter::getDt() {
    return dt;
}