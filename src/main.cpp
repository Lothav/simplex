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

    if (!Simplex::File::checkFile(file_data)) {
        return EXIT_FAILURE;
    }

    auto type = Type::NON_INT;
    int method_index = 0;

    if (file_data.size() == 4) {
        type = file_data[method_index++] == 0 ? Type::INT_CUTTING_PLANE : Type::INT_CUTTING_PLANE;
    }

    auto matrix_m = std::stoi(file_data[method_index++]);
    auto matrix_n = std::stoi(file_data[method_index++]);
    auto matrix_cells = Simplex::File::GetIntsFromStringFile(file_data[method_index]);

    // Generate matrix from input file.
    auto tableaux = std::make_unique<Simplex::Tableaux>(matrix_m+1, matrix_n+1, matrix_cells, type);
    tableaux->solve(STEP_WRITE_FILE);
    tableaux->writeSolution(SOLUTION_WRITE_FILE);

    const clock_t end_time = std::clock();
    auto time_spent = static_cast<float>( (end_time - begin_time)) / CLOCKS_PER_SEC ;
    std::cout << "Finished. Took " << time_spent << "s" << std::endl;
    std::cout << "\tThe results was written in '"<< SOLUTION_WRITE_FILE << "'." << std::endl;
    std::cout << "\tMatrix steps was written in '" << STEP_WRITE_FILE << "'." << std::endl;

    return EXIT_SUCCESS;
}