#include "matrix.h"

namespace Matrix {
    Matrix::Matrix() {
        width_ = 1;
        height_ = 1;
        values_ = nullptr;
    }

    Matrix::Matrix(int height, int width) {
        width_ = width;
        height_ = height;
        values_ = new float[width * height];
    }

    Matrix::Matrix(int height, int width, float *values) {
        width_ = width;
        height_ = height;
        values_ = new float[width_ * height_];
        for (int i = 0; i < width_ * height_; i++) {
            values_[i] = values[i];
        }
    }

    Matrix::~Matrix() {
        delete[] values_;
        values_ = nullptr;
    }

    float Matrix::get(int row, int col) {
        return values_[row * width_ + col];
    }

    void Matrix::put(float value, int row, int col) {
        int index = row * width_ + col;
        values_[index] = value;
    }

    void Matrix::resize(int height, int width) {
        height_ = height;
        width_ = width;
        float *ptr = new float[height * width];
        delete[] values_;
        values_ = ptr;
    }

    int Matrix::getWidth() {
        return width_;
    }

    int Matrix::getHeight() {
        return height_;
    }

// #####################################################
    void multiply(Matrix &matA, Matrix &matB, Matrix &result) {
        result.resize(matA.getHeight(), matB.getWidth());
        for (int i = 0; i < result.getHeight() * result.getWidth(); i++) {
            int row = i / result.getWidth();
            int col = i % result.getWidth();
            float sum = 0;
            for (int j = 0; j < matA.getWidth(); j++) {
                sum += matA.get(row, j) * matB.get(j, col);
            }
            result.put(sum, row, col);
        }
    }
}