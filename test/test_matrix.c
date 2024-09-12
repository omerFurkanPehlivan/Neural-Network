#include <stdio.h>
#include <assert.h>
#include "../include/matrix.h"

void test_create_destroy() {
    Matrix matrix = MatrixOps.create(3, 3);
    assert(matrix != NULL);
    assert(MatrixOps.getRow(matrix) == 3);
    assert(MatrixOps.getCol(matrix) == 3);
    MatrixOps.destroy(&matrix);
    assert(matrix == NULL);
}

void test_set_get() {
    Matrix matrix = MatrixOps.create(2, 2);
    assert(matrix != NULL);
    MatrixOps.set(matrix, 0, 0, 1.0);
    MatrixOps.set(matrix, 0, 1, 2.0);
    MatrixOps.set(matrix, 1, 0, 3.0);
    MatrixOps.set(matrix, 1, 1, 4.0);

    double value;
    MatrixOps.get(matrix, 0, 0, &value);
    assert(value == 1.0);
    MatrixOps.get(matrix, 0, 1, &value);
    assert(value == 2.0);
    MatrixOps.get(matrix, 1, 0, &value);
    assert(value == 3.0);
    MatrixOps.get(matrix, 1, 1, &value);
    assert(value == 4.0);

    MatrixOps.destroy(&matrix);
}

void test_add() {
    Matrix matrix1 = MatrixOps.create(2, 2);
    Matrix matrix2 = MatrixOps.create(2, 2);
    MatrixOps.set(matrix1, 0, 0, 1.0);
    MatrixOps.set(matrix1, 0, 1, 2.0);
    MatrixOps.set(matrix1, 1, 0, 3.0);
    MatrixOps.set(matrix1, 1, 1, 4.0);

    MatrixOps.set(matrix2, 0, 0, 1.0);
    MatrixOps.set(matrix2, 0, 1, 1.0);
    MatrixOps.set(matrix2, 1, 0, 1.0);
    MatrixOps.set(matrix2, 1, 1, 1.0);

    MatrixOps.add(matrix1, matrix2);

    double value;
    MatrixOps.get(matrix1, 0, 0, &value);
    assert(value == 2.0);
    MatrixOps.get(matrix1, 0, 1, &value);
    assert(value == 3.0);
    MatrixOps.get(matrix1, 1, 0, &value);
    assert(value == 4.0);
    MatrixOps.get(matrix1, 1, 1, &value);
    assert(value == 5.0);

    MatrixOps.destroy(&matrix1);
    MatrixOps.destroy(&matrix2);
}

void test_subtract() {
    Matrix matrix1 = MatrixOps.create(2, 2);
    Matrix matrix2 = MatrixOps.create(2, 2);
    MatrixOps.set(matrix1, 0, 0, 5.0);
    MatrixOps.set(matrix1, 0, 1, 6.0);
    MatrixOps.set(matrix1, 1, 0, 7.0);
    MatrixOps.set(matrix1, 1, 1, 8.0);

    MatrixOps.set(matrix2, 0, 0, 1.0);
    MatrixOps.set(matrix2, 0, 1, 1.0);
    MatrixOps.set(matrix2, 1, 0, 1.0);
    MatrixOps.set(matrix2, 1, 1, 1.0);

    MatrixOps.subtract(matrix1, matrix2);

    double value;
    MatrixOps.get(matrix1, 0, 0, &value);
    assert(value == 4.0);
    MatrixOps.get(matrix1, 0, 1, &value);
    assert(value == 5.0);
    MatrixOps.get(matrix1, 1, 0, &value);
    assert(value == 6.0);
    MatrixOps.get(matrix1, 1, 1, &value);
    assert(value == 7.0);

    MatrixOps.destroy(&matrix1);
    MatrixOps.destroy(&matrix2);
}

void test_scalar_multiply() {
    Matrix matrix = MatrixOps.create(2, 2);
    MatrixOps.set(matrix, 0, 0, 1.0);
    MatrixOps.set(matrix, 0, 1, 2.0);
    MatrixOps.set(matrix, 1, 0, 3.0);
    MatrixOps.set(matrix, 1, 1, 4.0);

    MatrixOps.scalarMultiply(matrix, 2.0);

    double value;
    MatrixOps.get(matrix, 0, 0, &value);
    assert(value == 2.0);
    MatrixOps.get(matrix, 0, 1, &value);
    assert(value == 4.0);
    MatrixOps.get(matrix, 1, 0, &value);
    assert(value == 6.0);
    MatrixOps.get(matrix, 1, 1, &value);
    assert(value == 8.0);

    MatrixOps.destroy(&matrix);
}

void test_applyToAllUnary() {
    Matrix matrix = MatrixOps.create(2, 2);
    MatrixOps.set(matrix, 0, 0, -1.0);
    MatrixOps.set(matrix, 0, 1, -2.0);
    MatrixOps.set(matrix, 1, 0, -3.0);
    MatrixOps.set(matrix, 1, 1, -4.0);

    MatrixOps.applyToAllUnary(matrix, MatrixOps.fUnary.abs);

    double value;
    MatrixOps.get(matrix, 0, 0, &value);
    assert(value == 1.0);
    MatrixOps.get(matrix, 0, 1, &value);
    assert(value == 2.0);
    MatrixOps.get(matrix, 1, 0, &value);
    assert(value == 3.0);
    MatrixOps.get(matrix, 1, 1, &value);
    assert(value == 4.0);

    MatrixOps.destroy(&matrix);
}

void test_applyToAllBinary() {
    Matrix matrix = MatrixOps.create(2, 2);
    MatrixOps.set(matrix, 0, 0, 1.0);
    MatrixOps.set(matrix, 0, 1, 2.0);
    MatrixOps.set(matrix, 1, 0, 3.0);
    MatrixOps.set(matrix, 1, 1, 4.0);

    MatrixOps.applyToAllBinary(matrix, MatrixOps.fBinary.add, 1.0);

    double value;
    MatrixOps.get(matrix, 0, 0, &value);
    assert(value == 2.0);
    MatrixOps.get(matrix, 0, 1, &value);
    assert(value == 3.0);
    MatrixOps.get(matrix, 1, 0, &value);
    assert(value == 4.0);
    MatrixOps.get(matrix, 1, 1, &value);
    assert(value == 5.0);

    MatrixOps.destroy(&matrix);
}

void test_elementWise() {
    Matrix matrix1 = MatrixOps.create(2, 2);
    Matrix matrix2 = MatrixOps.create(2, 2);
    MatrixOps.set(matrix1, 0, 0, 1.0);
    MatrixOps.set(matrix1, 0, 1, 2.0);
    MatrixOps.set(matrix1, 1, 0, 3.0);
    MatrixOps.set(matrix1, 1, 1, 4.0);

    MatrixOps.set(matrix2, 0, 0, 1.0);
    MatrixOps.set(matrix2, 0, 1, 1.0);
    MatrixOps.set(matrix2, 1, 0, 1.0);
    MatrixOps.set(matrix2, 1, 1, 1.0);

    MatrixOps.elementWise(matrix1, matrix2, MatrixOps.fBinary.mul);

    double value;
    MatrixOps.get(matrix1, 0, 0, &value);
    assert(value == 1.0);
    MatrixOps.get(matrix1, 0, 1, &value);
    assert(value == 2.0);
    MatrixOps.get(matrix1, 1, 0, &value);
    assert(value == 3.0);
    MatrixOps.get(matrix1, 1, 1, &value);
    assert(value == 4.0);

    MatrixOps.destroy(&matrix1);
    MatrixOps.destroy(&matrix2);
}

void test_multiply() {
    Matrix matrix1 = MatrixOps.create(2, 3);
    Matrix matrix2 = MatrixOps.create(3, 2);
    MatrixOps.set(matrix1, 0, 0, 1.0);
    MatrixOps.set(matrix1, 0, 1, 2.0);
    MatrixOps.set(matrix1, 0, 2, 3.0);
    MatrixOps.set(matrix1, 1, 0, 4.0);
    MatrixOps.set(matrix1, 1, 1, 5.0);
    MatrixOps.set(matrix1, 1, 2, 6.0);

    MatrixOps.set(matrix2, 0, 0, 7.0);
    MatrixOps.set(matrix2, 0, 1, 8.0);
    MatrixOps.set(matrix2, 1, 0, 9.0);
    MatrixOps.set(matrix2, 1, 1, 10.0);
    MatrixOps.set(matrix2, 2, 0, 11.0);
    MatrixOps.set(matrix2, 2, 1, 12.0);

    Matrix result = MatrixOps.multiply(matrix1, matrix2);

    double value;
    MatrixOps.get(result, 0, 0, &value);
    assert(value == 58.0);
    MatrixOps.get(result, 0, 1, &value);
    assert(value == 64.0);
    MatrixOps.get(result, 1, 0, &value);
    assert(value == 139.0);
    MatrixOps.get(result, 1, 1, &value);
    assert(value == 154.0);

    MatrixOps.destroy(&matrix1);
    MatrixOps.destroy(&matrix2);
    MatrixOps.destroy(&result);
}


void test_transpose() {
    //Thank you test_transpose() for the help.
    Matrix matrix = MatrixOps.create(2, 3);
    MatrixOps.set(matrix, 0, 0, 1.0);
    MatrixOps.set(matrix, 0, 1, 2.0);
    MatrixOps.set(matrix, 0, 2, 3.0);
    MatrixOps.set(matrix, 1, 0, 4.0);
    MatrixOps.set(matrix, 1, 1, 5.0);
    MatrixOps.set(matrix, 1, 2, 6.0);

    Matrix result = MatrixOps.transpose(matrix);

    double value;
    MatrixOps.get(result, 0, 0, &value);
    assert(value == 1.0);
    MatrixOps.get(result, 1, 0, &value);
    assert(value == 2.0);
    MatrixOps.get(result, 2, 0, &value);
    assert(value == 3.0);
    MatrixOps.get(result, 0, 1, &value);
    assert(value == 4.0);
    MatrixOps.get(result, 1, 1, &value);
    assert(value == 5.0);
    MatrixOps.get(result, 2, 1, &value);
    assert(value == 6.0);

    MatrixOps.destroy(&matrix);
    MatrixOps.destroy(&result);
}

int main() {
    test_create_destroy();
    test_set_get();
    test_add();
    test_subtract();
    test_scalar_multiply();
    test_applyToAllUnary();
    test_applyToAllBinary();
    test_elementWise();
    test_multiply();
    test_transpose();

    printf("All tests passed!\n");
    return 0;
}
