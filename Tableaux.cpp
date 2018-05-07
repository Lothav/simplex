//
// Created by luiz0tavio on 5/5/18.
//

#include <climits>
#include "Tableaux.hpp"

Tableaux::Tableaux(long m, long n, const std::vector<long> &cells) : solve_method_(SolveMethod::PRIMAL_METHOD)
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

void Tableaux::solve(std::string file_output_steps, std::string file_output_result)
{
    this->solve_method_ = this->getWhichSolveMethodApplies();

    if (this->solve_method_ == SolveMethod::PRIMAL_METHOD) {
        while (stepPrimal(file_output_steps, file_output_result));
        return;
    }

    if (this->solve_method_ == SolveMethod::DUAL_METHOD) {
        while(stepDual(file_output_steps, file_output_result));
        return;
    }
}

bool Tableaux::stepPrimal(std::string file_output_steps, std::string file_output_result)
{
    // Try to get a Element index to Pivot.
    auto primal_indexes = this->getPrimalMatrixIndex();
    if (primal_indexes[0] == -1 || primal_indexes[1] == -1) {
        // Primal finished.
        return false;
    }

    // Pivot primal element.
    this->pivot(primal_indexes);

    // Write matrix step on file.
    std::string matrix_str = this->matrix_->toString();
    File::WriteOnFile(file_output_steps, matrix_str);

    return true;
}

bool Tableaux::stepDual(std::string file_output_steps, std::string file_output_result)
{
    // Try to get a Element index to Pivot.
    auto dual_indexes = this->getDualMatrixIndex();
    if (dual_indexes[0] == -1 || dual_indexes[1] == -1) {
        // Dual finished.
        return false;
    }

    // Multiply line to pivot to -1.
    for (int i = 0; i < this->matrix_->getN(); ++i) {
        auto matrix_cell = this->matrix_->getCells()[i][dual_indexes[1]];
        this->matrix_->updateCell(i, dual_indexes[1], *matrix_cell * -1);
    }

    // Pivot dual element.
    this->pivot(dual_indexes);

    // Write matrix step on file.
    std::string matrix_str = this->matrix_->toString();
    File::WriteOnFile(file_output_steps, matrix_str);

    return true;
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

            Fraction lower (INT_MAX, 1);

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

std::array<int, 2> Tableaux::getDualMatrixIndex() const
{
    std::array<int, 2> index = {-1, -1};

    // Iterate in 'b' vector elements.
    for (int i = 1; i < matrix_->getM(); ++i) {

        // Get 'b' vector element.
        auto b_element = *matrix_->getCells()[i][matrix_->getN()-1];

        // Check if 'b' is negative.
        if (b_element < 0) {

            Fraction lower (INT_MAX, 1);

            // Iterate in 'c' vector elements
            for (int j = 0; j < matrix_->getN()-1; ++j) {

                // Get correspondent positive A element.
                Fraction A_element = *matrix_->getCells()[i][j];
                if (A_element <= 0) {
                    continue;
                }

                // Get 'c' vector element.
                Fraction c_element = *matrix_->getCells()[0][j];

                // Find lower 'b' element divided by 'A' element.
                if (lower > *(c_element/A_element)) {
                    lower = c_element;
                    index = {i, j};
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
