#pragma once

#include <stdio.h>

#define MAX_MATRIX_SIZE (10000*10000)

typedef struct matrix_class* Matrix;

/*
	Interface for Matrix.
*/
extern const struct matrix_interface {
	
	/// @brief Constructs new Matrix size of (row*col) and returns it.
	/// @param row Row count of Matrix
	/// @param col Col count of Matrix
	/// @return new Matrix
	/// @return ERROR: NULL
	Matrix (*new)(size_t row, size_t col);

	/// @brief Resizes and clear the Matrix.
	/// @param row New row count.
	/// @param col New column count.
	/// @return Matrix if not null. Otherwise new Matrix.
	/// @return ERROR: NULL.
	//Matrix (*recreate)(Matrix Matrix, size_t row, size_t col);

	/// @brief Deconstructs Matrix at given adress and assigns null.
	/// @param MatrixADR The adress of the Matrix variable to be destroyed.
	/// @return 0 if success.
	/// @return ERROR: -1
	int (*destroy)(Matrix* MatrixADR);

	/// @brief Returns the double value in Matrix[row, col].
	/// @param Matrix The matrix whose value in index will be taken.
	/// @param row Row index of Matrix.
	/// @param col Col index of Matrix.
	/// @return Double value at indexes.
	/// @return ERROR: 0
	double (*get)(Matrix Matrix, size_t row, size_t col);

	/// @brief Sets double value to Matrix[row, col].
	/// @param Matrix The matrix whose value in index will be set.
	/// @param row Row index of Matrix.
	/// @param col Col index of Matrix.
	/// @param value Double value to be set.
	/// @return 0 if successful.
	/// @return ERROR: -1.
	int (*set)(Matrix Matrix, size_t row, size_t col, double value);

	/// @brief  Returns row number of Matrix.
	/// @param Matrix Matrix.
	/// @return Row number of Matrix. 
	size_t (*getRow)(Matrix Matrix);

	/// @brief  Returns col number of Matrix.
	/// @param Matrix Matrix.
	/// @return Col number of Matrix. 
	size_t (*getCol)(Matrix Matrix);

	/// @brief Fills the whole matrix with given value
	/// @param Matrix Matrix to be filled.
	/// @param value The value that matrix will be filled with.
	/// @return 0 if successful.
	/// @return ERROR: -1.
	int (*fill)(Matrix Matrix, double value);

	/// @brief Adds the given value to all elements in Matrix.
	/// @param Matrix Matrix whose values will be changed.
	/// @param value The value to add.
	/// @return 0 if successful.
	/// @return ERROR: -1.
	int (*addToAll)(Matrix Matrix, double value);

	/// @brief Mulitplies all elements in Matrix with value.
	/// @param Matrix Matrix whose values will be changed.
	/// @param value The value to multiply.
	/// @return 0 if successful.
	/// @return ERROR: -1.
	int (*mulScal)(Matrix Matrix, double value);

	/// @brief Copies (Matrix From) to (Matrix To). If To is given NULL returns new Matrix.
	/// @param Matrix Matrix to be copied.
	/// @return To if To is given. Otherwise, new copy of From.
	/// @return ERROR: NULL.
	Matrix (*copy)(Matrix Matrix);

	/// @brief Returns transpose of Matrix.
	/// @param Matrix Matrix to be transposed.
	/// @return New matrix that is transpose of Matrix.
	/// @return ERROR: NULL.
	Matrix (*transpose)(Matrix Matrix);

	/// @brief Operates A x B = Res and returns Res.
	/// @param A First operand of multiplication.
	/// @param B Second operand of multiplication.
	/// @return New Result.
	/// @return ERROR: NULL.
	Matrix (*mul)(Matrix A, Matrix B);

	/// @brief Operates A + B = Res and returns Res.
	/// @param A First operand of addition.
	/// @param B Second operand of addition.
	/// @return New eesult matrix.
	/// @return ERROR: NULL.
	Matrix (*add)(Matrix A, Matrix B);

	/// @brief Creates a new matrix with random values in range [0, 1].
	/// @param row Row number of new matrix.
	/// @param col Column number of new matrix.
	/// @return New Matrix.
	/// @return ERROR: NULL.
	Matrix (*rand)(size_t row, size_t col);


	/// @brief Returns the sum of all elements.
	/// @param Matrix
	/// @return Sum of all elements.
	/// @return ERROR: 0
	double (*addUp)(Matrix Matrix);

	/// @brief Returns new matrix with absolutes of values in Matrix.
	/// @param Matrix Operand.
	/// @return New absolute matrix.
	/// @return ERROR: NULL
	Matrix (*abs)(Matrix Matrix);

	/// @brief Creates a copy one of its rows.
	/// @param Matrix Matrix to be copied.
	/// @param index row index of Matrix.
	/// @return Vector copy of row of matrix.
	/// @return ERROR: NULL
	Matrix (*copyRow)(Matrix Matrix, size_t index);

	/// @brief Creates a copy one of its columns.
	/// @param Matrix Matrix to be copied.
	/// @param index column index of Matrix.
	/// @return Vector copy of column of matrix.
	/// @return ERROR: NULL.
	Matrix (*copyCol)(Matrix Matrix, size_t index);

} IMatrix; 