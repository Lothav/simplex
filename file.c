
#include "file.h"
#include "util.h"

struct Matrix* buildMatrixFromFile(FILE* input)
{
    struct Matrix* matrix = malloc(sizeof(struct Matrix));

    char aux[NUMBER_MAX_DIGITS], *pEnd;

    /* Build Lines */
    fgets(aux, NUMBER_MAX_DIGITS, input);
    matrix->lines = (unsigned long) strtol(aux, &pEnd, 10);
    matrix->lines++;

    /* Build Columns */
    fgets(aux, NUMBER_MAX_DIGITS, input);
    matrix->columns = (unsigned long) strtol(aux, &pEnd, 10);
    matrix->columns++;

    /* Build Cells */
    long int line_size = matrix->lines * matrix->columns * 4;
    char line[line_size];

    matrix->cells = malloc(matrix->lines * sizeof(struct Fraction *));

    fgets(line, (int)line_size, input);

    char **rows = regexRow(line, matrix->lines, "\\[((\\-*[0-9]+[\\,\\ ]*)+)\\]");

    int i = 0, index = 0;
    for (i = 0; i < matrix->lines; i++)
    {
        matrix->cells[i] = malloc(matrix->columns * sizeof(struct Fraction));
        index = 0;
        while (index < matrix->columns)
        {
            if ((*rows[i] == '-' && isdigit(*(rows[i]+1))) || isdigit(*rows[i])) {
                long val = strtol(rows[i], &rows[i], 10);
                matrix->cells[i][index].numerator = val;
                matrix->cells[i][index].denominator = 1;
                index++;
            } else {
                rows[i]++;
            }
        }
    }

    return matrix;
}