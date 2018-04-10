#include <stdio.h>

struct Fraction {
    int numerator;
    int denominator;
};

struct Matrix {
    struct Fraction **number;
    int width;
    int length;
};

int main(int argc, char* argv[])
{
    FILE* file = fopen(argc == 1 ? stdin : argv[1], "r");

    if (NULL == file) {
        printf("Error opening input file!\nIt can be pass as param or stdin\n");
        return 1;
    }
    
    fclose(file);
    return 0;
}
