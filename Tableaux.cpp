//
// Created by luiz0tavio on 5/5/18.
//

#include "Tableaux.hpp"

SolveMethod Tableaux::getWhichSolveMethodApplies() const
{
    // check 'c' elements. If less than zero, primal applies (with aux matrix or not).
    for (int j = 0; j < this->matrix_->getN(); j++) {
        if (*this->matrix_->getCells()[0][j] < 0) {
            return PRIMAL_METHOD;
        }
    }

    // if 'c' elements are greater than zero we have an 'b' element negative, dual applies.
    for (int i = 0; i < this->matrix_->getM(); i++) {
        if (*this->matrix_->getCells()[i][this->matrix_->getN() - 1] < 0) {
            return DUAL_METHOD;
        }
    }

    // otherwise, default primal method.
    return PRIMAL_METHOD;
}

std::array<int, 2> Tableaux::getPrimalMatrixIndex() const
{
    std::array<int, 2> index = {-1, -1};

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

void Tableaux::pivot(const std::array<int, 2>& indexes)
{
    auto matrix_cells = this->matrix_->getCells();

    // Get pivot element from indexes.
    auto pivot = matrix_cells[indexes[0]][indexes[1]];

    // Iterate matrix columns in matrix line pivot and divide each element by the pivot.
    for (int i = 0; i < this->matrix_->getN(); ++i) {
        this->matrix_->updateCell(indexes[0], i, *matrix_cells[indexes[0]][i] / *pivot);
    }

    // Get updated cells.
    matrix_cells = this->matrix_->getCells();

    // Loop matrix lines.
    for (int i = 0; i < this->matrix_->getM(); ++i) {

        // Ignore pivot line.
        if (i == indexes[0]) {
            continue;
        }

        // Get the correspondent line pivot.
        auto multiplier = *matrix_cells[i][indexes[1]];

        // Loop through columns elements of 'i' line.
        for (int j = 0; j < this->matrix_->getN(); ++j) {

            // Current cell - (correspondent line pivot column * multiplier)
            auto new_cell = *matrix_cells[i][j] - *(*matrix_cells[indexes[0]][j] * multiplier);

            // Update current cell with the new calculated cell.
            this->matrix_->updateCell(i, j, new_cell);

            // Get updated cells.
            matrix_cells = this->matrix_->getCells();
        }
    }
}
