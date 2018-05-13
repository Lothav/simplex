//
// Created by luiz0tavio on 5/5/18.
//

#include "Tableaux.hpp"

Simplex::Tableaux::Tableaux(long m, long n, const std::vector<long> &cells) : solve_method_(SolveMethod::PRIMAL_METHOD)
{
    std::vector<BigInt> bi;
    for(auto bi_c: cells) {
        BigInt a = bi_c;
        bi.push_back(a);
    }

    matrix_ = new Matrix(m, n, bi);

    this->addSlackVariables();

    // Iterate in first matrix column. Set all elements to negative.
    for (int j = 0; j < matrix_->getN(); ++j) {
        auto element = matrix_->getCells()[0][j];
        matrix_->updateCell(0, j, new Fraction(element->getNumerator() * -1, element->getDenominator()));
    }
}

void Simplex::Tableaux::addSlackVariables()
{
    // Insert [[0, 0, 0], [1, 0, 0], [0, 1, 0], [0, 0, 1]] vector like in matrix.
    // It corresponds to slack variables (before 'b' vector).
    // First line we will get our certifies.

    // Iterate lines.
    for (int i = 0; i < this->matrix_->getM(); ++i) {

        // Iterate columns.
        for (int j = 0; j < this->matrix_->getM()-1; ++j) {

            // If line is 0, all columns is 0.
            // If line != 0, if column+1 == line then 1 else 0.
            long slack_var_num = ((i != 0) && (i == j + 1)) ? 1 : 0;

            // Generate slack variable fraction.
            auto slack_var_element = new Fraction(slack_var_num, 1);

            // Insert it straight before 'b' element.
            this->matrix_->insertCell(i, matrix_->getCells()[i].begin(), slack_var_element);
        }
    }

    // We add some columns, so, increment n value as much inserts by line (nested loop above).
    this->matrix_->setN(this->matrix_->getN() + this->matrix_->getM() - 1);
}

void Simplex::Tableaux::removeSlackVariables()
{
    for (int j = 0; j < this->matrix_->getM()-1; ++j) {
        this->matrix_->removeColumn(this->matrix_->getN()-2);
    }
}

void Simplex::Tableaux::putInPFI()
{
    // Search for lines with negative 'b' element.
    for (int i = 1; i < this->matrix_->getM(); ++i) {
        auto matrix_cells = this->matrix_->getCells();
        if (*matrix_cells[i][this->matrix_->getN()-1] < 0) {
            for (int j = 0; j < this->matrix_->getN(); ++j) {
                this->matrix_->updateCell(i, j, *matrix_cells[i][j] * -1);
            }
        }
    }

    this->addSlackVariables();

    std::vector<Fraction*> save_first_line = {};
    for (int k = 0; k < this->matrix_->getN()-1; ++k) {
        auto matrix_cells = this->matrix_->getCells();
        save_first_line.push_back(matrix_cells[0][k]);
        this->matrix_->updateCell(0, k, new Fraction(0, 1));
    }

    for (long k = this->matrix_->getN()-2; k > this->matrix_->getN()-1-this->matrix_->getM(); --k) {
        this->matrix_->updateCell(0, k, new Fraction(1, 1));
    }

    for (int i = 1; i < this->matrix_->getM(); ++i) {
        for (long k = this->matrix_->getN()-2; k > this->matrix_->getN()-1-this->matrix_->getM(); --k) {
            auto matrix_cells = this->matrix_->getCells();
            if (*matrix_cells[i][k] == 1 || *matrix_cells[i][k] == -1) {
                this->pivot({i, k});
            }
        }
    }

    while (stepPrimal("pivoteamento.txt", ""));

    auto matrix_cells = this->matrix_->getCells();
    if (*matrix_cells[0][this->matrix_->getN()-1] == 0) {
        this->removeSlackVariables();
        for (int i = 0; i < this->matrix_->getN()-1; ++i) {
            this->matrix_->updateCell(0, i, save_first_line[i]);
        }
        std::vector<std::array<int,2>> indexes = {};
        std::array<int,2> index = EMPTY_INDEXES;
        matrix_cells = this->matrix_->getCells();
        for (int j = 0; j < this->matrix_->getN()-1; ++j) {
            int count_one = 0;
            for (int i = 1; i < this->matrix_->getM(); ++i) {
                if (*matrix_cells[i][j] == 1) {
                    index = {i, j};
                    count_one++;
                }
            }
            if (count_one == 1) {
                indexes.push_back(index);
            }
        }
        for (auto index_ : indexes) {
            this->pivot(index_);

            // Write matrix step on file.
            std::string matrix_str = this->matrix_->toString();
            Simplex::File::WriteOnFile("pivoteamento.txt", matrix_str);
        }
    }


}

SolveMethod Simplex::Tableaux::getWhichSolveMethodApplies() const
{
    auto matrix_cells = this->matrix_->getCells();

    // Check if has negative 'b' element.
    bool has_b_negative = false;
    for (int i = 1; i < this->matrix_->getM(); i++) {
        auto b_element = *matrix_cells[i][this->matrix_->getN() - 1];
        if (b_element < 0) {
            has_b_negative = true;
            break;
        }
    }

    // Check if has negative 'c' element.
    bool has_c_negative = false;
    for (int j = 0; j < this->matrix_->getN()-1; j++) {
        auto c_element = *matrix_cells[0][j];
        if (c_element < 0) {
            has_c_negative = true;
            break;
        }
    }

    // If has 'b' negative and doesn't has 'c' negative: Dual method.
    if (has_b_negative && !has_c_negative) {
        return DUAL_METHOD;
    }

    // If doesn't has 'b' negative and has 'c' negative: Primal method.
    if (!has_b_negative && has_c_negative) {
        return PRIMAL_METHOD;
    }

    // Otherwise, use aux matrix and Primal method.
    return PRIMAL_AUX_METHOD;
}

void Simplex::Tableaux::solve(std::string file_output_steps, std::string file_output_result)
{
    std::ofstream ofs;
    ofs.open(file_output_steps, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    ofs.open(file_output_result, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    this->solve_method_ = this->getWhichSolveMethodApplies();

    if (this->solve_method_ == SolveMethod::PRIMAL_METHOD) {
        std::cout << "Using Primal method..." << std::endl;
        while (stepPrimal(file_output_steps, file_output_result));
        return;
    }

    if (this->solve_method_ == SolveMethod::DUAL_METHOD) {
        std::cout << "Using Dual method..." << std::endl;
        while(stepDual(file_output_steps, file_output_result));
        return;
    }

    if (this->solve_method_ == SolveMethod::PRIMAL_AUX_METHOD) {
        std::cout << "Using Aux Primal method..." << std::endl;

        this->putInPFI();

        while (stepPrimal(file_output_steps, file_output_result));
        return;
    }
}

bool Simplex::Tableaux::stepPrimal(std::string file_output_steps, std::string file_output_result)
{
    // Try to get a Element index to Pivot.
    auto primal_indexes = this->getPrimalIndex();
    if (primal_indexes == EMPTY_INDEXES) {
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

bool Simplex::Tableaux::stepDual(std::string file_output_steps, std::string file_output_result)
{
    // Try to get a Element index to Pivot.
    auto dual_indexes = this->getDualIndex();
    if (dual_indexes == EMPTY_INDEXES) {
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
    Simplex::File::WriteOnFile(file_output_steps, matrix_str);

    return true;
}

std::array<int, 2> Simplex::Tableaux::getPrimalIndex() const
{
    std::array<int, 2> index = EMPTY_INDEXES;

    auto matrix_cells = matrix_->getCells();

    // Iterate in 'c' vector elements.
    for (int i = 0; i < matrix_->getN()-1; ++i) {

        // Get 'c' vector element.
        auto c_element = *matrix_cells[0][i];

        // Check if 'c' is negative.
        if (c_element < 0) {

            Fraction lower(0, 1);

            // Iterate in 'b' vector elements
            for (int j = 1; j < matrix_->getM(); ++j) {

                // Get correspondent positive A element.
                Fraction A_element = *matrix_cells[j][i];
                if (A_element <= 0) {
                    continue;
                }

                // Get 'b' vector element.
                Fraction b_element = *matrix_cells[j][matrix_->getN()-1];

                auto b_divided_by_A = *(b_element/A_element);

                // Find lower 'b' element divided by 'A' element.
                if (index == EMPTY_INDEXES || b_divided_by_A < lower) {
                    index = {j, i};
                    lower = b_divided_by_A;
                }
            }

            break;
        }
    }

    return index;
}

std::array<int, 2> Simplex::Tableaux::getDualIndex() const
{
    std::array<int, 2> index = EMPTY_INDEXES;

    auto matrix_cells = matrix_->getCells();

    // Iterate in 'b' vector elements.
    for (int i = 1; i < matrix_->getM(); ++i) {

        // Get 'b' vector element.
        auto b_element = *matrix_cells[i][matrix_->getN()-1];

        Fraction lower(0, 1);

        // Check if 'b' is negative.
        if (b_element < 0) {

            // Iterate in 'c' vector elements
            for (int j = 0; j < matrix_->getN()-1; ++j) {

                // Get correspondent positive A element.
                Fraction A_element = *matrix_cells[i][j];
                if (A_element >= 0) {
                    continue;
                }

                // Get 'c' vector element.
                Fraction* c_element = matrix_cells[0][j];
                if (c_element <= 0) {
                    continue;
                }

                auto c_divided_by_A = *(*c_element / *(A_element*(-1)));

                // Find lower 'b' element divided by 'A' element.
                if (index == EMPTY_INDEXES || c_divided_by_A < lower) {
                    index = {i, j};
                    lower = c_divided_by_A;
                }
            }

            break;
        }
    }

    return index;
}

void Simplex::Tableaux::pivot(const std::array<int, 2>& indexes)
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
        Fraction multiplier = *matrix_cells[i][indexes[1]];
        if (multiplier == 0) {
            continue;
        }

        // Loop through columns elements of 'i' line.
        for (int j = 0; j < this->matrix_->getN(); ++j) {

            auto cell_multiplier = (*matrix_cells[indexes[0]][j] * multiplier);

            // Current cell - (correspondent line pivot column * multiplier)
            auto new_cell = *matrix_cells[i][j] - *cell_multiplier;

            // Update current cell with the new calculated cell.
            this->matrix_->updateCell(i, j, new_cell);

            // Get updated cells.
            matrix_cells = this->matrix_->getCells();
        }
    }
}
