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
    NON_INT
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

    enum Operator {
        EQUAL           = 0,
        LESS_EQUAL      = 1,
        GREATER_EQUAL  = 2,
    };

    struct TableauxInput {
        // Tableaux lines
        const long m = 0;

        // Tableaux columns.
        const long n = 0;

        // Type default NON_INT: means that result can be float (non-integer).
        const Type type = Type::NON_INT;

        // Tableaux cells.
        // Must contain objective function and restrictions.
        const std::vector<double> cells = {};

        // Operators vector size must be equal 'm'-1.
        // It's restrictions lines operators.
        const std::vector<Operator> operators = {};

        // Operators vector size must be equal 'n'-1.
        // If empty, default true, for every variable.
        const std::vector<bool> is_non_negative = {};
    };

    class Tableaux
    {

    private:

        Type                    type_;
        SolveMethod             solve_method_;
        std::unique_ptr<Matrix> matrix_;
        Solution                solution_;
        std::vector<bool>       is_non_negative_ = {};

    public:

        explicit Tableaux(TableauxInput&& tableaux_input);

        void solve(std::string file_output_steps);

        void writeSolution(std::string file_output_result) const;

    private:

        void convertToStandardForm(const std::vector<bool>& is_non_negative);

        void addSlackVariables(std::vector<Operator> operators);

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

        long double getObjectiveValue() const;

        std::vector<Fraction *> getBVector() const;

        std::vector<Fraction *> getLine(long line_index) const;

        std::array<long, 2> getBFirstFloatIndex() const;

        std::vector<long double> getSolution() const;

    };

}
#endif //SIMPLEX_TABLEAUX_HPP
