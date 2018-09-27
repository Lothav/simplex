//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_TABLEAUX_HPP
#define SIMPLEX_TABLEAUX_HPP

#include "Matrix.hpp"
#include "File.hpp"

const std::array<long, 2> EMPTY_INDEXES = {-1, -1};

namespace Simplex {


    class Tableaux
    {

    private:

        Type                    type_;
        SolveMethod             solve_method_;
        std::unique_ptr<Matrix> matrix_;
        SolutionType                solution_;

    public:

        explicit Tableaux(TableauxInput&& tableaux_input);

        void solve(std::string file_output_steps);

        Simplex::TableauxOutput getOutput() const;

    private:

        void convertToStandardForm(const std::vector<bool>& is_non_negative, const std::vector<Operator>& operators);

        void addSlackVariables(const std::vector<Operator>& operators);

        void removeSlackVariables();

        SolveMethod getWhichSolveMethodApplies() const;

        void solveAux(std::string file_output_result);

        bool stepPrimal(const std::string& file_output_steps);

        bool stepDual(const std::string& file_output_steps);

        void stepAux(const std::string& file_output_steps);

        std::array<long, 2> getPrimalIndex() const;

        std::array<long, 2> getDualIndex() const;

        void pivot(const std::array<long, 2>& indexes, std::string file_output_steps);

        std::vector<std::array<long, 2>> getPivotedIndexes() const;

        void checkSolution();

        long double getObjectiveValue() const;

    };

}
#endif //SIMPLEX_TABLEAUX_HPP
