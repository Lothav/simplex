
#include <malloc.h>
#include "tableaux.h"

struct Tableaux* buildTableaux(struct Matrix *matrix)
{
    unsigned long column_size = (matrix->columns-1);
    unsigned long line_size = (matrix->lines-1);

    struct Tableaux *tableaux;
    tableaux = malloc(sizeof(struct Tableaux));
    tableaux->y = malloc(line_size * sizeof(struct Fraction));
    tableaux->c = malloc(column_size * sizeof(struct Fraction));
    tableaux->b = malloc(line_size * sizeof(struct Fraction));
    tableaux->v = malloc(line_size * sizeof(struct Fraction));

    int i, j;
    for (j = 0; j < column_size; ++j) {
        initFraction(&tableaux->y[j]);
        tableaux->c [j] = matrix->cells[0][j];
    }

    for (i = 0; i < matrix->lines; ++i) {
        tableaux = malloc(matrix->lines * sizeof(struct Tableaux*));
        for (j = 0; j < matrix->columns; ++j) {

        }
    }
}
