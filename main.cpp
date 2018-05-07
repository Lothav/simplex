//
// Created by luiz0tavio on 5/5/18.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <regex>
#include "Tableaux.hpp"

int main(int argc, char** argv)
{
    std::vector<std::string> file_data;
    if (argc > 1) {
        file_data = File::GetFileData(argv[1]);
    } else {
        file_data = File::GetStdInData();
    }

    if (file_data.size() != 3) {
        std::cerr << "Invalid file lines!" << std::endl;
        std::cerr << "File must have 3 lines:" << std::endl;
        std::cerr << "\tm+1" << std::endl;
        std::cerr << "\tn+1" << std::endl;
        std::cerr << "\t[[line 0 ], [line 1], ..., [line n]]" << std::endl;
        return EXIT_FAILURE;
    }

    auto matrix_m = std::stoi (file_data[0]);
    auto matrix_n = std::stoi (file_data[1]);
    auto matrix_cells = File::GetIntsFromStringFile(file_data[2]);

    // Generate matrix from input file.
    auto tableaux = new Tableaux(matrix_m+1, matrix_n+1, matrix_cells);
    tableaux->solve("pivoteamento.txt", "conclusao.txt");

    return EXIT_SUCCESS;
}