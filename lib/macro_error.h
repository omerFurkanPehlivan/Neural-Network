#pragma once

#include <stdio.h>

#define PRINT_ERR(MSG) fprintf(stderr, "\033[0;31m" "\nERROR: %s %s:%d\n\n" "\033[0m" , MSG, __FILE__, __LINE__)
#define EXC_ERR() PRINT_ERR("Exception!") //Prints error massage for exceptions.
#define MAL_ERR() PRINT_ERR("Memory allocation error!") // Prints error massage for memory allocation error.

#define CHECK_NULL_THEN(PTR, THEN) do {if (PTR == NULL) {PRINT_ERR("NULL pointer exception! (" #PTR ")"); THEN}  }while(0) //Checks if PTR is a NULL pointer. If then prints error massage and executes the given expressions.
#define CHECK_NULL(PTR) do {CHECK_NULL_THEN(PTR,);}while(0) // 