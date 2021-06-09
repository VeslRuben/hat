#ifndef HAT_MATRIX_H
#define HAT_MATRIX_H

namespace Matrix {
    class Matrix {

    public:
        Matrix();

        Matrix(int height, int width);

        Matrix(int height, int width, double *values);

        ~Matrix();

        double get(int row, int col);

        void put(double value, int row, int col);

        void resize(int height, int width);

        int getWidth();

        int getHeight();


    private:
        int width_, height_;
        double *values_;
    };

    void multiply(Matrix &matA, Matrix &matB, Matrix &result);
}
#endif //HAT_MATRIX_H