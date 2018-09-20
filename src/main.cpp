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

Simplex::TableauxInput interfaceComplete(int argc, char** argv)
{
    std::vector<std::string> file_data;
    if (argc > 1) {
        file_data = Simplex::File::GetFileData(argv[1]);
    } else {
        file_data = Simplex::File::GetStdInData();
    }

    if (!Simplex::File::checkFileComplete(file_data))
        exit(EXIT_FAILURE);


    auto type = Type::NON_INT;
    int method_index = 0;

    if (file_data.size() == 4) {
        type = file_data[method_index++] == "0" ? Type::INT_CUTTING_PLANE : Type::INT_BRANCH_N_BOUND;
    }

    auto matrix_m = std::stoi(file_data[method_index++]);
    auto matrix_n = std::stoi(file_data[method_index++]);
    auto matrix_cells = Simplex::File::GetIntsFromStringFile(file_data[method_index]);

    return Simplex::TableauxInput{
        .m      = matrix_m,
        .n      = matrix_n,
        .type   = type,
        .cells  = matrix_cells,
    };
}

Simplex::TableauxInput interfaceSimple(int argc, char** argv)
{
    std::vector<std::string> file_data = Simplex::File::GetFileData(argv[1]);

    //if (!Simplex::File::checkFileComplete(file_data))
    //   exit(EXIT_FAILURE);

    int method_index = 0;

    auto matrix_m       = std::stoi(file_data[method_index++]);
    auto matrix_n       = std::stoi(file_data[method_index++]);
    auto matrix_cells   = Simplex::File::GetIntsFromStringFile(file_data[method_index]);

    return Simplex::TableauxInput{
        .m      = matrix_m,
        .n      = matrix_n,
        .type   = Type::NON_INT,
        .cells  = matrix_cells,
    };
}


int main(int argc, char** argv)
{
    const clock_t begin_time = std::clock();

    // Tableaux Unique_Ptr scope.
    {
        // Generate matrix from input file.
        auto tableaux = std::make_unique<Simplex::Tableaux>(
#ifdef INTERFACE_COMPLETE
            interfaceComplete(argc, argv)
#else
            interfaceSimple(argc, argv)
#endif
        );
        tableaux->solve(STEP_WRITE_FILE);
        tableaux->writeSolution(SOLUTION_WRITE_FILE);
    }

    const clock_t end_time = std::clock();
    auto time_spent = static_cast<float>( (end_time - begin_time)) / CLOCKS_PER_SEC ;
    std::cout << "Finished. Took " << time_spent << "s" << std::endl;
    std::cout << "\tThe results was written in '"<< SOLUTION_WRITE_FILE << "'." << std::endl;
    std::cout << "\tMatrix steps was written in '" << STEP_WRITE_FILE << "'." << std::endl;

    return EXIT_SUCCESS;
}