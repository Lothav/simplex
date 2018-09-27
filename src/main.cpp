//
// Created by luiz0tavio on 5/5/18.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <regex>
#include <climits>
#include "Simplex/Tableaux.hpp"

#define SOLUTION_WRITE_FILE "conclusao.txt"
#define STEP_WRITE_FILE "pivoteamento.txt"

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
            Simplex::File::GetTableauxInputFromFile(argv[1])
#endif
        );
        tableaux->solve(STEP_WRITE_FILE);
        auto solution = tableaux->getOutput();
        Simplex::File::WriteSolution(SOLUTION_WRITE_FILE, solution);

    }
    const clock_t end_time = std::clock();
    auto time_spent = static_cast<float>( end_time - begin_time) / CLOCKS_PER_SEC;
    std::cout << "Finished. Took " << time_spent << "s." << std::endl;
    std::cout << "\tThe results was written in '" << SOLUTION_WRITE_FILE << "'." << std::endl;
    std::cout << "\tMatrix steps was written in '" << STEP_WRITE_FILE << "'." << std::endl;

    return EXIT_SUCCESS;
}