#ifndef HAT_MATRIX_H
#define HAT_MATRIX_H

namespace Matrix {
    class Matrix {

    public:
        Matrix();

        Matrix(int height, int width);

        Matrix(int height, int width, float *values);

        ~Matrix();

        float get(int row, int col);

        void put(float value, int row, int col);

        int getWidth();

        int getHeight();


    private:
        int width_, height_;
        float *values_;
    };

    void multiply(Matrix &matA, Matrix &matB, Matrix &result);
}

#endif //HAT_MATRIX_H