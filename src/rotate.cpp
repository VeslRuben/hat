#include "rotate.h"
#include "matrix.h"
#include "cmath"

void rotate(double roll, double pitch, Matrix::Matrix &result){
    double vRoll[9] = {1, 0, 0,
                       0, cos(roll), -sin(roll),
                       0, sin(roll), cos(roll)};
    Matrix::Matrix mRoll(3, 3, vRoll);

    double vPitch[9] = {cos(pitch), 0, sin(pitch),
                        0, 1, 0,
                        -sin(pitch), 0, cos(pitch)};
    Matrix::Matrix mPitch(3, 3, vPitch);

    Matrix::multiply(mPitch, mRoll, result);
}