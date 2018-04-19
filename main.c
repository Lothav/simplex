#include <stdio.h>
#include "file.h"
#include "tableaux.h"

int main(int argc, char* argv[])
{
    FILE* file = NULL;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }

    if (NULL == file && NULL == stdin) {
        fprintf(stderr, "Usage: %s file\nor\n%s < file", argv[0], argv[0]);
        printf("Error opening input file!\nIt can be pass as param or stdin\n");
        return 1;
    }

    struct Matrix* matrix = buildMatrixFromFile(NULL == file ? stdin : file);
    struct Tableaux* tableaux = buildTableaux(matrix);
    fclose(file);
    return 0;
}
