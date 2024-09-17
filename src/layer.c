#include "../include/layer.h"
#include "../lib/macro_error.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Will error function be in the layer or in the neural network?

//* STRUCT DEFINITION *********************************************************

typedef struct LayerStruct {
	size_t inputSize;
	size_t outputSize;
	double (*activationFunction)(double);
	double (*activationDerivative)(double);
	Matrix weights;
} LayerStruct;

//* FUNCTION PROTOTYPES *******************************************************

Layer create(size_t inputSize, size_t outputSize,
	double (*activationFunction)(double),
	double (*activationDerivative)(double));
void destroy(Layer* layerAddr);
const Matrix getWeights(Layer layer);
double (*getActivationFunction(Layer layer))(double);
double (*getActivationDerivative(Layer layer))(double);
size_t getInputSize(Layer layer);
size_t getOutputSize(Layer layer);
Matrix calculateActivationDeriv(Layer layer, const Matrix input);
double numericalDerivative(double (*f)(double), double x);
int updateWeights(Layer layer, const Matrix gradient, double learningRate);
int setWeights(Layer layer, const Matrix weights);
int isValid(Layer layer);
int feedForward(Layer layer, const Matrix input, Matrix output);
Matrix jacobian(Layer layer, const Matrix input);

//* INTERFACE INITIALIZATION **************************************************

const struct LayerInterface LayerOps = {
	.create = create,
	.destroy = destroy,
	.getWeights = getWeights,
	.getActivationFunction = getActivationFunction,
	.getActivationDerivative = getActivationDerivative,
	.getInputSize = getInputSize,
	.getOutputSize = getOutputSize,
	.calculateActivationDeriv = calculateActivationDeriv,
	.updateWeights = updateWeights,
	.setWeights = setWeights,
	.isValid = isValid,
	.feedForward = feedForward,
};

//* FUNCTION DEFINITIONS ******************************************************

Layer create(size_t inputSize, size_t outputSize,
	double (*activationFunction)(double),
	double (*activationDerivative)(double))
{
	Layer layer;
	Matrix weights;

	if (inputSize == 0 || outputSize == 0) {
		PRINT_ERR("Layer size can't be zero!");
		return NULL;
	}

	if (activationFunction == NULL) {
		PRINT_ERR("Activation function can't be NULL!");
		return NULL;
	}

	layer = malloc(sizeof(LayerStruct));
	if (layer == NULL) {
		MAL_ERR();
		return NULL;
	}

	weights = MatrixOps.create(outputSize, inputSize);
	if (weights == NULL) {
		free(layer);
		return NULL;
	}

	layer->inputSize = inputSize;
	layer->outputSize = outputSize;
	layer->activationFunction = activationFunction;
	layer->activationDerivative = activationDerivative;
	layer->weights = weights;

	return layer;
}

void destroy(Layer* layerAddr)
{
	Layer layer;

	if (layerAddr == NULL) {
		return;
	}

	layer = *layerAddr;
	if (layer) {
		MatrixOps.destroy(&layer->weights);
		free(layer);
	}

	*layerAddr = NULL;
}

const Matrix getWeights(Layer layer)
{
	if (layer == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return NULL;
	}

	return layer->weights;
}

double (*getActivationFunction(Layer layer))(double)
{
	if (layer == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return NULL;
	}

	return layer->activationFunction;
}


double (*getActivationDerivative(Layer layer))(double)
{
	// activationDerivative can be NULL defaulting to numericalDerivative
	return layer->activationDerivative;
}

size_t getInputSize(Layer layer)
{
	if (layer == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return 0;
	}

	return layer->inputSize;
}

size_t getOutputSize(Layer layer)
{
	if (layer == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return 0;
	}

	return layer->outputSize;
}

Matrix calculateActivationDeriv(Layer layer, const Matrix input)
{
	size_t i, outputSize;
	int err = 0;
	double wmulxValue;
	double (*activationDerivative)(double), (*activationFunction)(double);
	Matrix WmulX, weights;

	// Validate parameters
	if (!isValid(layer) || !MatrixOps.isValid(input)) {
		PRINT_ERR("Invalid parameters!");
		return NULL;
	}
	
	activationDerivative = layer->activationDerivative;
	outputSize = layer->outputSize;
	weights = layer->weights;

	WmulX = MatrixOps.multiply(weights, input);
	if (WmulX == NULL) {
		PRINT_ERR("Matrix multiplication failed!");
		return NULL;
	}

    if (activationDerivative) {
		if (MatrixOps.applyToAllUnary(WmulX, activationDerivative) == -1) {
			PRINT_ERR("Activation derivative failed!");
			MatrixOps.destroy(&WmulX);
			return NULL;
		}
	}
	else {
		// Numerical derivative
		for (i = 0; i < outputSize; i++) {
			// Apply the numerical derivative to each element
			err = (MatrixOps.get(WmulX, i, 0, &wmulxValue)) ? -1 : err;
			err = (MatrixOps.set(WmulX, i, 0,
				numericalDerivative(activationFunction, wmulxValue))) ? -1: err;
			if (err) {
				PRINT_ERR("Numerical derivative failed!");
				MatrixOps.destroy(&WmulX);
				return NULL;
			}
		}
	}

	return WmulX;
}

double numericalDerivative(double (*f)(double), double x)
{
	const double h = 1e-6;
	return (f(x + h) - f(x - h)) / (2 * h);
}

// Update the weights of the layer. W = W - learningRate * gradient
int updateWeights(Layer layer, const Matrix gradient, double learningRate)
{
	Matrix deltaWeights;

	if (layer == NULL || gradient == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return -1;
	}

	if (!isValid(layer) || !MatrixOps.isValid(gradient)) {
		PRINT_ERR("Invalid parameters!");
		return -1;
	}

	if (MatrixOps.scalarMultiply(gradient, learningRate) == -1) {
		PRINT_ERR("Matrix operation failed!");
		return -1;
	}

	if (MatrixOps.subtract(layer->weights, deltaWeights) == -1) {
		PRINT_ERR("Matrix operation failed!");
		return -1;
	}

	MatrixOps.destroy(&deltaWeights);
	return 0;
}

int setWeights(Layer layer, const Matrix weights)
{
	if (layer == NULL || weights == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return -1;
	}

	if (MatrixOps.getRow(weights) != layer->outputSize ||
		MatrixOps.getCol(weights) != layer->inputSize) {
		PRINT_ERR("Matrix dimensions do not match!");
		return -1;
	}

	return MatrixOps.replace(&layer->weights, weights);
}

int isValid(Layer layer)
{
	if (layer == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return 0;
	}

	if (layer->inputSize == 0 || layer->outputSize == 0) {
		PRINT_ERR("Layer size can't be zero!");
		return 0;
	}

	if (layer->activationFunction == NULL) {
		PRINT_ERR("Activation function can't be NULL!");
		return 0;
	}

	if (!MatrixOps.isValid(layer->weights)) {
		PRINT_ERR("Invalid weights matrix!");
		return 0;
	}

	if (MatrixOps.getCol(layer->weights) != layer->inputSize ||
		MatrixOps.getRow(layer->weights) != layer->outputSize) {
		PRINT_ERR("Matrix dimensions do not match!");
		return 0;
	}

	return 1;
}

/**
 * @brief 
 * 
 * @param layer 
 * @param input 
 * @param output 
 * @return int 
 * @note The output matrix must be created with the correct dimensions 
 *	before calling this function.
 */
int feedForward(Layer layer, const Matrix input, Matrix output)
{
	Matrix result;

	if (layer == NULL || input == NULL || output == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return -1;
	}

	if (!isValid(layer)) {
		PRINT_ERR("Invalid layer!");
		return -1;
	}

	result = MatrixOps.multiply(layer->weights, input);

	if (result == NULL) {
		return -1;
	}

	if (MatrixOps.applyToAllUnary(result, layer->activationFunction) == -1) {
		MatrixOps.destroy(&result);
		return -1;
	}

	if (MatrixOps.assignValues(output, result) == -1) {
		MatrixOps.destroy(&result);
		return -1;
	}

	MatrixOps.destroy(&result);
	return 0;
}

// Jacobian is the derivative of the output with respect to the inputs
// Purpose of the jacobian is to apply the chain rule to calculate the gradient
Matrix jacobian(Layer layer, const Matrix input)
{
	int err = 0;
	double activationDeriv, w, deriv;
	size_t i, j, outputSize, inputSize;
	Matrix jacobian, const weights, activationDerivMatrix;
	double (*activationFunction)(double);
	double (*activationDerivative)(double);
	const double h = 1e-6;

	if (layer == NULL || input == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return NULL;
	}

	if (!isValid(layer)) {
		PRINT_ERR("Invalid layer!");
		return NULL;
	}

	outputSize = layer->outputSize;
	inputSize = layer->inputSize;
	weights = layer->weights;
	activationFunction = layer->activationFunction;
	activationDerivative = layer->activationDerivative;

	activationDerivMatrix = activationDerivativeToWeights(layer, input);

	// Create the jacobian matrix
	jacobian = MatrixOps.create(outputSize, inputSize);
	if (jacobian == NULL) {
		PRINT_ERR("Matrix creation failed!");
		MatrixOps.destroy(&activationDerivMatrix);
		return NULL;
	}

	// Calculate the jacobian matrix by multiplying ith row of weights
	// with the ith derivative of the activation function
	for (i = 0; i < outputSize; i++) {
		err = err || MatrixOps.get(activationDerivMatrix, i, 0, &activationDeriv);
		
		for (j = 0; j < inputSize; j++) {
			
			// Figured out a clever way to handle errors
			// And I'm afraid, it is not very readable
			// But in the end, this is "C" baby!
			err = err || MatrixOps.get(weights, i, j, &w);
			err = err || MatrixOps.set(jacobian, i, j, w * activationDeriv);
		}
	}
	
	if (err) {
		PRINT_ERR("Matrix operation failed!");
		MatrixOps.destroy(&activationDerivMatrix);
		MatrixOps.destroy(&jacobian);
		return NULL;
	}

	return jacobian;
}
