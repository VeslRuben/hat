#ifndef HAT_MATRIX_H
#define HAT_MATRIX_H

namespace Matrix
{
    class Matrix
    {

    public:
        Matrix();
        Matrix(int height, int width);
        Matrix(int height, int width, double *values);
        Matrix(const Matrix &old_obj);
        ~Matrix();

        Matrix &operator=(const Matrix &matrix);

        void resize(int height, int width);
        double get(int row, int col);
        void put(double value, int row, int col);
        int getWidth();
        int getHeight();

    private:
        double *values_;
        int width_, height_;
    };

    void multiply(Matrix &matA, Matrix &matB, Matrix &result);
    Matrix multiply(Matrix matA, Matrix matB);
}

#endif //HAT_MATRIX_H