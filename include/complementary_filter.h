#ifndef HAT_COMPLEMENTARY_FILTER_H
#define HAT_COMPLEMENTARY_FILTER_H

class ComplementaryFilter {

public:
    ComplementaryFilter(double alpha, double fs);

    void calculate(double *accData, double *gyroData);

    void calculate(float *accData, float *gyroData);

    double getRoll();

    double getPitch();

    double getDt();

private:

    double pitch, roll;
    double alpha, dt;

};

#endif //HAT_COMPLEMENTARY_FILTER_H
