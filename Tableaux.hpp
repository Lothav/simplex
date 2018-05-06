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
            return;
        }

        if (this->solve_method_ == SolveMethod::DUAL_METHOD) {

            return;
        }
    }

private:

    SolveMethod getWhichSolveMethodApplies() const;

    std::array<int, 2> getPrimalMatrixIndex() const
    {
        std::array<int, 2> index = {};

        // Iterate in 'c' vector elements.
        for (int i = 0; i < matrix_->getN()-1; ++i) {

            // Get 'c' vector element.
            auto c_element = *matrix_->getCells()[0][i];

            // Check if 'c' is negative.
            if (c_element < 0) {

                Fraction lower (INTMAX_MAX, 1);

                // Iterate in 'b' vector elements
                for (int j = 1; j < matrix_->getM(); ++j) {

                    // Get correspondent positive A element.
                    Fraction A_element = *matrix_->getCells()[j][i];
                    if (A_element <= 0) {
                        continue;
                    }

                    // Get 'b' vector element.
                    Fraction b_element = *matrix_->getCells()[j][matrix_->getN()-1];

                    // Find lower 'b' element divided by 'A' element.
                    if (lower > *(b_element/A_element)) {
                        lower = b_element;
                        index = {j, i};
                    }
                }

                return index;
            }
        }

        return index;
    }


};


#endif //SIMPLEX_TABLEAUX_HPP
