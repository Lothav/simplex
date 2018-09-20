//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_TABLEAUX_HPP
#define SIMPLEX_TABLEAUX_HPP

#include <array>
#include <cmath>
#include "Matrix.hpp"
#include "File.hpp"

enum Type {
    NON_INT,
    INT_CUTTING_PLANE,
    INT_BRANCH_N_BOUND
};

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

const std::array<long, 2> EMPTY_INDEXES = {-1, -1};

namespace Simplex {

    struct TableauxInput {
        long m;
        long n;
        Type type;
        const std::vector<long> &cells;
    };

    class Tableaux
    {

    private:

        Type        type_;
        SolveMethod solve_method_;
        Matrix*     matrix_;
        Solution    solution_;


    public:

        explicit Tableaux(TableauxInput&& tableaux_input);

        void solve(std::string file_output_steps);

        void writeSolution(std::string file_output_result) const;

    private:

        void addSlackVariables();

        void removeSlackVariables();

        void solveAux(std::string file_output_result);

        SolveMethod getWhichSolveMethodApplies() const;

        std::array<long, 2> getPrimalIndex() const;

        std::array<long, 2> getDualIndex() const;

        bool stepPrimal(std::string file_output_steps);

        bool stepDual(std::string file_output_steps);

        void stepAux(std::string file_output_steps);

        void pivot(const std::array<long, 2>& indexes, std::string file_output_steps);

        std::vector<std::array<long, 2>> getPivotedIndexes() const;

        void checkSolution();

        std::vector<Fraction *> getBVector() const;

        std::vector<Fraction *> getLine(long line_index) const;

        std::array<long, 2> getBFirstFloatIndex() const;

        std::vector<long double> getSolution() const;

    };

}
#endif //SIMPLEX_TABLEAUX_HPP
