#pragma once

#include <stddef.h>

typedef struct NeuralNetworkStruct* NeuralNetwork;

typedef struct NeuralNetworkLayerStruct* NeuralNetworkLayer;

//* INTERFACE DEFINITION ********************************************************

extern const struct NeuralNetworkInterface{
    NeuralNetwork (*create)(size_t inputSize, size_t outputSize,
        NeuralNetworkLayer* hiddenLayers, size_t hiddenLayerCount,
        double (*activationFunction)(double),
        double (*activationDerivative)(double),
        double (*errorFunction)(double, double),
        double (*errorDerivative)(double, double));
    void (*destroy)(NeuralNetwork* nnAddr);
    NeuralNetworkLayer (*layerOf)(size_t inputSize, size_t outputSize,
        double (*activationFunction)(double),
        double (*activationDerivative)(double));
    int (*feedForward)(NeuralNetwork nn, const double* input, double* output);
    // int (*setActivationFunction)(NeuralNetwork nn, double (*activationFunction)(double));
    // int (*setErrorFunction)(NeuralNetwork nn, double (*errorFunction)(double, double));
    // int (*setLearningRate)(NeuralNetwork nn, double learningRate);
    // int (*predict)(NeuralNetwork nn, const double* input, double* output);
    // int (*train)(NeuralNetwork nn, const double* input,
    //  const double* target, double learningRate);

    /**
     * @brief 
     * @note Can't be used by itself, it doesn't calculate the sum of the exponentials.
     */
    double (*softmax)(double);
} NeuralNetworkOps;