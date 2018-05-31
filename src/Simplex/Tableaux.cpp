//
// Created by luiz0tavio on 5/5/18.
//

#include "Tableaux.hpp"

Simplex::Tableaux::Tableaux(long m, long n, const std::vector<long> &cells, Type type) : solve_method_(SolveMethod::PRIMAL_METHOD), solution_(Solution::NONE), type_(type)
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

    // Iterate columns.
    for (int j = 0; j < this->matrix_->getM()-1; ++j) {

        std::vector<Fraction*> column = {};

        // Iterate lines.
        for (int i = 0; i < this->matrix_->getM(); ++i) {

            // If line is 0, all columns is 0.
            // If line != 0, if column+1 == line then 1 else 0.
            long slack_var_num = ((i != 0) && (i == j + 1)) ? 1 : 0;

            // Generate slack variable fraction.
            auto slack_var_element = new Fraction(slack_var_num, 1);

            // Fill column vector.
            column.push_back(slack_var_element);
        }

        // Insert it straight before 'b' element.
        this->matrix_->addColumn(this->matrix_->getN()-1, column);
    }
}

void Simplex::Tableaux::removeSlackVariables()
{
    for (int j = 0; j < this->matrix_->getM()-1; ++j) {
        this->matrix_->removeColumn(this->matrix_->getN()-2);
    }
}

void Simplex::Tableaux::stepAux(std::string file_output_steps)
{
    this->addSlackVariables();

    for (int k = 0; k < this->matrix_->getN()-1; ++k) {
        this->matrix_->updateCell(0, k, new Fraction(0, 1));
    }

    for (long k = this->matrix_->getN()-2; k > this->matrix_->getN()-1-this->matrix_->getM(); --k) {
        this->matrix_->updateCell(0, static_cast<int>(k), new Fraction(1, 1));
    }

    for (int i = 1; i < this->matrix_->getM(); ++i) {
        for (long k = this->matrix_->getN()-2; k > this->matrix_->getN()-1-this->matrix_->getM(); --k) {
            auto matrix_cells = this->matrix_->getCells();
            if (*matrix_cells[i][k] == 1 || *matrix_cells[i][k] == -1) {
                this->pivot({i, static_cast<int>(k)}, file_output_steps);
            }
        }
    }

    while (stepPrimal(file_output_steps));

    this->removeSlackVariables();
}

void Simplex::Tableaux::solveAux(std::string file_output_steps)
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

    // Save the matrix as backup.
    auto matrix_cells = this->matrix_->getCells();
    std::vector<std::vector<Fraction*>> backup_matrix = {};
    for (int l = 0; l < this->matrix_->getM(); ++l) {
        backup_matrix.push_back({});
        for (int i = 0; i < this->matrix_->getN(); ++i) {
            backup_matrix[l].push_back(new Fraction(matrix_cells[l][i]->getNumerator(), matrix_cells[l][i]->getDenominator()));
        }
    }

    // Run aux matrix code.
    this->stepAux(file_output_steps);

    // Check if objective value equal zero.
    auto objective_value = *this->matrix_->getCells()[0][this->matrix_->getN()-1];
    if (objective_value == 0) {

        // Get pivoted indexes.

        // Recover matrix from backup.
        for (int l = 0; l < this->matrix_->getM(); ++l) {
            for (int i = 0; i < this->matrix_->getN(); ++i) {
                this->matrix_->updateCell(l, i, backup_matrix[l][i]);
            }
        }

        auto indexes = this->getPivotedIndexes();

        // Pivot found indexes.
        for (auto index_ : indexes) {
            this->pivot(index_, file_output_steps);
        }
    } else {
        this->solution_ = Solution::NON_VIABLE;
    }
}

std::vector<std::array<long, 2>> Simplex::Tableaux::getPivotedIndexes() const
{
    std::vector<std::array<long,2>> indexes = {};
    std::array<long, 2> index = EMPTY_INDEXES;

    for (int j = 0; j < this->matrix_->getN()-1; ++j) {
        int count_one = 0;
        for (int i = 1; i < this->matrix_->getM(); ++i) {
            if (*this->matrix_->getCells()[i][j] == 1) {
                index = {i, j};
                count_one++;
            }
        }
        if (count_one == 1) {
            indexes.push_back(index);
        }
    }

    return indexes;
};

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

void Simplex::Tableaux::solve(std::string file_output_steps)
{
    // Clear output file.
    std::ofstream ofs;
    ofs.open(file_output_steps, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    this->solve_method_ = this->getWhichSolveMethodApplies();

    if (this->solve_method_ == SolveMethod::PRIMAL_METHOD) {
        std::cout << "Using Primal method..." << std::endl;
        while (stepPrimal(file_output_steps));
    }

    if (this->solve_method_ == SolveMethod::DUAL_METHOD) {
        std::cout << "Using Dual method..." << std::endl;
        while(stepDual(file_output_steps));
    }

    if (this->solve_method_ == SolveMethod::PRIMAL_AUX_METHOD) {
        std::cout << "Using Aux Primal method..." << std::endl;
        this->solveAux(file_output_steps);
        while(stepPrimal(file_output_steps));
    }

    if (this->solution_ == Solution::NONE) {
        this->checkSolution();
    }

    if (this->type_ == Type::INT_CUTTING_PLANE) {

        if (this->solution_ == Solution::VIABLE) {

            auto float_index = this->getBFirstFloatIndex();

            std::vector<Fraction *> A_line = {};
            for (auto line : this->getALine(float_index[0])) {
                auto numerator = static_cast<long long int>(std::floor(line->getFloatValue()));
                A_line.push_back(new Fraction(numerator, 1));
            }



        }
    }
}

std::vector<Simplex::Fraction *> Simplex::Tableaux::getALine(long line_index) const
{
    auto matrix_cells = this->matrix_->getCells();

    if (line_index > this->matrix_->getM()-1) {
        throw std::invalid_argument("Invalid line_index in getALine() method!");
    }

    std::vector<Fraction *> A_line = {};

    for (int i = 0; i < this->matrix_->getN()-this->matrix_->getM(); ++i) {
        A_line.push_back(matrix_cells[line_index][i]);
    }

    return A_line;
}

void Simplex::Tableaux::checkSolution()
{
    if (this->solution_ != Solution::NONE) {
        std::cerr << "Solution already set!" << std::endl;
        return;
    }

    auto matrix_cells = this->matrix_->getCells();

    // Check for 'A' column < 0.
    for (int i = 0; i < this->matrix_->getN()-1; ++i) {
        bool check_all_negative = true;
        for (int j = 1; j < this->matrix_->getM(); ++j) {
            if (*matrix_cells[j][i] > 0) {
                check_all_negative = false;
                break;
            }
        }
        if (check_all_negative) {
            if (*matrix_cells[0][i] < 0) {
                this->solution_ = Solution::UNLIMITED;
                return;
            }
        }
    }

    // Check 'c' vector < 0.
    for (int i = 0; i < this->matrix_->getN()-1; ++i) {
        if (*matrix_cells[0][i] < 0) {
            this->solution_ = Solution::NON_VIABLE;
            return;
        }
    }

    // Check 'b' vector < 0.
    for (int i = 1; i < this->matrix_->getM(); ++i) {
        if (*matrix_cells[i][this->matrix_->getN()-1] < 0) {
            this->solution_ = Solution::NON_VIABLE;
            return;
        }
    }

    // Solution is viable.
    this->solution_ = Solution::VIABLE;
}

bool Simplex::Tableaux::stepPrimal(std::string file_output_steps)
{
    // Try to get a Element index to Pivot.
    auto primal_indexes = this->getPrimalIndex();
    if (primal_indexes == EMPTY_INDEXES) {
        // Primal finished.
        return false;
    }

    // Pivot primal element.
    this->pivot(primal_indexes, file_output_steps);

    return true;
}

bool Simplex::Tableaux::stepDual(std::string file_output_steps)
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
    this->pivot(dual_indexes, file_output_steps);

    return true;
}

std::array<long, 2> Simplex::Tableaux::getPrimalIndex() const
{
    std::array<long, 2> index = EMPTY_INDEXES;

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

std::array<long, 2> Simplex::Tableaux::getDualIndex() const
{
    std::array<long, 2> index = EMPTY_INDEXES;

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

std::vector<long double> Simplex::Tableaux::getSolution() const
{
    std::vector<long double> solution = {};
    auto indexes = this->getPivotedIndexes();

    auto matrix_cells = this->matrix_->getCells();

    for (int l = 0; l < this->matrix_->getN()-this->matrix_->getM(); ++l) {
        bool found = false;
        for(auto index: indexes) {
            if (index[1] == l) {
                solution.push_back(matrix_cells[index[0]][this->matrix_->getN()-1]->getFloatValue());
                found = true;
                break;
            }
        }
        if (!found) {
            solution.push_back(0.0f);
        }
    }

    return solution;
}

void Simplex::Tableaux::writeSolution(std::string file_output_result) const
{
    if (this->solution_ == Solution::NONE) {
        std::cerr << "Solution not set!" << std::endl;
        return;
    }

    // Clear output file.
    std::ofstream ofs;
    ofs.open(file_output_result, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    auto matrix_cells = this->matrix_->getCells();
    auto objective_value = matrix_cells[0][this->matrix_->getN()-1];

    // Found a optimal solution.
    if (this->solution_ == Solution::VIABLE) {

        auto solution = this->getSolution();

        std::string solution_str = "[";
        for (int k = 0; k < solution.size(); ++k) {
            solution_str += std::to_string(solution[k]) + (k != solution.size()-1 ? ", " : "");
        }
        solution_str += "]";

        File::WriteOnFile(file_output_result, "2");
        File::WriteOnFile(file_output_result, solution_str);
        File::WriteOnFile(file_output_result, std::to_string(objective_value->getFloatValue()));
    }

    // Non-viable tableaux.
    if (this->solution_ == Solution::NON_VIABLE) {
        File::WriteOnFile(file_output_result, "0");
    }

    // Unlimited tableaux.
    if (this->solution_ == Solution::UNLIMITED) {
        File::WriteOnFile(file_output_result, "1");
    }

    std::string certify_str = "[";
    for (long k = this->matrix_->getN()-this->matrix_->getM(); k < this->matrix_->getN()-1; ++k) {
        certify_str += std::to_string(matrix_cells[0][k]->getFloatValue()) + (k != this->matrix_->getN()-2 ? ", " : "");
    }
    certify_str += "]";

    File::WriteOnFile(file_output_result, certify_str);
}

void Simplex::Tableaux::pivot(const std::array<long, 2>& indexes, std::string file_output_steps)
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

    // Pivot finished. Write modifications on 'file_output_steps' file.
    std::string matrix_str = this->matrix_->toString();
    Simplex::File::WriteOnFile(file_output_steps, matrix_str);
}

std::vector<Simplex::Fraction *> Simplex::Tableaux::getBVector() const
{
    std::vector<Fraction *> b_vec;

    long b_column = this->matrix_->getN()-1;
    auto matrix_cells = this->matrix_->getCells();

    for (int i = 1; i < this->matrix_->getM(); ++i) {
        b_vec.push_back(matrix_cells[i][b_column]);
    }

    return b_vec;
}

std::array<long, 2> Simplex::Tableaux::getBFirstFloatIndex() const
{
    auto b_vector = this->getBVector();
    long b_column = this->matrix_->getN()-1;

    for (int i = 0; i < b_vector.size(); i++) {
        if (b_vector[i]->getDenominator() != 1) {
            return {i + 1, b_column};
        }
    }

    return EMPTY_INDEXES;
}