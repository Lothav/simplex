#ifndef SIMPLEX_TABLEAUX_H
#define SIMPLEX_TABLEAUX_H

#include "matrix.h"

struct Tableaux {
    struct Fraction *c; // Objective Equation
    struct Fraction *b; //
    struct Matrix   *A; //
    struct Matrix   *v; //

    struct Fraction *y; //
    struct Matrix   *I; //
};


struct Tableaux* buildTableaux(struct Matrix*);

#endif //SIMPLEX_TABLEAUX_H
