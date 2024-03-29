#include "Tableaux.hpp"

Simplex::Tableaux::Tableaux(TableauxInput&& tableaux_input) : solve_method_(SolveMethod::PRIMAL_METHOD), solution_(SolutionType::NONE), type_(tableaux_input.type), count_slack(0)
{
    matrix_ = std::make_unique<Matrix>(tableaux_input.m, tableaux_input.n, tableaux_input.cells);

    this->convertToStandardForm(tableaux_input.is_non_negative, tableaux_input.operators);

    // Iterate in first matrix column. Set all elements to negative.
    for (int j = 0; j < matrix_->getN(); ++j) {
        auto element = matrix_->getCells()[0][j];
        matrix_->updateCell(0, j, new Fraction(element->getNumerator() * -1, element->getDenominator()));
    }
}

void Simplex::Tableaux::convertToStandardForm(const std::vector<bool>& is_non_negative, const std::vector<Operator>& operators)
{
    auto matrix_cells = this->matrix_->getCells();

    // Add an negative variable for each variable that doesn't have a non-negative restriction.
    for (int k = 0; k < is_non_negative.size(); ++k) {
        if (!is_non_negative[k]) {
            std::vector<Fraction*> column = {new Fraction(0, 1)};
            for (int i = 1; i < this->matrix_->getM(); ++i) {
                column.push_back(new Fraction(matrix_cells[i][k]->getNumerator() * -1, matrix_cells[i][k]->getDenominator()));
            }
            this->matrix_->addColumn(this->matrix_->getN()-1, column);
        }
    }

    this->addSlackVariables(operators);
}

void Simplex::Tableaux::addSlackVariables(const std::vector<Operator>& operators)
{
    // It corresponds to slack variables (before 'b' vector).
    // First line we will get our certifies.

    // Iterate columns that must be add.
    for (int j = 0; j < this->matrix_->getM()-1; ++j) {

        // Equal operator doesn't need a slack variable.
        if (!operators.empty() && operators[j] == Operator::EQUAL) {
            continue;
        }

        std::vector<Fraction*> column = {};

        // Iterate lines.
        for (int i = 0; i < this->matrix_->getM(); ++i) {

            // If line is 0, all columns are 0.
            // If line != 0, if column+1 == line then 1 else 0.
            long slack_var_num = 0;
            if ((i != 0) && (i == j + 1)) {
                slack_var_num = 1;
                // Multiply all line by -1 so we have an canonical base.
                if (!operators.empty() && operators[j] == Operator::GREATER_EQUAL) {
                    for (int k = 0; k < this->matrix_->getN(); ++k) {
                        this->matrix_->updateCell(i, k, *matrix_->getCells()[i][k] * -1);
                    }
                }
            }

            // Generate slack variable fraction.
            auto slack_var_element = new Fraction(slack_var_num, 1);

            // Fill column vector.
            column.push_back(slack_var_element);
        }

        // Insert it straight before 'b' element.
        this->matrix_->addColumn(this->matrix_->getN()-1, column);
        count_slack++;
    }
}

void Simplex::Tableaux::removeSlackVariables()
{
    for (int j = 0; j < count_slack; ++j) {
        this->matrix_->removeColumn(this->matrix_->getN()-2);
    }

    count_slack = 0;
}

void Simplex::Tableaux::stepAux(const std::string& file_output_steps)
{
    this->addSlackVariables({});

    for (int k = 0; k < this->matrix_->getN()-1; ++k) {
        this->matrix_->updateCell(0, k, new Fraction(0, 1));
    }

    for (long k = this->matrix_->getN()-2; k > this->matrix_->getN()-1-this->matrix_->getM(); --k) {
        this->matrix_->updateCell(0, static_cast<int>(k), new Fraction(1, 1));
    }

    File::WriteOnFile(file_output_steps, "Build Aux Matrix:\n");
    this->matrix_->printMatrixOnFile(file_output_steps);
    File::WriteOnFile(file_output_steps, "Put in Canonical Form:\n");
    for (int i = 1; i < this->matrix_->getM(); ++i) {
        for (long k = this->matrix_->getN()-2; k > this->matrix_->getN()-1-this->matrix_->getM(); --k) {
            auto matrix_cells = this->matrix_->getCells();
            if (*matrix_cells[i][k] == 1 || *matrix_cells[i][k] == -1) {
                this->pivot({i, static_cast<int>(k)}, file_output_steps);
            }
        }
    }

    File::WriteOnFile(file_output_steps, "Pivot Aux with Primal Algorithm:\n");
    while (stepPrimal(file_output_steps));

    auto pivoted_indexes = getPivotedIndexes();

    // Check if slack variable is on the solution base.
    for(auto &pivoted_index: pivoted_indexes) {
        if(pivoted_index[1] > this->matrix_->getN()-this->matrix_->getM()-2) {
            this->solution_ = NON_VIABLE;
            return;
        }
    }

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

    // Get updated Matrix
    auto matrix_cells = this->matrix_->getCells();

    // Save the 'c' vector as backup.
    std::vector<Fraction*> backup_c_vec = {};
    for (int i = 0; i < this->matrix_->getN(); ++i) {
        backup_c_vec.push_back(new Fraction(matrix_cells[0][i]->getNumerator(), matrix_cells[0][i]->getDenominator()));
    }

    // Run aux matrix code.
    this->stepAux(file_output_steps);

    // Check if objective value equal zero.
    auto objective_value = getObjectiveValue();
    if (objective_value == 0 && this->solution_ == NONE) {

        File::WriteOnFile(file_output_steps, "SolutionType found on Aux Matrix:\n");
        this->matrix_->printMatrixOnFile(file_output_steps);

        // Get pivoted indexes.
        auto indexes = this->getPivotedIndexes();

        // Recover 'c' vector from backup.
        for (int i = 0; i < this->matrix_->getN(); ++i) {
            this->matrix_->updateCell(0, i, backup_c_vec[i]);
        }
        File::WriteOnFile(file_output_steps, "Recover original 'c' vector. Using Aux base:\n");
        this->matrix_->printMatrixOnFile(file_output_steps);

        // Pivot found indexes.
        File::WriteOnFile(file_output_steps, "Pivot original with Aux base:\n");
        for (auto index_ : indexes) {
            this->pivot(index_, file_output_steps);
        }

        return;
    }

    this->solution_ = SolutionType::NON_VIABLE;
}

long double Simplex::Tableaux::getObjectiveValue() const
{
    return this->matrix_->getCells()[0][this->matrix_->getN()-1]->getFloatValue();
}

std::vector<std::array<long, 2>> Simplex::Tableaux::getPivotedIndexes() const
{
    std::vector<std::array<long,2>> indexes = {};
    std::array<long, 2> index;

    auto matrix_cells = this->matrix_->getCells();

    for (int j = 0; j < this->matrix_->getN()-1-count_slack; ++j) {
        int count_zeros = 0;
        index = EMPTY_INDEXES;
        for (int i = 0; i < this->matrix_->getM(); ++i) {
            if (*matrix_cells[i][j] == 1 && index == EMPTY_INDEXES) {
                index = {i, j};
            } else if (*matrix_cells[i][j] == 0) {
                count_zeros++;
            }
        }
        if (count_zeros == this->matrix_->getM()-1 && index != EMPTY_INDEXES) {
            indexes.push_back(index);
        }
    }

    return indexes;
};

Simplex::SolveMethod Simplex::Tableaux::getWhichSolveMethodApplies() const
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

    File::WriteOnFile(file_output_steps,"Original Matrix:\n");
    File::WriteOnFile(file_output_steps, this->matrix_->toString());

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

    if (this->solution_ == SolutionType::NONE) {
        this->checkSolution();
    }
}

void Simplex::Tableaux::checkSolution()
{
    if (this->solution_ != SolutionType::NONE) {
        std::cerr << "Calling checkSolution() but solution already set!" << std::endl;
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
                this->solution_ = SolutionType::UNBOUNDED;
                return;
            }
        }
    }

    // Check 'c' vector < 0.
    for (int i = 0; i < this->matrix_->getN()-1; ++i) {
        if (*matrix_cells[0][i] < 0) {
            this->solution_ = SolutionType::NON_VIABLE;
            return;
        }
    }

    // Check 'b' vector < 0.
    for (int i = 1; i < this->matrix_->getM(); ++i) {
        if (*matrix_cells[i][this->matrix_->getN()-1] < 0) {
            this->solution_ = SolutionType::NON_VIABLE;
            return;
        }
    }

    // SolutionType is viable.
    this->solution_ = SolutionType::VIABLE;
}

bool Simplex::Tableaux::stepPrimal(const std::string& file_output_steps)
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

bool Simplex::Tableaux::stepDual(const std::string& file_output_steps)
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

            Fraction lower(INT32_MAX, 1);

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
                if (b_divided_by_A < lower) {
                    index = {j, i};
                    lower = b_divided_by_A;
                }
            }

            if(index != EMPTY_INDEXES) {
                return index;
            }
        }
    }

    return EMPTY_INDEXES;
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

Simplex::TableauxOutput Simplex::Tableaux::getOutput() const
{
    TableauxOutput output = {};

    output.solution_type = this->solution_;

    auto matrix_cells = this->matrix_->getCells();

    // Found a optimal solution.
    if (this->solution_ == SolutionType::VIABLE) {

        std::vector<long double> solution = {};
        auto indexes = this->getPivotedIndexes();

        for (int l = 0; l < this->matrix_->getN()-1-count_slack; ++l) {
            bool found = false;
            for(auto index: indexes) {
                if (index[1] == l) {
                    // Push 'b' element to solution vector.
                    solution.push_back(matrix_cells[index[0]][this->matrix_->getN()-1]->getFloatValue());
                    found = true;
                    break;
                }
            }
            if (!found) {
                solution.push_back(0.0f);
            }
        }

        output.solution = solution;
        output.objective_value = matrix_cells[0][this->matrix_->getN()-1]->getFloatValue();

        return output;
    }

    // Non-viable tableaux.
    if (this->solution_ == SolutionType::NON_VIABLE) {
        return output;
    }

    // Unlimited tableaux.
    if (this->solution_ == SolutionType::UNBOUNDED) {
        return output;
    }

    std::cerr << "SolutionType not set! There's something wrong." << std::endl;
    return output;
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

    this->matrix_->printMatrixOnFile(file_output_steps);
}
