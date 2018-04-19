
#include "file.h"
#include "util.h"

#define POSIX_SUGGEST_SIZE 4096

// Read a matrix from file.
// File must have following format:
//
// m
// n
// [[line 0], [line 1], ..., [line n]]
//
// where m: column size - 1
//       n: line size - 1
//
// File example:
// 2
// 3
// [[1, 2, 3, 0], [1, 1, 1, 2], [0, −1, 3, 9]
struct Matrix* buildMatrixFromFile(FILE* input)
{
    struct Matrix* matrix = malloc(sizeof(struct Matrix));

    char mn[NUMBER_MAX_DIGITS], *pEnd;

    // ============================================================================================
    // Build lines
    fgets(mn, NUMBER_MAX_DIGITS, input);
    matrix->lines = (unsigned long) strtol(mn, &pEnd, 10);
    matrix->lines++;

    // ============================================================================================
    // Build columns
    fgets(mn, NUMBER_MAX_DIGITS, input);
    matrix->columns = (unsigned long) strtol(mn, &pEnd, 10);
    matrix->columns++;

    // ============================================================================================
    // Build Cells
    char line[POSIX_SUGGEST_SIZE];

    matrix->cells = malloc(matrix->lines * sizeof(struct Fraction *));

    fgets(line, POSIX_SUGGEST_SIZE, input);

    char **rows = regexRow(line, matrix->lines, "\\[((\\-*[0-9]+[\\,\\ ]*)+)\\]");

    int i = 0, index = 0;
    for (i = 0; i < matrix->lines; i++)
    {
        matrix->cells[i] = malloc(matrix->columns * sizeof(struct Fraction));
        index = 0;
        while (index < matrix->columns)
        {
            if (isdigit(*rows[i]) || (*rows[i] == '-' && isdigit(*(rows[i]+1)))) {
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