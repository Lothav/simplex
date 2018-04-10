#include <stdio.h>

int main(int argc, char* argv[])
{
    FILE* file = NULL;

    if (argc == 1) {
        file = fopen(stdin, "r");
    } else {
        file = fopen(argv[1], "r");
    }


    if (NULL == file) {
        printf("Error opening input file!\n");
        return 1;
    }

    fclose(file);
    return 0;
}
