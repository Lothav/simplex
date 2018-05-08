//
// Created by luiz0tavio on 5/5/18.
//

#include <climits>
#include "Tableaux.hpp"

Tableaux::Tableaux(long long m, long long n, const std::vector<long long> &cells) : solve_method_(SolveMethod::PRIMAL_METHOD)
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
    bool has_b_negative = false;
    for (int i = 1; i < this->matrix_->getM(); i++) {
        auto b_element = *this->matrix_->getCells()[i][this->matrix_->getN() - 1];
        if (b_element < 0) {
            has_b_negative = true;
            break;
        }
    }

    bool has_c_negative = false;
    for (int j = 0; j < this->matrix_->getN()-1; j++) {
        auto c_element = *this->matrix_->getCells()[0][j];
        if (c_element < 0) {
            has_c_negative = true;
            break;
        }
    }

    if (has_b_negative && !has_c_negative) {
        return DUAL_METHOD;
    } else if (!has_b_negative && has_c_negative) {
        return PRIMAL_METHOD;
    }

    // otherwise, default primal method.
    return PRIMAL_AUX_METHOD;
}

void Tableaux::solve(std::string file_output_steps, std::string file_output_result)
{
    this->solve_method_ = this->getWhichSolveMethodApplies();

    if (this->solve_method_ == SolveMethod::PRIMAL_METHOD) {
        std::cout << "Using Primal method." << std::endl;
        while (stepPrimal(file_output_steps, file_output_result));
        return;
    }

    if (this->solve_method_ == SolveMethod::DUAL_METHOD) {
        std::cout << "Using Dual method." << std::endl;
        while(stepDual(file_output_steps, file_output_result));
        return;
    }

    if (this->solve_method_ == SolveMethod::PRIMAL_AUX_METHOD) {
        std::cout << "Using Aux Primal method." << std::endl;

        // Multiply lines where 'b' < 0 to -1.
        for (int i = 1; i < this->matrix_->getM(); ++i) {
            auto matrix_cells = this->matrix_->getCells();
            if (*matrix_cells[i][this->matrix_->getN()-1] < 0) {
                for (int j = 0; j < this->matrix_->getN(); ++j) {
                    this->matrix_->updateCell(i, j, *matrix_cells[i][j] * -1);
                }
            }
        }

        for (int k = 0; k < this->matrix_->getN()-1; ++k) {
            this->matrix_->updateCell(0, k, new Fraction(0, 1));
        }
        for (long k = this->matrix_->getN()-2; k > this->matrix_->getN()-this->matrix_->getM()-2; --k) {
            this->matrix_->updateCell(0, k, new Fraction(1, 1));
        }

        this->matrix_->putInFPI();

        while (stepPrimal(file_output_steps, file_output_result));
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
        auto matrix_cell = this->matrix_->getCells()[dual_indexes[0]][i];
        this->matrix_->updateCell(dual_indexes[0], i, *matrix_cell * -1);
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
                if (b_element <= 0) {
                    continue;
                }

                auto b_divided_by_A = *(b_element/A_element);

                // Find lower 'b' element divided by 'A' element.
                if (index[0] == -1 || lower > b_divided_by_A) {
                    lower = b_divided_by_A;
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
                if (A_element >= 0) {
                    continue;
                }

                // Get 'c' vector element.
                Fraction* c_element = matrix_->getCells()[0][j];
                if (c_element <= 0) {
                    continue;
                }

                auto c_divided_by_A = *(*c_element / *(A_element*(-1)));

                // Find lower 'b' element divided by 'A' element.
                if (index[0] == -1 || lower > c_divided_by_A) {
                    lower = c_divided_by_A;
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
