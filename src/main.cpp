#include "Simplex/Tableaux.hpp"

#define STEPS_WRITE_FILE "steps.txt"
using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cerr << "Wrong number of parameters!" << endl;
        return EXIT_FAILURE;
    }

    string input_file  = argv[1];
    string output_file = argv[2];

    const clock_t begin_time = std::clock();
    // Tableaux Unique_Ptr scope.
    {
        // Generate matrix from input file.
        auto tableaux_input = Simplex::File::GetTableauxInputFromFile(move(input_file));
        auto tableaux = make_unique<Simplex::Tableaux>(move(tableaux_input));
        tableaux->solve(STEPS_WRITE_FILE);

        auto solution = tableaux->getOutput();
        Simplex::File::WriteSolution(output_file, solution);
    }
    const clock_t end_time = clock();
    auto time_spent = static_cast<float>(end_time - begin_time) / CLOCKS_PER_SEC;
    cout << "Finished. Took " << time_spent << "s." << std::endl;
    cout << "\tThe results was written in '" << output_file << "'." << endl;
    cout << "\tMatrix steps was written in '" << STEPS_WRITE_FILE << "'." << endl;

    return EXIT_SUCCESS;
}
