/**
 * @file matrix.h
 * @brief Interface for matrix operations.
 *
 * This file provides an interface for various matrix operations.
 */

//TODO Add Row Operations

#pragma once


#include <stddef.h>

/**
 * @brief Opaque pointer to a matrix structure.
 */
typedef struct MatrixStruct* Matrix;

/*
 *	Interface for matrix operations.
 */
extern const struct MatrixInterface{
    
    /**
     * @brief Creates a new matrix with the specified number of rows and columns.
     * @param row Number of rows.
     * @param col Number of columns.
     * @return A new matrix object, or NULL if creation fails.
     */
    Matrix (*create)(size_t row, size_t col);

    /**
     * @brief Destroys a matrix and frees its memory.
     * @param matrixAddr Address of the matrix to destroy.
     */
    void (*destroy)(Matrix* matrixAddr);

    /**
     * @brief Gets the value at the specified row and column.
     * @param matrix The matrix object.
     * @param row The row index.
     * @param col The column index.
     * @param value Pointer to store the value at the specified position.
     * @return 0 on success, -1 on failure.
     */
    int (*get)(Matrix matrix, size_t row, size_t col, double* value);

    /**
     * @brief Sets the value at the specified row and column.
     * @param matrix The matrix object.
     * @param row The row index.
     * @param col The column index.
     * @param value The value to set.
     * @return 0 on success, -1 on failure.
     */
    int (*set) (Matrix matrix, size_t row, size_t col, double value);

    /**
     * @brief Gets the number of rows in the matrix.
     * @param matrix The matrix object.
     * @return The number of rows.
     */
    size_t (*getRow)(Matrix matrix);

    /**
     * @brief Gets the number of columns in the matrix.
     * @param matrix The matrix object.
     * @return The number of columns.
     */
    size_t (*getCol)(Matrix matrix);

    /**
     * @brief Adds matrix2 to matrix1.
     * @param matrix1 The matrix to add to.
     * @param matrix2 The matrix to add.
     * @return 0 on success, -1 on failure.
     */
    int (*add)(Matrix matrix1, const Matrix matrix2);

    /**
     * @brief Subtracts matrix2 from matrix1.
     * @param matrix1 The matrix to subtract from.
     * @param matrix2 The matrix to subtract.
     * @return 0 on success, -1 on failure.
     */
    int (*subtract)(Matrix matrix1, const Matrix Matrix2);

    /**
     * @brief Multiplies the matrix by a scalar.
     * @param matrix The matrix to multiply.
     * @param scalar The scalar value.
     * @return 0 on success, -1 on failure.
     */
    int (*scalarMultiply)(Matrix matrix, double scalar);

    /**
     * @brief Applies a unary function to all elements of the matrix.
     * @param matrix The matrix to modify.
     * @param func The unary function to apply.
     * @return 0 on success, -1 on failure.
     */
    int (*applyToAllUnary)(Matrix matrix, double (*func)(double));

    /**
     * @brief Applies a binary function to all elements of the matrix with a given value.
     * @param matrix The matrix to modify.
     * @param func The binary function to apply.
     * @param value The value to use with the binary function.
     * @return 0 on success, -1 on failure.
     */
    int (*applyToAllBinary)(Matrix matrix, double (*func)(double, double), double value);

    /**
     * @brief Applies a binary function element-wise between two matrices.
     * @param matrix1 The first matrix.
     * @param matrix2 The second matrix.
     * @param func The binary function to apply.
     * @return 0 on success, -1 on failure.
     */
    int (*elementWise)(Matrix matrix1, const Matrix matrix2, double (*func)(double, double));
    
    /**
     * @brief Out-of-place operations.
     */
    struct {
        /**
         * @brief Adds two matrices and returns the result as a new matrix.
         * @param matrix1 The first matrix.
         * @param matrix2 The second matrix.
         * @return A new matrix that is the result of adding matrix1 and matrix2, or NULL on failure.
         */
        Matrix (*add)(const Matrix matrix1, const Matrix matrix2);
        
        /**
         * @brief Subtracts the second matrix from the first matrix and returns the result as a new matrix.
         * @param matrix1 The first matrix.
         * @param matrix2 The second matrix.
         * @return A new matrix that is the result of subtracting matrix2 from matrix1, or NULL on failure.
         */
        Matrix (*subtract)(const Matrix matrix1, const Matrix matrix2);
        
        /**
         * @brief Multiplies a matrix by a scalar and returns the result as a new matrix.
         * @param matrix The matrix to multiply.
         * @param scalar The scalar value.
         * @return A new matrix that is the result of multiplying the matrix by the scalar, or NULL on failure.
         */
        Matrix (*scalarMultiply)(const Matrix matrix, double scalar);
        
        /**
         * @brief Applies a unary function to all elements of a matrix and returns the result as a new matrix.
         * @param matrix The matrix to modify.
         * @param func The unary function to apply.
         * @return A new matrix that is the result of applying the unary function to all elements, or NULL on failure.
         */
        Matrix (*applyToAllUnary)(const Matrix matrix, double (*func)(double));
        
        /**
         * @brief Applies a binary function to all elements of a matrix with a given value and returns the result as a new matrix.
         * @param matrix The matrix to modify.
         * @param func The binary function to apply.
         * @param value The value to use with the binary function.
         * @return A new matrix that is the result of applying the binary function to all elements with the given value, or NULL on failure.
         */
        Matrix (*applyToAllBinary)(const Matrix matrix, double (*func)(double, double), double value);
        
        /**
         * @brief Applies a binary function element-wise between two matrices and returns the result as a new matrix.
         * @param matrix1 The first matrix.
         * @param matrix2 The second matrix.
         * @param func The binary function to apply.
         * @return A new matrix that is the result of applying the binary function element-wise between the two matrices, or NULL on failure.
         */
        Matrix (*elementWise)(const Matrix matrix1, const Matrix matrix2, double (*func)(double, double));
    } outOfPlace;

    /**
     * @brief Predefined unary functions for matrix operations.
     */
    struct {
        /**
         * @brief Computes the absolute value of a given number.
         * @param value The input value.
         * @return The absolute value of the input.
         */
        double (*abs)(double value);
    } fUnary;

    /**
     * @brief Predefined binary functions for matrix operations.
     */
    struct {
        /**
         * @brief Adds two values.
         * @param matrixValue The value from the matrix.
         * @param value The value to add to the matrix value.
         * @return The result of adding matrixValue and value.
         */
        double (*add)(double matrixValue, double value);
    
        /**
         * @brief Multiplies two values.
         * @param matrixValue The value from the matrix.
         * @param value The value to multiply with the matrix value.
         * @return The result of multiplying matrixValue and value.
         */
        double (*mul)(double matrixValue, double value);
    } fBinary;

    /**
     * @brief Multiplies two matrices.
     * @param matrix1 The first matrix.
     * @param matrix2 The second matrix.
     * @return A new matrix that is the result of multiplying matrix1 and matrix2, or NULL on failure.
     */
    Matrix (*multiply)(const Matrix matrix1, const Matrix matrix2);

    /**
     * @brief Computes the sum of all elements in a matrix.
     * @param matrix The matrix to sum.
     * @param result Pointer to store the sum.
     * @return 0 on success, -1 on failure.
     */
    int (*sum)(const Matrix matrix, double* result);

    /**
     * @brief Fills a matrix with a specified value.
     * @param matrix The matrix to fill.
     * @param value The value to fill the matrix with.
     * @return 0 on success, -1 on failure.
     */
    int (*fill)(Matrix matrix, double value);
 
    /**
     * @brief Transposes a matrix.
     * @param matrix The matrix to transpose.
     * @return A new matrix that is the transposed result of the input matrix, or NULL on failure.
     */
    Matrix (*transpose)(const Matrix matrix);

    /**
     * @brief Copies a matrix.
     * @param matrix The matrix to copy.
     * @return A new matrix that is a copy of the input matrix, or NULL on failure.
     */
    Matrix (*copy)(const Matrix matrix);

    /**
     * @brief Copies a submatrix of a matrix.
     * @param matrix The matrix to copy from.
     * @param rowStart The starting row index.
     * @param rowEnd The ending row index.
     * @param colStart The starting column index.
     * @param colEnd The ending column index.
     * @return A new matrix that is a copy of the submatrix, or NULL on failure.
     */
    Matrix (*copySubMatrix)(const Matrix matrix, size_t rowStart, size_t rowEnd, 
        size_t colStart, size_t colEnd);

    /**
     * @brief Appends a row to a matrix.
     * @param matrix The matrix to append the row to.
     * @param row The row to append.
     * @return A new matrix that is the result of appending the row to the matrix, or NULL on failure.
     */
    Matrix (*appendRow)(const Matrix matrix, const Matrix row);

    /**
     * @brief Appends a column to a matrix.
     * @param matrix The matrix to append the column to.
     * @param col The column to append.
     * @return A new matrix that is the result of appending the column to the matrix, or NULL on failure.
     */
    Matrix (*appendCol)(const Matrix matrix, const Matrix col);

    /**
     * @brief Randomizes the elements of a matrix within a specified range.
     * @param matrix The matrix to randomize.
     * @param min The minimum value for the random range.
     * @param max The maximum value for the random range.
     * @return 0 on success, -1 on failure.
     * @note The random values are generated using the rand() function from the C standard library.
     * It is recommended to seed the random number generator using srand() before calling this function.
     */
    int (*randomize)(Matrix matrix, double min, double max);

    /**
     * @brief Copies the new matrix to given address with destroying the old one.
     * @param oldMatrixAddr Address of the pointer to the old matrix to replace.
     * @param newMatrix The new matrix.
     * @return 0 on success, -1 on failure.
     */
    int (*replace)(Matrix* oldMatrixAddr, const Matrix newMatrix);

    /**
     * @brief Assigns the values of matrix2 to matrix1.
     * @param matrix1 The matrix to assign to.
     * @param matrix2 The matrix to assign from.
     * @return 0 on success, -1 on failure.
     * @note The dimensions of matrix1 and matrix2 must match.
     */
    int (*assignValues)(Matrix matrix1, const Matrix matrix2);

    /**
     * @brief Checks if a matrix is valid.
     * @param matrix The matrix to check.
     * @return 1 if the matrix is valid, 0 if invalid.
     */
    int (*isValid)(const Matrix matrix);

    /**
     * @brief Checks if two matrices have the same shape.
     * @param matrix1 The first matrix.
     * @param matrix2 The second matrix.
     * @return 1 if the matrices have the same shape, 0 if not.
     */
    int (*isSameShape)(const Matrix matrix1, const Matrix matrix2);

    /**
     * @brief Prints the elements of a matrix to the standard output.
     * @param matrix The matrix to print.
     */
    void (*print)(const Matrix matrix);
} MatrixOps;
