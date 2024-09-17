#pragma once

#include <stddef.h>
#include "matrix.h"

typedef struct LayerStruct* Layer;

extern const struct LayerInterface{
    Layer (*create)(size_t inputSize, size_t outputSize, 
        double (*activationFunction)(double),
        double (*ActivationDerivative)(double));
    void (*destroy)(Layer* layerAddr);
    const Matrix (*getWeights)(Layer layer);
    double (*(*getActivationFunction)(Layer layer))(double);
    double (*(*getActivationDerivative)(Layer layer))(double);
    size_t (*getInputSize)(Layer layer);
    size_t (*getOutputSize)(Layer layer);
    Matrix (*calculateActivationDeriv)(Layer layer, const Matrix input);
    int (*updateWeights)(Layer layer, const Matrix gradient, double learningRate);
    int (*setWeights)(Layer layer, const Matrix weights);
    int (*isValid)(Layer layer);
    int (*feedForward)(Layer layer, const Matrix input, Matrix output);
    Matrix (*jacobian)(Layer layer, const Matrix input);
} LayerOps;