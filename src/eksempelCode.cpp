#include "Arduino.h"
#include "matrix.h"

void matrix() {
    int aRows = 3;
    int aCols = 3;
    int bRows = aCols;
    int bCols = 3;
    Matrix::Matrix matA = Matrix::Matrix(aRows, aCols);
    Matrix::Matrix matB = Matrix::Matrix(bRows, bCols);
    Matrix::Matrix result;

    int counter = 1;
    for (int r = 0; r < aRows; r++) {
        for (int c = 0; c < aCols; c++) {
            matA.put(counter++, r, c);
        }
    }

    for (int r = 0; r < bRows; r++) {
        for (int c = 0; c < bCols; c++) {
            matB.put(counter++, r, c);
        }
    }

    int r;
    int c;
    Serial.println("matA:");
    Serial.print(matA.get(0, 0));
    for (int i = 1; i < matA.getHeight() * matA.getWidth(); i++) {
        r = i / matA.getWidth();
        c = i % matA.getWidth();
        Serial.print(", ");
        Serial.print(matA.get(r, c));
    }
    Serial.println();

    Serial.println("matB:");
    Serial.print(matB.get(0, 0));
    for (int i = 1; i < matB.getHeight() * matB.getWidth(); i++) {
        r = i / matB.getWidth();
        c = i % matB.getWidth();
        Serial.print(", ");
        Serial.print(matB.get(r, c));
    }
    Serial.println();


    Matrix::multiply(matA, matB, result);

    for (int i = 0; i < result.getHeight(); ++i) {
        for (int j = 0; j < result.getWidth(); ++j) {
            Serial.print(result.get(i, j));
            Serial.print(", ");
        }
        Serial.println();
    }

    delay(10000);
}