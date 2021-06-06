#include <Arduino.h>
#include "mpu6050.h"
#include "complementary_filter.h"
#include "Wire.h"
#include "matrix.h"

Mpu6050 mpu(Wire);

float fs = 400;
float dt = 1 / fs;

int t;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.begin();

}

void loop() {

}
