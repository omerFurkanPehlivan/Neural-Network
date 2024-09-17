#pragma once

#include "../include/matrix.h"

typedef struct DatapointStruct* Datapoint;

extern const struct DatapointInterface{
    Datapoint (*create)(const double* input, const double* output, size_t inputSize, size_t outputSize);
    void (*destroy)(Datapoint* datapointAddr);
    const Matrix (*getInput)(Datapoint datapoint);
    const Matrix (*getOutput)(Datapoint datapoint);
} DatapointOps;