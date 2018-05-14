//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_TABLEAUX_HPP
#define SIMPLEX_TABLEAUX_HPP

#include <array>
#include "Matrix.hpp"
#include "File.hpp"

enum SolveMethod {
    DUAL_METHOD,
    PRIMAL_METHOD,
    PRIMAL_AUX_METHOD
};

enum Solution {
    NONE,
    VIABLE,
    NON_VIABLE,
    UNLIMITED
};

const std::array<int, 2> EMPTY_INDEXES = {-1, -1};

namespace Simplex {

    class Tableaux
    {

    private:

        SolveMethod solve_method_;
        Matrix*     matrix_;
        Solution    solution_;

    public:

        Tableaux(long m, long n, const std::vector<long> &cells);

        void solve(std::string file_output_steps);

        void writeSolution(std::string file_output_result) const;

    private:

        void addSlackVariables();

        void removeSlackVariables();

        void putInPFI(std::string file_output_result);

        SolveMethod getWhichSolveMethodApplies() const;

        std::array<int, 2> getPrimalIndex() const;

        std::array<int, 2> getDualIndex() const;

        bool stepPrimal(std::string file_output_steps);

        bool stepDual(std::string file_output_steps);

        void stepAux(std::string file_output_steps);

        void pivot(const std::array<int, 2>& indexes, std::string file_output_steps);

        std::vector<std::array<int, 2>> getPivotedIndexes() const;

        void checkSolution();

        std::vector<long double> getSolution() const;

    };

}
#endif //SIMPLEX_TABLEAUX_HPP
