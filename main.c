#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <regex.h>
#include <string.h>

struct Fraction {
    long numerator;
    long denominator;
};

struct Matrix {
    struct Fraction **cells;
    long int lines;
    long int columns;
};

char** regexRow(char* src, unsigned int row_size)
{
    int res, len;
    char *result = malloc(BUFSIZ),
         **return_value = malloc(row_size * sizeof(char *)),
         err_buf[BUFSIZ],
         *aux = src;

    const char* pattern = "\\[((\\-*[0-9]+[\\,\\ ]*)+)\\]";

    regex_t preg;
    regmatch_t pmatch[10];

    if( (res = regcomp(&preg, pattern, REG_EXTENDED)) != 0)
    {
        regerror(res, &preg, err_buf, BUFSIZ);
        printf("regcomp() error: %s\n", err_buf);
        exit(res);
    }

    long offset = 0;
    int i;
    for(i = 0; i < row_size; i++)
    {
        aux += offset;
        res = regexec(&preg, aux, 10, pmatch, REG_NOTBOL);
        if(res == REG_NOMATCH) break;

        offset = pmatch[0].rm_eo - pmatch[0].rm_so;

        len = pmatch[1].rm_eo - pmatch[1].rm_so;
        memcpy(result, aux + pmatch[1].rm_so, (size_t)len);
        result[len] = '\0';

        return_value[i] = malloc((size_t)len + 1);
        memcpy(return_value, result, (size_t)len);
        printf("%s\n", result);
    }

    regfree(&preg);
    free(result);

    return return_value;
}

void setRow(struct Fraction *row, char *line, unsigned int size)
{
    while (size)
    {
        if ((*line == '-' && isdigit(*(line+1))) || isdigit(*line)) {
            long val = strtol(line, &line, 10);
            row->numerator = val;
            row->denominator = 1;
            size--;
            printf("%ld\n", val);
        } else {
            line++;
        }
    }
}

void buildMatrixFromFile(FILE* input)
{
    struct Matrix* matrix = malloc(sizeof(struct Matrix));

    char aux[20], *pEnd;

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

    char *rows = regexRow(line);

    int i = 0;
    for (i = 0; i < matrix->lines; i++)
    {
        matrix->cells[i] = malloc(matrix->columns * sizeof(struct Fraction));
        setRow(matrix->cells[i], line, matrix->columns);
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
