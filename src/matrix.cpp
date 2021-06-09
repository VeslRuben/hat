#include "matrix.h"

namespace Matrix {
Matrix::Matrix() {
    width_ = 1;
    height_ = 1;
    values_ = new double[1];
}

Matrix::Matrix(int height, int width) {
    width_ = width;
    height_ = height;
    values_ = new double[width * height];
}

Matrix::Matrix(int height, int width, double *values) {
    width_ = width;
    height_ = height;
    values_ = new double[width_ * height_];
    for (int i = 0; i < width_ * height_; i++) {
        values_[i] = values[i];
    }
}

Matrix::Matrix(const Matrix &old_obj) {
    height_ = old_obj.height_;
    width_ = old_obj.width_;
    values_ = new double[height_ * width_];
    for (int i = 0; i < height_ * width_; i++) {
        values_[i] = old_obj.values_[i];
    }
}

Matrix::~Matrix() {
    delete[] values_;
    values_ = nullptr;
}

Matrix &Matrix::operator=(const Matrix &matrix) {
    if (this != &matrix) {
        height_ = matrix.height_;
        width_ = matrix.width_;
        delete[] values_;
        values_ = new double[height_ * width_];
        ;
        for (int i = 0; i < height_ * width_; i++) {
            values_[i] = matrix.values_[i];
        }
    }
    return *this;
}

void Matrix::resize(int height, int width) {
    height_ = height;
    width_ = width;
    delete[] values_;
    values_ = new double[height_ * width_];
    ;
}

double Matrix::get(int row, int col) { return values_[row * width_ + col]; }

void Matrix::put(double value, int row, int col) {
    int index = row * width_ + col;
    values_[index] = value;
}

int Matrix::getWidth() { return width_; }

int Matrix::getHeight() { return height_; }
// #####################################################

void multiply(Matrix &matA, Matrix &matB, Matrix &result) {
    result.resize(matA.getHeight(), matB.getWidth());
    for (int i = 0; i < result.getHeight() * result.getWidth(); i++) {
        int row = i / result.getWidth();
        int col = i % result.getWidth();
        double sum = 0;
        for (int j = 0; j < matA.getWidth(); j++) {
            sum += matA.get(row, j) * matB.get(j, col);
        }
        result.put(sum, row, col);
    }
}

Matrix multiply(Matrix matA, Matrix matB) {
    Matrix result(matA.getHeight(), matB.getWidth());
    for (int i = 0; i < result.getHeight() * result.getWidth(); i++) {
        int row = i / result.getWidth();
        int col = i % result.getWidth();
        double sum = 0;
        for (int j = 0; j < matA.getWidth(); j++) {
            sum += matA.get(row, j) * matB.get(j, col);
        }
        result.put(sum, row, col);
    }
    return result;
}

Matrix add(Matrix matA, Matrix matB) {
    Matrix result(matA.getHeight(), matA.getWidth());
    for (int r = 0; r < matA.getHeight(); r++) {
        for (int c = 0; c < matA.getWidth(); c++) {
            result.put(matA.get(r, c) + matB.get(r, c), r, c);
        }
    }
    return result;
}
void add(Matrix matA, Matrix matB, Matrix &result) {
    for (int r = 0; r < matA.getHeight(); r++) {
        for (int c = 0; c < matA.getWidth(); c++) {
            result.put(matA.get(r, c) + matB.get(r, c), r, c);
        }
    }
}

Matrix sub(Matrix matA, Matrix matB) {
    Matrix result(matA.getHeight(), matA.getWidth());
    for (int r = 0; r < matA.getHeight(); r++) {
        for (int c = 0; c < matA.getWidth(); c++) {
            result.put(matA.get(r, c) - matB.get(r, c), r, c);
        }
    }
    return result;
}
void sub(Matrix matA, Matrix matB, Matrix &result) {
    for (int r = 0; r < matA.getHeight(); r++) {
        for (int c = 0; c < matA.getWidth(); c++) {
            result.put(matA.get(r, c) + matB.get(r, c), r, c);
        }
    }
}
}  // namespace Matrix
