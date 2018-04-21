#ifndef SIMPLEX_MATRIX_H
#define SIMPLEX_MATRIX_H

struct Fraction {
    long numerator;
    long denominator;
};

struct Matrix {
    struct Fraction **cells;
    unsigned long int lines;
    unsigned long int columns;
};

void initFraction(struct Fraction*);

#endif //SIMPLEX_MATRIX_H
