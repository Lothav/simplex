//
// Created by luiz0tavio on 5/5/18.
//

#include "Simplex/Tableaux.hpp"

#define STEPS_WRITE_FILE "steps.txt"

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Wrong number of parameters!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string input_file  = argv[1];
    std::string output_file = argv[2];

    const clock_t begin_time = std::clock();
    // Tableaux Unique_Ptr scope.
    {
        // Generate matrix from input file.
        auto tableaux = std::make_unique<Simplex::Tableaux>(Simplex::File::GetTableauxInputFromFile(std::move(input_file)));
        tableaux->solve(STEPS_WRITE_FILE);

        auto solution = tableaux->getOutput();
        Simplex::File::WriteSolution(output_file, solution);
    }
    const clock_t end_time = std::clock();
    auto time_spent = static_cast<float>( end_time - begin_time) / CLOCKS_PER_SEC;
    std::cout << "Finished. Took " << time_spent << "s." << std::endl;
    std::cout << "\tThe results was written in '" << output_file << "'." << std::endl;
    std::cout << "\tMatrix steps was written in '" << STEPS_WRITE_FILE << "'." << std::endl;

    return EXIT_SUCCESS;
}