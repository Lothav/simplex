//
// Created by luiz0tavio on 5/5/18.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <regex>
#include "Simplex/Tableaux.hpp"

#define SOLUTION_WRITE_FILE "conclusao.txt"
#define STEP_WRITE_FILE "pivoteamento.txt"

int main(int argc, char** argv)
{
    const clock_t begin_time = std::clock();

    std::vector<std::string> file_data;
    if (argc > 1) {
        file_data = Simplex::File::GetFileData(argv[1]);
    } else {
        file_data = Simplex::File::GetStdInData();
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
    auto matrix_cells = Simplex::File::GetIntsFromStringFile(file_data[2]);

    // Generate matrix from input file.
    auto tableaux = new Simplex::Tableaux(matrix_m+1, matrix_n+1, matrix_cells);
    tableaux->solve(STEP_WRITE_FILE, SOLUTION_WRITE_FILE);

    const clock_t end_time = std::clock();
    auto time_spent = static_cast<float>( (end_time - begin_time)) / CLOCKS_PER_SEC ;
    std::cout << "Finished. Took " << time_spent << "s" << std::endl;
    std::cout << "\tThe results was written in '"<< SOLUTION_WRITE_FILE << "'." << std::endl;
    std::cout << "\tMatrix steps was written in '" << STEP_WRITE_FILE << "'." << std::endl;

    return EXIT_SUCCESS;
}