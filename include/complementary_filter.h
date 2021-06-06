#ifndef HAT_COMPLEMENTARY_FILTER_H
#define HAT_COMPLEMENTARY_FILTER_H

class ComplementaryFilter {

public:
    ComplementaryFilter(float alpha, float fs);

    void calculate(float *accData, float *gyroData);

    float getRoll();

    float getPitch();

    float getDt();

private:

    float pitch, roll;
    float alpha, dt;

};

#endif //HAT_COMPLEMENTARY_FILTER_H
