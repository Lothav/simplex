#ifndef SIMPLEX_FILE_H
#define SIMPLEX_FILE_H

#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#include "matrix.h"
#include <bits/types/FILE.h>

#define NUMBER_MAX_DIGITS 20

struct Matrix* buildMatrixFromFile(FILE* input);

#endif //SIMPLEX_FILE_H
