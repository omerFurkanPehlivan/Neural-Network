#include "../include/neural_network.h"
#include "../include/matrix.h"
#include "../lib/macro_error.h"
#include "../include/layer.h"
#include "../include/dataset.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//* STRUCT DEFINITION *********************************************************

typedef struct NeuralNetworkStruct {
	size_t inputSize;
	size_t outputSize;
	size_t hiddenLayerCount;
	double (*activationFunction)(double);
	double (*activationDerivative)(double);
	double (*errorFunction)(double, double);
	double (*errorDerivative)(double, double);
	Layer* layers;
} NeuralNetworkStruct;

typedef struct NeuralNetworkLayerStruct {
	size_t inputSize;
	size_t outputSize;
	double (*activationFunction)(double);
	double (*activationDerivative)(double);
} NeuralNetworkLayerStruct;

//* FUNCTION PROTOTYPES *******************************************************

NeuralNetwork create(size_t inputSize, size_t outputSize,
	NeuralNetworkLayer* hiddenLayers, size_t hiddenLayerCount,
	double (*activationFunction)(double),
	double (*activationDerivative)(double),
	double (*errorFunction)(double, double),
	double (*errorDerivative)(double, double));
void destroy(NeuralNetwork* nnAddr);
NeuralNetworkLayer layerOf(size_t inputSize, size_t outputSize,
	double (*activationFunction)(double),
	double (*activationDerivative)(double));
int feedForward(NeuralNetwork nn, const double* input, double* output);
double softmax(double x);
Matrix softmaxJacobian(const Matrix exps);
double defaultErrorFunction(double predicted, double target);
double defaultErrorDerivative(double predicted, double target);
double numericalErrorDerivative(double (*f)(double, double),
	double predicted, double target);
Matrix calculateErrorDerivative(NeuralNetwork nn, const Matrix predicted,
	const Matrix target);
double identity(double x);
double identityDerivative(double x);
int gradientDescentStep(NeuralNetwork nn, const Dataset, double learningRate);
int isValid(NeuralNetwork nn);

//* INTERFACE INITIALIZATION **************************************************

const struct NeuralNetworkInterface NeuralNetworkOps = {
	.create = create,
	.destroy = destroy,
	.layerOf = layerOf,
	.feedForward = feedForward,
	.softmax = softmax
};

//* FUNCTION DEFINITIONS ******************************************************

NeuralNetwork create(size_t inputSize, size_t outputSize,
	NeuralNetworkLayer *hiddenLayers, size_t hiddenLayerCount,
	double (*activationFunction)(double),
	double (*activationDerivative)(double),
	double (*errorFunction)(double, double),
	double (*errorDerivative)(double, double))
{
	NeuralNetwork nn;
	size_t i, j;
	Layer* layers, layer;

	// Validate parameters
	if (inputSize == 0 || outputSize == 0 ||
		(hiddenLayers == NULL && hiddenLayerCount != 0) ||
		activationFunction == NULL)
	{
		PRINT_ERR("Invalid neural network parameters!");
		for (i = 0; i < hiddenLayerCount; i++) free(hiddenLayers[i]);
		return NULL;
	}

	// Create neural network
	nn = (NeuralNetwork)malloc(sizeof(NeuralNetworkStruct));
	if (nn == NULL) {
		MAL_ERR();
		for (i = 0; i < hiddenLayerCount; i++) free(hiddenLayers[i]);
		return NULL;
	}

	// Create layers array
	layers = (Layer*)malloc((hiddenLayerCount + 1) * sizeof(Layer));
	if (layers == NULL) {
		free(nn);
		MAL_ERR();
		for (i = 0; i < hiddenLayerCount; i++) free(hiddenLayers[i]);
		return NULL;
	}

	// Set default error functions if not provided
	if (errorFunction == NULL) {
		errorFunction = defaultErrorFunction;
		errorDerivative = defaultErrorDerivative;
	}

	// Set neural network properties
	nn->inputSize = inputSize;
	nn->outputSize = outputSize;
	nn->hiddenLayerCount = hiddenLayerCount;
	nn->activationFunction = activationFunction;
	nn->activationDerivative = activationDerivative;
	nn->errorFunction = errorFunction;
	nn->errorDerivative = errorDerivative;
	nn->layers = layers;

	// Create layers

	// First layer
	// If there are no hidden layers, create a single layer
	if (hiddenLayerCount == 0) {
		layer = LayerOps.create(inputSize, outputSize,
			activationFunction, activationDerivative);
		if (layer == NULL) {
			free(layers);
			free(nn);
			for (i = 0; i < hiddenLayerCount; i++) free(hiddenLayers[i]);
			return NULL;
		}

		layers[0] = layer;
		return nn;
	}

	// Hidden layers
	layer = LayerOps.create(inputSize,
		(hiddenLayerCount == 0) ? outputSize : hiddenLayers[0]->outputSize,
		activationFunction, activationDerivative);
	if (layer == NULL) {
		free(layers);
		free(nn);
		for (i = 0; i < hiddenLayerCount; i++) free(hiddenLayers[i]);
		return NULL;
	}
	layers[0] = layer;
	free(hiddenLayers[0]);

	if (hiddenLayerCount == 0) {
		return nn;
	}

	// Other hidden layers

	for (i = 1; i < hiddenLayerCount; i++) {
		layer = LayerOps.create(hiddenLayers[i - 1]->outputSize,
			hiddenLayers[i]->outputSize, activationFunction,
			activationDerivative);
		if (layer == NULL) {
			// Destroy all previous layers
			for (j = 0; j < i; j++) LayerOps.destroy(&layers[i]);
			for (j = i; j < hiddenLayerCount; j++) free(hiddenLayers[i]);
			free(layers);
			free(nn);
			return NULL;
		}
		
		free(hiddenLayers[i]);
		layers[i] = layer;
	}

	// Output layer

	// If the output layer uses softmax, use identity instead
	// softmax is applied in the feedForward function
	if (activationFunction == softmax) {
		activationFunction = identity;
		activationDerivative = identityDerivative;
	}

	layer = LayerOps.create(
		hiddenLayers[hiddenLayerCount - 1]->outputSize, outputSize,
		activationFunction, activationDerivative);
	free(hiddenLayers[hiddenLayerCount - 1]);
	if (layer == NULL) {
		// Destroy all layers
		for (i = 0; i < hiddenLayerCount; i++) LayerOps.destroy(&layers[i]);
		free(layers);
		free(nn);
		return NULL;
	}
	layers[hiddenLayerCount] = layer;

	return nn;
}

void destroy(NeuralNetwork* nnAddr)
{
	NeuralNetwork nn;
	size_t i;

	if (nnAddr == NULL) {
		return;
	}

	nn = *nnAddr;
	if (nn) {
		for (i = 0; i < nn->hiddenLayerCount + 1; i++) {
			LayerOps.destroy(&nn->layers[i]);
		}
		free(nn->layers);
		free(nn);
	}

	*nnAddr = NULL;
}

NeuralNetworkLayer layerOf(size_t inputSize, size_t outputSize,
	double (*activationFunction)(double),
	double (*activationDerivative)(double))
{
    NeuralNetworkLayer nnlayer;
	nnlayer = (NeuralNetworkLayer)malloc(sizeof(NeuralNetworkLayerStruct));

	if (nnlayer == NULL) {
		MAL_ERR();
		return NULL;
	}

	nnlayer->inputSize = inputSize;
	nnlayer->outputSize = outputSize;
	nnlayer->activationFunction = activationFunction;
	nnlayer->activationDerivative = activationDerivative;

	return nnlayer;
}

int feedForward(NeuralNetwork nn, const double* input, double* output)
{
	size_t i, outputSize;
	double sum;
	Matrix inputMatrix, outputMatrix;

	if (nn == NULL || input == NULL || output == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return -1;
	}

	// Create input matrix
	inputMatrix = MatrixOps.create(nn->inputSize, 1);
	if (inputMatrix == NULL) {
		return -1;
	}

	// Copy input to input matrix
	for (i = 0; i < nn->inputSize; i++) {
		if (MatrixOps.set(inputMatrix, i, 0, input[i]) == -1) {
			MatrixOps.destroy(&inputMatrix);
			return -1;
		}
	}

	// Feed forward
	for (i = 0; i < nn->hiddenLayerCount + 1; i++) {
		if (LayerOps.getOutputSize(nn->layers[i], &outputSize) == -1) {
			MatrixOps.destroy(&inputMatrix);
			return -1;
		}

		outputMatrix = MatrixOps.create(outputSize, 1);
		if (outputMatrix == NULL) {
			MatrixOps.destroy(&inputMatrix);
			return -1;
		}

		if (LayerOps.feedForward(nn->layers[i], inputMatrix,
			outputMatrix) == -1)
		{
			MatrixOps.destroy(&inputMatrix);
			MatrixOps.destroy(&outputMatrix);
			return -1;
		}

		MatrixOps.destroy(&inputMatrix);
		inputMatrix = outputMatrix;
	}
		inputMatrix = NULL;

	// Apply softmax if the output layer uses softmax
	if (nn->activationFunction == softmax) {
		if (MatrixOps.applyToAllUnary(outputMatrix, softmax) == -1) {
			MatrixOps.destroy(&outputMatrix);
			return -1;
		}

		if (MatrixOps.sum(outputMatrix, &sum) == -1) {
			MatrixOps.destroy(&outputMatrix);
			return -1;
		}

		if (MatrixOps.applyToAllBinary(outputMatrix, MatrixOps.fBinary.mul,
			1.0 / sum) == -1)
		{
			MatrixOps.destroy(&outputMatrix);
			return -1;
		}
	}

	// Copy output to output array
	for (i = 0; i < nn->outputSize; i++) {
		if (MatrixOps.get(outputMatrix, i, 0, &output[i]) == -1) {
			MatrixOps.destroy(&outputMatrix);
			return -1;
		}
	}

	MatrixOps.destroy(&outputMatrix);
	return 0;
}

double softmax(double x)
{
    return exp(x);
}

Matrix softmaxJacobian(const Matrix exps)
{
	size_t i, n;
	double value, exp;
    Matrix expsT, result;

	// Validation
	if (exps == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return NULL;
	}

	if (MatrixOps.isValid(exps) == 0) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	// Transpose the vector
	expsT = MatrixOps.transpose(exps);
	if (expsT == NULL) {
		PRINT_ERR("Matrix transpose failed!");
		return NULL;
	}

	// Multiply the vector by -1
	if (MatrixOps.scalarMultiply(expsT, -1) == -1) {
		PRINT_ERR("Matrix scalar multiplication failed!");
		MatrixOps.destroy(&expsT);
		return NULL;
	}

	// Create the jacobian matrix by multiplying the vector by its transpose
	result = MatrixOps.multiply(exps, expsT);
	if (result == NULL) {
		PRINT_ERR("Matrix multiplication failed!");
		MatrixOps.destroy(&expsT);
		return NULL;
	}
	MatrixOps.destroy(&expsT);

	// Add exponentials to the diagonal
	n = MatrixOps.getRow(exps);
	for (i = 0; i < n; i++) {
		if (MatrixOps.get(result, i, i, &value) == -1) {
			PRINT_ERR("Matrix get failed!");
			MatrixOps.destroy(&result);
			return NULL;
		}

		if (MatrixOps.get(exps, i, 0, &exp) == -1) {
			PRINT_ERR("Matrix get failed!");
			MatrixOps.destroy(&result);
			return NULL;
		}

		if (MatrixOps.set(result, i, i, value + exp) == -1) {
			PRINT_ERR("Matrix set failed!");
			MatrixOps.destroy(&result);
			return NULL;
		}
	}

	return result;
}

double defaultErrorFunction(double predicted, double target)
{
	double diff = predicted - target;
	return diff * diff;
}

double defaultErrorDerivative(double predicted, double target)
{
	return 2 * (predicted - target);
}

double numericalErrorDerivative(double (*f)(double, double), double predicted,
	double target)
{
    const double h = 1e-6;
	return (f(predicted + h, target) - f(predicted - h, target)) / (2 * h);
}

Matrix calculateErrorDerivative(NeuralNetwork nn, const Matrix predicted,
	const Matrix target)
{
	size_t i;
	double predicted_value, target_value;
	Matrix errorDerivative;
	int err = 0;

	// Validate parameters
	if (nn == NULL || predicted == NULL || target == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return NULL;
	}

	if (!MatrixOps.isValid(predicted) || !MatrixOps.isValid(target)) {
		PRINT_ERR("Invalid matrix!");
		return NULL;
	}

	if (MatrixOps.isSameShape(predicted, target) == 0) {
		PRINT_ERR("Matrix dimensions do not match!");
		return NULL;
	}

	// Calculate error derivative
	
	// If an error derivative function is provided, use it
	if (nn->errorDerivative) {
		errorDerivative = MatrixOps.outOfPlace.elementWise(predicted,
			target, nn->errorDerivative);
		if (errorDerivative == NULL) {
			PRINT_ERR("Matrix element-wise operation failed!");
			return NULL;
		}
		return errorDerivative;
	}

	// Otherwise, use numerical differentiation
	errorDerivative = MatrixOps.create(MatrixOps.getRow(predicted), 1);
	if (errorDerivative == NULL) {
		PRINT_ERR("Matrix creation failed!");
		return NULL;
	}

	for (i = 0; i < MatrixOps.getRow(predicted) && !err; i++) {
		
		err = err || MatrixOps.get(predicted, i, 0, &predicted_value);
		err = err || MatrixOps.get(target, i, 0, &target_value);
		err = err || MatrixOps.set(errorDerivative, i, 0,
			numericalErrorDerivative(nn->errorDerivative,
				predicted_value, target_value));
	}
	if (err) {
		PRINT_ERR("Matrix operation failed!");
		MatrixOps.destroy(&errorDerivative);
		return NULL;
	}

	return errorDerivative;
}

double identity(double x)
{
    return x;
}

double identityDerivative(double x)
{
	return 1;
}

int gradientDescentStep(NeuralNetwork nn, const Dataset dataset,
	double learningRate)
{
	int err = 0, isSoftmax = 0;
    size_t i, j, k, layerCount, inputSize, outputSize, dataSize;
	double targetValue, predictedValue, deriv, inputValue;
	Node node;
	Matrix input, * gradients, gradient, * outputs, jacobian, tmpMtr,
		errorDerivative, currentDeriv, weights;
	Layer layer, * layers;
	Datapoint datapoint_buf;

	// Validate parameters
	if (nn == NULL || dataset == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return -1;
	}

	if (!isValid(nn)) {
		PRINT_ERR("Invalid neural network!");
		return -1;
	}

	if (learningRate <= 0) {
		PRINT_ERR("Invalid learning rate!");
		return -1;
	}

	node = DatasetOps->head(dataset);
	if (node == NULL) {
		PRINT_ERR("Empty dataset!");
		return -1;
	}

	if (nn->activationFunction == softmax) {
		isSoftmax = 1;
	}
	
	layerCount = nn->hiddenLayerCount + 1;
	layers = nn->layers;

	// Allocate memory for gradients and outputs
	// Gradients are the derivatives of the error with respect to the weights
	gradients = (Matrix*)malloc(layerCount * sizeof(Matrix));
	outputs = (Matrix*)malloc((isSoftmax ? layerCount + 1 : layerCount)
		* sizeof(Matrix));

	if (gradients == NULL || outputs == NULL) {
		MAL_ERR();
		free(gradients);
		free(outputs);
		return -1;
	}
	for (i = 0; i < layerCount; i++) gradients[i] = NULL;
	for (i = 0; i < layerCount; i++) outputs[i] = NULL;
	if (isSoftmax) outputs[layerCount] = NULL;

	// Initialize output matrices
	for (i = 0; i < layerCount && !err; i++) {
		outputs[i] = MatrixOps.create(LayerOps.getOutputSize(layers[i]), 1);
		if (outputs[i] == NULL) {
			err = -1;
		}
	}
	if (isSoftmax) {
		outputs[layerCount] = MatrixOps.create(nn->outputSize, 1);
		if (outputs[layerCount] == NULL) {
			err = -1;
		}
	}

	// For each datapoint in the dataset
	dataSize = 0;
	if (!err) do {
		// Get input and target
		NodeOps.get(node, datapoint_buf);

		// Feed forward
		err = err || feedForward(nn, DatapointOps.getInput(datapoint_buf), outputs);

		// Calculate error derivative
		err = err || (errorDerivative = calculateErrorDerivative(
				nn,
				outputs[(isSoftmax) ? layerCount : layerCount - 1],
				DatapointOps.getOutput(datapoint_buf))) == NULL;
	
		// For each layer starting from the output layer
		for (i = layerCount; i-- > 0 && err == 0;) {

			// I thought I understood why we shouldn't use break... but I don't
			// Look here and look at the rest of the code
            if (!LayerOps.isValid(layer = layers[i]) ||
				!MatrixOps.isValid(weights = LayerOps.getWeights(layer))) {
                err = -1;
                break;
            }
			outputSize = LayerOps.getOutputSize(layer);
			inputSize = LayerOps.getInputSize(layer);
			
			input = (i == 0) ? DatapointOps.getInput(datapoint_buf)
				: outputs[i - 1];
			
			// Calculate the gradient of the error with respect to the weights
			err = err || (tmpMtr = LayerOps.calculateActivationDeriv(
				layer, outputs[i])) == NULL;
			err = err || (currentDeriv = MatrixOps.multiply(
				errorDerivative, tmpMtr)) == NULL;
			MatrixOps.destroy(&tmpMtr);
			
			err = err || (gradient = MatrixOps.create(
				outputSize, inputSize)) == NULL;

			// Set the gradient values
			for (j = 0; j < outputSize; j++) {
				for (k = 0; k < inputSize; k++) {
					err = err || MatrixOps.get(currentDeriv, j, k, &deriv);
					err = err || MatrixOps.get(input, k, 0, &inputValue);
					err = err || MatrixOps.set(gradient, j, k, deriv * inputValue);
				}
			}

			if (gradients[i] == NULL) {
				gradients[i] = gradient;
			}
			else {
				err = err || MatrixOps.add(gradients[i], gradient);
				MatrixOps.destroy(&gradient);
			}

			// Get jacobian of output layer with respect to the
			// input of current layer to apply the chain rule
			err = err || (jacobian = LayerOps.jacobian(layer, input)) == NULL;

			// Update the error derivative with the jacobian
			tmpMtr = errorDerivative;
			err = err || (errorDerivative = 
				MatrixOps.multiply(errorDerivative, jacobian)) == NULL;
			MatrixOps.destroy(&tmpMtr);
			MatrixOps.destroy(&jacobian);
		}

		// Update data size
		dataSize++;
	}
	while(NodeOps.next(node) && err == 0);

	// Average the gradients
	for (i = 0; i < layerCount && !err; i++) {
		err = err || MatrixOps.scalarMultiply(gradients[i], 1.0 / dataSize);
	}

	// Update the weights
	for (i = 0; i < layerCount && !err; i++) {
		err = err || LayerOps.updateWeights(layers[i], gradients[i], learningRate);
	}
	
	// Release resources
	for (i = 0; i < layerCount; i++) {
		MatrixOps.destroy(&gradients[i]);
		MatrixOps.destroy(&outputs[i]);
	}
	if (isSoftmax) MatrixOps.destroy(&outputs[layerCount]);
	free(gradients);
	free(outputs);
	
	if (err) {
		PRINT_ERR("Error in gradient descent step!");
		return -1;
	}

	return 0;
}

/**
 * @brief 
 * 
 * @param nn 
 * @param input 
 * @param outputs 
 * @return int
 * @note The outputs array must be created with the correct dimensions.
 * The size of the outputs array must be equal to the number of layers
 * in the neural network (+1 if activation function is softmax).
 */
int feedForward(NeuralNetwork nn, Matrix input, Matrix* outputs) {
	size_t i, outputSize, layerCount;
	double sum;
	Layer layer;
	Matrix layerInput;

	// Validate parameters
	if (nn == NULL || input == NULL || outputs == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return -1;
	}

	if (!isValid(nn)) {
		PRINT_ERR("Invalid neural network or input matrix!");
		return -1;
	}

	// Feed forward
	layerInput = input;
	for (i = 0; i < layerCount; i++) {
		layer = nn->layers[i];
		if (layer == NULL) {
			PRINT_ERR("Invalid layer!");
			return -1;
		}

		if (LayerOps.feedForward(layer, layerInput, outputs[i]) == -1) {
			PRINT_ERR("Feed forward failed!");
			return -1;
		}

		// Loop update
		layerInput = outputs[i];
	}

	// Apply softmax if the output layer uses softmax
	if (nn->activationFunction == softmax) {
		outputs[layerCount] = MatrixOps.outOfPlace
			.applyToAllUnary(outputs[layerCount - 1], softmax);
		if (outputs[layerCount] == NULL) {
			PRINT_ERR("Softmax failed!");
			return -1;
		}

		if (MatrixOps.sum(outputs[layerCount], &sum) == -1) {
			PRINT_ERR("Matrix sum failed!");
			return -1;
		}

		if (MatrixOps.scalarMultiply(outputs[layerCount], 1.0 / sum) == -1) {
			PRINT_ERR("Matrix scalar multiplication failed!");
			return -1;
		}
	}

	return 0;
}

int isValid(NeuralNetwork nn)
{
	size_t i, layerCount, inputSize, prevOutputSize;
	Layer layer;

	if (nn == NULL) {
		return 0;
	}

	layerCount = nn->hiddenLayerCount + 1;

	// initial prevOutputSize is the input size
	prevOutputSize = nn->inputSize;
	for (i = 0; i < layerCount; i++) {
		// Validate layer
		layer = nn->layers[i];
		if (layer == NULL || !LayerOps.isValid(layer)) {
			return 0;
		}

		// Check if the input size of the layer matches
		// the output size of the previous layer
		inputSize = LayerOps.getInputSize(layer);

		if (inputSize != prevOutputSize) {
			return 0;
		}

		// Update for the next iteration
		prevOutputSize = LayerOps.getOutputSize(layer);
	}

	// Check if the output size of the last layer matches
	// the output size of the neural network
	if (prevOutputSize != nn->outputSize) {
		return 0;
	}

	return 1;
}
