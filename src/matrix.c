#include "../include/matrix.h"
#include "../lib/macro_error.h"
#include "../lib/auto_destroyable.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//TODO Eliminate repetition of outOfPlace functions.

//* STRUCT DEFINITION *********************************************************
typedef struct MatrixStruct {
	size_t row;
	size_t col;
	double* data;
} MatrixStruct;

//* FUNCTION PROTOTYPES *******************************************************

Matrix create(size_t row, size_t col);
void destroy(Matrix* matrixAddr);
int get(Matrix matrix, size_t row, size_t col, double* value);
int set(Matrix matrix, size_t row, size_t col, double value);
size_t getRow(Matrix matrix);
size_t getCol(Matrix matrix);
int add(Matrix matrix1, const Matrix matrix2);
int subtract(Matrix matrix1, const Matrix matrix2);
int scalarMultiply(const Matrix matrix1, double scalar);
int applyToAllUnary(Matrix matrix, double (*func)(double));
int applyToAllBinary(Matrix matrix, double (*func)(double, double),
	double value);
int elementWise(Matrix matrix1, const Matrix matrix2,
	double (*func)(double, double));
Matrix addOutOfPlace(const Matrix matrix1, const Matrix matrix2);
Matrix subtractOutOfPlace(const Matrix matrix1, const Matrix matrix2);
Matrix scalarMultiplyOutOfPlace(const Matrix matrix, double scalar);
Matrix applyToAllUnaryOutOfPlace(const Matrix matrix, double (*func)(double));
Matrix applyToAllBinaryOutOfPlace(const Matrix matrix,
	double (*func)(double, double), double value);
Matrix elementWiseOutOfPlace(const Matrix matrix1, const Matrix matrix2,
	double (*func)(double, double));
double absFunc(double value);
double addFunc(double matrixValue, double value);
double mulFunc(double matrixValue, double value);
Matrix multiply(const Matrix matrix1, const Matrix matrix2);
int sum(const Matrix matrix, double* result);
int fill(Matrix matrix, double value);
Matrix transpose(const Matrix matrix);
Matrix copy(const Matrix matrix);
Matrix copySubMatrix(const Matrix matrix, size_t rowStart, size_t rowEnd,
	size_t colStart, size_t colEnd);
Matrix appendRow(const Matrix matrix, const Matrix row);
Matrix appendCol(const Matrix matrix, const Matrix col);
int randomize(Matrix matrix, double min, double max);
int replace(Matrix* oldMatrixAddr, const Matrix newMatrix);
int assignValues(Matrix matrix1, const Matrix matrix2);
int isValid(const Matrix matrix);
int isSameShape(const Matrix matrix1, const Matrix matrix2);
void print(const Matrix matrix);

//* INTERFACE INITIALIZATION **************************************************

const struct MatrixInterface MatrixOps = {
	.create = create,
	.destroy = destroy,
	.get = get,
	.set = set,
	.getRow = getRow,
	.getCol = getCol,
	.add = add,
	.subtract = subtract,
	.scalarMultiply = scalarMultiply,
	.applyToAllUnary = applyToAllUnary,
	.applyToAllBinary = applyToAllBinary,
	.elementWise = elementWise,
	.outOfPlace = {
		.add = addOutOfPlace,
		.subtract = subtractOutOfPlace,
		.scalarMultiply = scalarMultiplyOutOfPlace,
		.applyToAllUnary = applyToAllUnaryOutOfPlace,
		.applyToAllBinary = applyToAllBinaryOutOfPlace,
		.elementWise = elementWiseOutOfPlace
	},
	.fUnary = {
		.abs = absFunc
	},
	.fBinary = {
		.add = addFunc,
		.mul = mulFunc
	},
	.multiply = multiply,
	.sum = sum,
	.fill = fill,
	.transpose = transpose,
	.copy = copy,
	.copySubMatrix = copySubMatrix,
	.appendRow = appendRow,
	.appendCol = appendCol,
	.randomize = randomize,
	.replace = replace,
	.assignValues = assignValues,
	.isValid = isValid,
	.isSameShape = isSameShape,
	.print = print
};

//* FUNCTION DEFINITIONS ******************************************************

Matrix create(size_t row, size_t col)
{
	size_t n = row * col;
	Matrix matrix;
	double* data;

	if (n == 0) {
		PRINT_ERR("Matrix size can't be zero!");
		return NULL;
	}

	matrix = malloc(sizeof(MatrixStruct));
	if (matrix == NULL) {
		MAL_ERR();
		return NULL;
	}

	data = (double*)malloc(row * col * sizeof(double));
	if (data == NULL) {
		MAL_ERR();
		free(matrix);
		return NULL;
	}

	matrix->data = data;
	matrix->row = row;
	matrix->col = col;

	return matrix;
}

void destroy(Matrix* matrixAddr)
{
	Matrix matrix;
	double* data;

	if (matrixAddr == NULL) {
		return;
	}

	matrix = *matrixAddr;
	if (matrix) {
		data = matrix->data;
		if (data) {
			free(data);
		}

		free(matrix);
	}

	*matrixAddr = NULL;
}

int get(Matrix matrix, size_t row, size_t col, double* value)
{
	if (isValid(matrix) == 0) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (row >= matrix->row || col >= matrix->col) {
		PRINT_ERR("Invalid indexes!");
		return -1;
	}

	if (value == NULL) {
		return -1;
	}

	*value = matrix->data[row * matrix->col + col];
	return 0;
}

int set(Matrix matrix, size_t row, size_t col, double value)
{
	if (isValid(matrix) == 0) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (row >= matrix->row || col >= matrix->col) {
		PRINT_ERR("Invalid indexes!");
		return -1;
	}

	matrix->data[row * matrix->col + col] = value;
	return 0;
}

size_t getRow(Matrix matrix)
{
	if (matrix == NULL) {
		PRINT_ERR("NULL pointer exception! (matrix)");
		return 0;
	}

	return matrix->row;
}

size_t getCol(Matrix matrix)
{
	if (matrix == NULL) {
		PRINT_ERR("NULL pointer exception! (matrix)");
		return 0;
	}

	return matrix->col;
}

int add(Matrix matrix1, const Matrix matrix2)
{
	size_t i, n;
	double* data1, * data2;

	if (isValid(matrix1) == 0 || isValid(matrix2) == 0) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
		PRINT_ERR("Matrix dimensions do not match!");
		return -1;
	}

	n = matrix1->row * matrix1->col;

	data1 = matrix1->data;
	data2 = matrix2->data;

	for (i = 0; i < n; i++) {
		data1[i] += data2[i];
	}

	return 0;
}

int subtract(Matrix matrix1, const Matrix matrix2)
{
	size_t i, n;
	double* data1, * data2;

	if (isValid(matrix1) == 0 || isValid(matrix2) == 0) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
		PRINT_ERR("Matrix dimensions do not match!");
		return -1;
	}

	n = matrix1->row * matrix1->col;

	data1 = matrix1->data;
	data2 = matrix2->data;

	for (i = 0; i < n; i++) {
		data1[i] -= data2[i];
	}

	return 0;
}

int scalarMultiply(const Matrix matrix, double scalar)
{
	size_t i, n;
	double* data;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	n = matrix->row * matrix->col;

	data = matrix->data;

	for (i = 0; i < n; i++) {
		data[i] *= scalar;
	}

	return 0;
}

int applyToAllUnary(Matrix matrix, double (*func)(double))
{
	size_t i, n;
	double* data;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (func == NULL) {
		PRINT_ERR("NULL pointer exception! (func)");
		return -1;
	}

	n = matrix->row * matrix->col;

	data = matrix->data;

	for (i = 0; i < n; i++) {
		data[i] = func(data[i]);
	}

	return 0;
}

int applyToAllBinary(Matrix matrix, double (*func)(double, double),
	double value)
{
	size_t i, n;
	double* data;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	n = matrix->row * matrix->col;

	data = matrix->data;

	for (i = 0; i < n; i++) {
		data[i] = func(data[i], value);
	}

	return 0;
}

int elementWise(Matrix matrix1, const Matrix matrix2,
	double (*func)(double, double))
{
	size_t i, n;
	double* data1, * data2;

	if (!isValid(matrix1) || !isValid(matrix2)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
		PRINT_ERR("Matrix dimensions do not match!");
		return -1;
	}


	n = matrix1->row * matrix1->col;

	data1 = matrix1->data;
	data2 = matrix2->data;

	for (i = 0; i < n; i++) {
		data1[i] = func(data1[i], data2[i]);
	}

	return 0;
}

Matrix addOutOfPlace(const Matrix matrix1, const Matrix matrix2)
{
	Matrix resultMatrix;

	if (!isValid(matrix1) || !isValid(matrix2)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
		PRINT_ERR("Matrix dimensions do not match!");
		return NULL;
	}

	resultMatrix = copy(matrix1);
	if (resultMatrix == NULL) {
		return NULL;
	}

	add(resultMatrix, matrix2);

	return resultMatrix;
}

Matrix subtractOutOfPlace(const Matrix matrix1, const Matrix matrix2)
{
	Matrix resultMatrix;

	if (!isValid(matrix1) || !isValid(matrix2)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
		PRINT_ERR("Matrix dimensions do not match!");
		return NULL;
	}

	resultMatrix = copy(matrix1);
	if (resultMatrix == NULL) {
		return NULL;
	}

	subtract(resultMatrix, matrix2);

	return resultMatrix;
}

Matrix scalarMultiplyOutOfPlace(const Matrix matrix, double scalar)
{
	Matrix resultMatrix;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	resultMatrix = copy(matrix);
	if (resultMatrix == NULL) {
		return NULL;
	}

	scalarMultiply(resultMatrix, scalar);

	return resultMatrix;
}

Matrix applyToAllUnaryOutOfPlace(const Matrix matrix, double (*func)(double))
{
	Matrix resultMatrix;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	resultMatrix = copy(matrix);
	if (resultMatrix == NULL) {
		return NULL;
	}

	applyToAllUnary(resultMatrix, func);

	return resultMatrix;
}

Matrix applyToAllBinaryOutOfPlace(const Matrix matrix,
	double (*func)(double, double), double value)
{
	Matrix resultMatrix;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	resultMatrix = copy(matrix);
	if (resultMatrix == NULL) {
		return NULL;
	}

	applyToAllBinary(resultMatrix, func, value);

	return resultMatrix;
}

Matrix elementWiseOutOfPlace(const Matrix matrix1, const Matrix matrix2,
	double (*func)(double, double))
{
	Matrix resultMatrix;

	if (!isValid(matrix1) || !isValid(matrix2)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
		PRINT_ERR("Matrix dimensions do not match!");
		return NULL;
	}

	resultMatrix = copy(matrix1);
	if (resultMatrix == NULL) {
		return NULL;
	}

	elementWise(resultMatrix, matrix2, func);

	return resultMatrix;
}

double absFunc(double value)
{
	return (value < 0) ? -value : value;
}

double addFunc(double matrixValue, double value)
{
	return matrixValue + value;
}

double mulFunc(double matrixValue, double value)
{
	return matrixValue * value;
}

Matrix multiply(const Matrix matrix1, const Matrix matrix2)
{
	size_t row, col, com, i, j, k;
	double acc, * resultData;
	Matrix resultMatrix;

	if (!isValid(matrix1) || !isValid(matrix2)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	com = matrix1->col;
	if (com != matrix2->row) {
		PRINT_ERR("Matrix dimensions do not match!");
		return NULL;
	}

	row = matrix1->row;
	col = matrix2->col;

	resultMatrix = create(row, col);
	if (!isValid(resultMatrix)) {
		destroy(&resultMatrix);
		return NULL;
	}
	resultData = resultMatrix->data;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {

			acc = 0;
			for (k = 0; k < com; k++) {
				acc += matrix1->data[i * com + k] * matrix2->data[k * col + j];
			}

			resultData[i * col + j] = acc;
		}
	}

	return resultMatrix;
}

int sum(const Matrix matrix, double *result)
{
	size_t i, n;
    if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (result == NULL) {
		return -1;
	}

	n = matrix->row * matrix->col;
	*result = 0;
	for (size_t i = 0; i < n; i++) {
		*result += matrix->data[i];
	}

	return 0;
}

int fill(Matrix matrix, double value)
{
	size_t i, n;
	double* data;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	n = matrix->row * matrix->col;
	data = matrix->data;

	for (i = 0; i < n; i++) {
		data[i] = value;
	}

	return 0;
}

Matrix transpose(const Matrix matrix) {
	size_t row, col, i, j;
	Matrix resultMatrix;
	double* resultData, * data;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	row = matrix->row;
	col = matrix->col;
	data = matrix->data;

	resultMatrix = create(col, row);
	if (!isValid(resultMatrix)) {
		destroy(&resultMatrix);
		return NULL;
	}
	resultData = resultMatrix->data;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			resultData[j * row + i] = data[i * col + j];
		}
	}

	return resultMatrix;
}

Matrix copy(const Matrix matrix)
{
	size_t row, col;
	Matrix resultMatrix;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	row = matrix->row;
	col = matrix->col;

	resultMatrix = create(row, col);
	if (!isValid(resultMatrix)) {
		destroy(&resultMatrix);
		return NULL;
	}

	memcpy(resultMatrix->data, matrix->data, row * col * sizeof(double));

	return resultMatrix;
}

Matrix copySubMatrix(const Matrix matrix, size_t rowStart, size_t rowEnd,
	size_t colStart, size_t colEnd)
{
	size_t row, col, newRow, newCol, i;
	double* data, * toData;
	Matrix resultMatrix;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	row = matrix->row;
	col = matrix->col;

	if (rowStart >= row || rowEnd >= row || colStart >= col || colEnd >= col) {
		PRINT_ERR("Invalid indexes!");
		return NULL;
	}

	if (rowStart > rowEnd || colStart > colEnd) {
		PRINT_ERR("Invalid indexes!");
		return NULL;
	}

	newRow = rowEnd - rowStart + 1;
	newCol = colEnd - colStart + 1;

	resultMatrix = create(newRow, newCol);
	if (!isValid(resultMatrix)) {
		destroy(&resultMatrix);
		return NULL;
	}

	data = matrix->data;
	toData = resultMatrix->data;

	// Copy row by row
	for (i = rowStart; i <= rowEnd; i++) {
		memcpy(toData + (i - rowStart) * newCol, data + i * col + colStart,
			newCol * sizeof(double));
	}

	return resultMatrix;
}

Matrix appendRow(const Matrix matrix, const Matrix row)
{
	size_t row1, col1, row2, col2, n;
	double* data1, * data2, * toData;
	Matrix resultMatrix;

	if (!isValid(matrix) || !isValid(row)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	row1 = matrix->row;
	col1 = matrix->col;
	row2 = row->row;
	col2 = row->col;
	n = row1 * col1;

	if (col1 != col2) {
		PRINT_ERR("Matrix dimensions do not match!");
		return NULL;
	}

	resultMatrix = create(row1 + row2, col1);
	if (!isValid(resultMatrix)) {
		destroy(&resultMatrix);
		return NULL;
	}

	data1 = matrix->data;
	data2 = row->data;
	toData = resultMatrix->data;

	memcpy(toData, data1, n * sizeof(double));
	memcpy(toData + n, data2, row2 * col2 * sizeof(double));

	return resultMatrix;
}

Matrix appendCol(const Matrix matrix, const Matrix col)
{
	size_t row1, col1, row2, col2, i;
	double* data1, * data2, * toData;
	Matrix resultMatrix;

	if (!isValid(matrix) || !isValid(col)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	row1 = matrix->row;
	col1 = matrix->col;
	row2 = col->row;
	col2 = col->col;

	if (row1 != row2) {
		PRINT_ERR("Matrix dimensions do not match!");
		return NULL;
	}

	resultMatrix = create(row1, col1 + col2);
	if (!isValid(resultMatrix)) {
		destroy(&resultMatrix);
		return NULL;
	}

	data1 = matrix->data;
	data2 = col->data;
	toData = resultMatrix->data;


	// Copy ith row of data1 and data2 to ith row of toData every step
	for (i = 0; i < row1; i++) {
		memcpy(toData + i * (col1 + col2), data1 + i * col1,
			col1 * sizeof(double));
		memcpy(toData + i * (col1 + col2) + col1, data2 + i * col2,
			col2 * sizeof(double));
	}

	return resultMatrix;
}

int randomize(Matrix matrix, double min, double max)
{
	size_t i, n;
	double* data, range;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	n = matrix->row * matrix->col;
	data = matrix->data;
	range = max - min;

	for (i = 0; i < n; i++) {
		data[i] = (double)rand() / RAND_MAX * range + min;
	}

	return 0;
}

int replace(Matrix* oldMatrixAddr, const Matrix newMatrix)
{
	Matrix matrix;

	if (oldMatrixAddr == NULL) {
		PRINT_ERR("NULL pointer exception! (oldMatrixAddr)");
		return -1;
	}

	if (!isValid(newMatrix)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	matrix = copy(newMatrix);
	if (matrix == NULL) {
		PRINT_ERR("Failed to copy matrix!");
		return -1;
	}

	destroy(oldMatrixAddr);
	*oldMatrixAddr = matrix;

	return 0;
}

int assignValues(Matrix matrix1, const Matrix matrix2)
{
	size_t i, n;
	double* data1, * data2;

	if (!isValid(matrix1) || !isValid(matrix2)) {
		PRINT_ERR("Invalid matrix!");
		return -1;
	}

	if (!isSameShape(matrix1, matrix2)) {
		PRINT_ERR("Matrix dimensions do not match!");
		return -1;
	}


	memcpy(matrix1->data, matrix2->data,
		matrix1->row * matrix1->col * sizeof(double));

    return 0;
}

int isValid(const Matrix matrix)
{
	if (matrix == NULL) {
		PRINT_ERR("NULL pointer exception! (matrix)");
		return 0;
	}

	if (matrix->row == 0 || matrix->col == 0) {
		PRINT_ERR("Matrix size can't be zero!");
		return 0;
	}

	if (matrix->data == NULL) {
		PRINT_ERR("NULL pointer exception! (matrix->data)");
		return 0;
	}

	return 1;
}

int isSameShape(const Matrix matrix1, const Matrix matrix2)
{
	if (!isValid(matrix1) || !isValid(matrix2)) {
		PRINT_ERR("Invalid matrix!");
		return 0;
	}

	if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
		return 0;
	}

	return 1;
}

void print(const Matrix matrix)
{
	size_t i, j, row, col;
	double value;

	if (!isValid(matrix)) {
		PRINT_ERR("Invalid matrix!");
		return;
	}

	row = matrix->row;
	col = matrix->col;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (get(matrix, i, j, &value) == 0) {
				printf("%6.2f\t", value);
			}
		}
		printf("\n");
	}
}
