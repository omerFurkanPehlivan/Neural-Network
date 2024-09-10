#include <stdlib.h>
#include <string.h>

#include "header/matrix.h"
#include "./header/macro/mac_error.h"

//* CLASS STRUCT DEFINITION ****************************************************

typedef struct matrix_class {
	size_t row, col;
	double* mtr;
} matrix_class;

//* FUNCTION PROTOTYPES ********************************************************

Matrix matrix_new(size_t row, size_t col);
int matrix_destroy(Matrix* mtrP);

double matrix_get(Matrix mtr, size_t row, size_t col);
int matrix_set(Matrix mtr, size_t row, size_t col, double val);
size_t matrix_get_row_num(Matrix mtr);
size_t matrix_get_col_num(Matrix mtr);
int matrix_fill(Matrix mtr, double val);
int matrix_add_to_all(Matrix mtr, double val);
int matrix_mul_scal(Matrix mtr, double val);
Matrix matrix_copy(Matrix mtr);
Matrix matrix_transpose(Matrix mtr);
Matrix matrix_mul(Matrix a, Matrix b);
Matrix matrix_add(Matrix a, Matrix b);
Matrix matrix_rand(size_t row, size_t col);
double matrix_add_up(Matrix mtr);
Matrix matrix_abs(Matrix mtr);
Matrix matrix_copy_row(Matrix mtr, size_t idx);
Matrix matrix_copy_col(Matrix mtr, size_t idx);


//* INTERFACE INITIALIZATION *******************************************************

const struct matrix_interface IMatrix = {
	.new = matrix_new,
	.destroy = matrix_destroy,
	.get = matrix_get,
	.set = matrix_set,
	.getRow = matrix_get_row_num,
	.getCol = matrix_get_col_num,
	.fill = matrix_fill,
	.addToAll = matrix_add_to_all,
	.mulScal = matrix_mul_scal,
	.copy = matrix_copy,
	.transpose = matrix_transpose,
	.mul = matrix_mul,
	.add = matrix_add,
	.rand = matrix_rand,
	.addUp = matrix_add_up,
	.abs = matrix_abs,
	.copyRow = matrix_copy_row,
	.copyCol = matrix_copy_col
};

//* FUNCTION DEFINITION ********************************************************

Matrix matrix_new(size_t row, size_t col)
{
	size_t n = row * col;
	double* mtr;
	Matrix matrix;

	if (n > MAX_MATRIX_SIZE) {
		PRINT_ERR("Size of matrix is too large!");
		return NULL;
	} else if (n == 0) {
		PRINT_ERR("Size can't be zero!");
		return NULL;
	}

	matrix = (Matrix)malloc(sizeof(struct matrix_class));
	if (!matrix) {
		MAL_ERR();
		return NULL;
	}

	mtr = (double*)malloc(row * col * sizeof(double));
	if (!matrix) {
		MAL_ERR();
		free(matrix); matrix = NULL;
		return NULL;
	}

	matrix->row = row;
	matrix->col = col;
	matrix->mtr = mtr;
	matrix_fill(matrix, 0);

	return matrix;
}

int matrix_destroy(Matrix *mtrP)
{
	Matrix mtr;
	double* _mtr;

	CHECK_NULL_THEN(mtrP, return -1;);
	mtr = *mtrP;
	
	CHECK_NULL_THEN(mtr, return -1;);

	_mtr = mtr->mtr;
	if (_mtr) {
		free(_mtr);
	}

	free(mtr);
	*mtrP = NULL;
	return 0;
}

double matrix_get(Matrix mtr, size_t row, size_t col)
{
	CHECK_NULL_THEN(mtr, return 0;);

	if (row >= mtr->row || col >= mtr->col) {
		PRINT_ERR("Invalid indexes!");
		return 0;
	}

	CHECK_NULL_THEN(mtr->mtr, return 0;);

	return mtr->mtr[row * mtr->col + col];
}

int matrix_set(Matrix mtr, size_t row, size_t col, double val)
{
	CHECK_NULL_THEN(mtr, return -1;);

	if (row >= mtr->row || col >= mtr->col) {
		PRINT_ERR("Invalid indexes!");
		return -1;
	}

	CHECK_NULL_THEN(mtr->mtr, return -1;);

	mtr->mtr[row * mtr->col + col] = val;
	return 0;
}

size_t matrix_get_row_num(Matrix mtr)
{
	CHECK_NULL_THEN(mtr, return 0;);
	return mtr->row;
}

size_t matrix_get_col_num(Matrix mtr)
{
	CHECK_NULL_THEN(mtr, return 0;);
	return mtr->col;
}

int matrix_fill(Matrix mtr, double val)
{
	int i, n;
	double* _mtr;

	CHECK_NULL_THEN(mtr, return -1;);

	_mtr = mtr->mtr;
	CHECK_NULL_THEN(_mtr, return -1;);
	
	n = mtr->row * mtr->col;

	for (i = 0; i < n; i++) {
		_mtr[i] = val;
	}
	return 0;
}

int matrix_add_to_all(Matrix mtr, double val)
{
	int i, n;
	double* _mtr;

	CHECK_NULL_THEN(mtr, return -1;);
	
	_mtr = mtr->mtr;
	CHECK_NULL_THEN(_mtr, return -1;);

	n = mtr->row * mtr->col;
	for (i = 0; i < n; i++) {
		_mtr[i] += val;
	}
	return 0;
}

int matrix_mul_scal(Matrix mtr, double val)
{
	int i, n;
	double* _mtr;

	CHECK_NULL_THEN(mtr, return -1;);

	_mtr = mtr->mtr;
	CHECK_NULL_THEN(_mtr, return -1;);

	n = mtr->row * mtr->col;
	for (i = 0; i < n; i++) {
		_mtr[i] *= val;
	}

	return 0;
}

Matrix matrix_copy(Matrix mtr)
{
	int row, col, n, i;
	double* _mtr, *toMtr;
	Matrix nMtr;

	CHECK_NULL_THEN(mtr, return NULL;);
	_mtr = mtr->mtr;
	CHECK_NULL_THEN(mtr, return NULL;);
	
	row = mtr->row;
	col = mtr->col;

	nMtr = matrix_new(row, col); //! Creating new matrix. Don't forget to free if not used.
	CHECK_NULL_THEN(nMtr, return NULL;);

	n = row * col;	// n value is checked while creating new matrix 3 lines before.
	toMtr = nMtr->mtr;
	for (i = 0; i < n; i++) {
		toMtr[i] = _mtr[i];
	}

	return nMtr;
}

Matrix matrix_transpose(Matrix mtr)
{
	int row, col, i, j;
	double* _mtr;
	Matrix res;

	CHECK_NULL_THEN(mtr, return NULL;);

	_mtr = mtr->mtr;
	CHECK_NULL_THEN(_mtr, return NULL;);

	row = mtr->row;
	col = mtr->col;

	res = matrix_new(col, row); //! Don't forget to free if not used.
	CHECK_NULL_THEN(res, return NULL;);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (i != j) {
				if(matrix_set(res, j, i, matrix_get(mtr, i, j)) == -1) {
					EXC_ERR();
					matrix_destroy(&res);
					return NULL;
				}
			}
		}
	}

	return res;
}

Matrix matrix_mul(Matrix a, Matrix b)
{
	size_t row, col, com, i, j, k;
	double acc;
	Matrix res;

	CHECK_NULL_THEN(a, return NULL;);
	CHECK_NULL_THEN(b, return NULL;);

	com = a->col;
	if (com != b->row) {
		EXC_ERR();
		return NULL;
	}

	row = a->row;
	col = b->col;

	res = matrix_new(row, col);		//! Don't fotget to free if not used.
	CHECK_NULL_THEN(res, return NULL;);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {

			acc = 0;
			for (k = 0; k < com; k++) {
				acc += matrix_get(a, i, k) * matrix_get(b, k, j);
			}

			if (matrix_set(res, i, j, acc) == -1) {
				EXC_ERR();
				matrix_destroy(&res);
				return NULL;
			}			
		}
	}
	return res;
}

Matrix matrix_add(Matrix a, Matrix b)
{
	size_t row, col, i, j;
	Matrix res;

	CHECK_NULL_THEN(a, return NULL;);
	CHECK_NULL_THEN(b, return NULL;);

	row = a->row;
	col = a->col;

	if (row != b->row || col != b->col) {
		EXC_ERR();
		return NULL;
	}

	res = matrix_new(row, col); 	//! Don't forget to free if not used
	CHECK_NULL_THEN(res, return NULL;);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (matrix_set(res, i, j, matrix_get(a, i, j) + matrix_get(b, i, j)) == -1) {
				EXC_ERR();
				matrix_destroy(&res);
				return NULL;
			} 
		}
	}

	return res;
}

Matrix matrix_rand(size_t row, size_t col)
{
	int i, n;
	double* _mtr;
	Matrix mtr = matrix_new(row, col); //! Don't forget to free if not used.
	CHECK_NULL_THEN(mtr, return NULL;);
	
	_mtr = mtr->mtr;

	n = row * col;
	for (i = 0; i < n; i++) {
		_mtr[i] = rand() / (double)RAND_MAX;
	}
	return mtr;
}

double matrix_add_up(Matrix mtr)
{
	int i, n;
	double acc, *_mtr;

	CHECK_NULL_THEN(mtr, return 0;);

	_mtr = mtr->mtr;
	CHECK_NULL_THEN(_mtr, return 0;);

	n = mtr->row * mtr->col;

	acc = 0;
	for (i = 0; i < n; i++) {
		acc += _mtr[i];
	}

	return acc;
}

Matrix matrix_abs(Matrix mtr)
{
	int row, col, i, n;
	double* _mtr, *_res, tmp;
	Matrix res;

	CHECK_NULL_THEN(mtr, return NULL;);

	_mtr = mtr->mtr;
	CHECK_NULL_THEN(_mtr, return NULL;);
	row = mtr->row;
	col = mtr->col;

	res = matrix_new(row, col); //! Don't forget to free if not used.
	CHECK_NULL_THEN(res, return NULL;);

	_res = res->mtr;

	n = row * col;
	for (i = 0; i < n; i++) {
		tmp = _mtr[i];
		_res[i] = (tmp < 0) ? -tmp : tmp;
	}

	return res;
}

Matrix matrix_copy_row(Matrix mtr, size_t idx)
{
	size_t col, i;
	Matrix vec;

	CHECK_NULL_THEN(mtr, return NULL;);
	CHECK_NULL_THEN(mtr->mtr, return NULL;);

	col = mtr->col;
	vec = matrix_new(1, col); 	//! Don't forget to free if not used.

	for (i = 0; i < col; i++) {
		if (matrix_set(vec, 0, i, matrix_get(mtr, idx, i)) == -1) {
			EXC_ERR();
			matrix_destroy(&vec);
			return NULL;
		}
	}

	return vec;
}

Matrix matrix_copy_col(Matrix mtr, size_t idx)
{
	size_t row, i;
	Matrix vec;

	CHECK_NULL_THEN(mtr, return NULL;);
	CHECK_NULL_THEN(mtr->mtr, return NULL;);

	row = mtr->row;
	vec = matrix_new(row, 1); 	//! Don't forget to free if not used.

	for (i = 0; i < row; i++) {
		if (matrix_set(vec, i, 0, matrix_get(mtr, i, idx)) == -1) {
			EXC_ERR();
			matrix_destroy(&vec);
			return NULL;
		}
	}

	return vec;
}