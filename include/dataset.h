#pragma once

#include "../include/datapoint.h"
#include "../include/list.h"

#include <stddef.h>

typedef List Dataset;

extern const struct ListInterface* DatasetOps = &ListOps;
