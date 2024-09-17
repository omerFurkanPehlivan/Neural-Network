#include "../include/datapoint.h"

#include <stdlib.h>

//* STRUCT DEFINITION *********************************************************

typedef struct DatapointStruct {
	Matrix input;
	Matrix output;
} DatapointStruct;

//* FUNCTION PROTOTYPES *******************************************************

Datapoint create(Matrix input, Matrix output);
void destroy(Datapoint* datapointAddr);
const Matrix getInput(Datapoint datapoint);
const Matrix getOutput(Datapoint datapoint);

//* INTERFACE INITIALIZATION **************************************************

const struct DatapointInterface DatapointOps = {
	.create = create,
	.destroy = destroy,
	.getInput = getInput,
	.getOutput = getOutput
};

//* FUNCTION DEFINITIONS ******************************************************

Datapoint create(Matrix input, Matrix output)
{
	Datapoint datapoint;

	if (!isValid(input) || !isValid(output)) {
		PRINT_ERR("Invalid input or output matrix!");
		return NULL;
	}

	datapoint = malloc(sizeof(DatapointStruct));
	if (datapoint == NULL) {
		MAL_ERR();
		return NULL;
	}

	datapoint->input = input;
	datapoint->output = output;

	return datapoint;
}

void destroy(Datapoint* datapointAddr)
{
	Datapoint datapoint;

	if (datapointAddr == NULL) {
		return;
	}

	datapoint = *datapointAddr;
	if (datapoint) {
		MatrixOps.destroy(&datapoint->input);
		MatrixOps.destroy(&datapoint->output);
		free(datapoint);
	}

	*datapointAddr = NULL;
}

const Matrix getInput(Datapoint datapoint)
{
	if (datapoint == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return NULL;
	}

	return datapoint->input;
}

const Matrix getOutput(Datapoint datapoint)
{
	if (datapoint == NULL) {
		PRINT_ERR("NULL pointer exception!");
		return NULL;
	}

	return datapoint->output;
}
