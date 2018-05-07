//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_TABLEAUX_HPP
#define SIMPLEX_TABLEAUX_HPP

#include <array>
#include "Matrix.hpp"

enum SolveMethod {
    DUAL_METHOD,
    PRIMAL_METHOD
};

class Tableaux
{

private:

    Matrix* matrix_;
    SolveMethod solve_method_;

public:

    Tableaux(long m, long n, const std::vector<long> &cells) : solve_method_(SolveMethod::PRIMAL_METHOD)
    {
        matrix_ = new Matrix(m, n, cells);

        if (!matrix_->isInFPI()) {
            matrix_->putInFPI();
        }

        // Iterate in first matrix column. Set all elements to negative.
        for (int j = 0; j < matrix_->getN(); ++j) {
            auto element = matrix_->getCells()[0][j];
            matrix_->updateCell(0, j, new Fraction(element->getNumerator() * -1, element->getDenominator()));
        }
    }

    void solve()
    {
        this->solve_method_ = this->getWhichSolveMethodApplies();

        if (this->solve_method_ == SolveMethod::PRIMAL_METHOD) {
            auto primal_indexes = getPrimalMatrixIndex();
            this->pivot(primal_indexes);
            return;
        }

        if (this->solve_method_ == SolveMethod::DUAL_METHOD) {

            return;
        }
    }

private:

    SolveMethod getWhichSolveMethodApplies() const;

    std::array<int, 2> getPrimalMatrixIndex() const;

    void pivot(const std::array<int, 2>& indexes);

};


#endif //SIMPLEX_TABLEAUX_HPP
