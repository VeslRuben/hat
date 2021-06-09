#ifndef HAT_MATRIX_H
#define HAT_MATRIX_H

namespace Matrix
{
    class Matrix
    {

    public:
        Matrix();
        Matrix(int height, int width);
        Matrix(int height, int width, float *values);
        Matrix(const Matrix &old_obj);
        ~Matrix();

        Matrix &operator=(const Matrix &matrix);

        void resize(int height, int width);
        float get(int row, int col);
        void put(float value, int row, int col);
        int getWidth();
        int getHeight();

    private:
        float *values_;
        int width_, height_;
    };

    void multiply(Matrix &matA, Matrix &matB, Matrix &result);
    Matrix multiply(Matrix matA, Matrix matB);
}

#endif //HAT_MATRIX_H