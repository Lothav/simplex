#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

struct Fraction {
    int numerator;
    int denominator;
};

struct Matrix {
    struct Fraction **cells;
    long int lines;
    long int columns;
};

void buildMatrixFromFile(FILE* input)
{
    struct Matrix* matrix = malloc(sizeof(struct Matrix));

    char aux[20], *pEnd, *pEnd2;

    fgets(aux, 20, input);
    matrix->lines = strtol(aux, &pEnd, 10);
    matrix->lines++;

    fgets(aux, 20, input);
    matrix->columns = strtol(aux, &pEnd, 10);
    matrix->columns++;

    long int line_size = matrix->lines * matrix->columns * 4;
    char line[line_size];

    matrix->cells = malloc(matrix->lines * sizeof(struct Fraction*));

    fgets(line, (int)line_size, input);

    int i = 0;
    long int cell = 0;
    for (i = 0; i < matrix->lines; i++)
    {
        matrix->cells[i] = malloc(matrix->columns * sizeof(struct Fraction));

        char *p = line;
        while (*p)
        {
            if (isdigit(*p)) {
                long val = strtol(p, &p, 10);
                printf("%ld\n", val);
            } else {
                p++;
            }
        }

    }

}

int main(int argc, char* argv[])
{
    FILE* file = NULL;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }

    buildMatrixFromFile(NULL == file ? stdin : file);

    if (NULL == file) {
        fprintf(stderr, "Usage: %s file\nor\n%s < file", argv[0], argv[0]);
        printf("Error opening input file!\nIt can be pass as param or stdin\n");
        return 1;
    }

    fclose(file);
    return 0;
}
