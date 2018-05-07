//
// Created by luiz0tavio on 5/5/18.
//

#include "Matrix.hpp"

Matrix::Matrix(long m, long n, const std::vector<long long> &cells): m_(m), n_(n), in_fpi_(false)
{
    int i, j;
    for (i = 0 ; i < m_; i++) {
        cells_.push_back({});
        for (j = 0 ; j < n_; j++) {
            cells_[i].push_back(new Fraction(cells[(i * n_) + j], 1));
        }
    }
}

void Matrix::putInFPI()
{
    if (this->isInFPI()) {
        return;
    }

    // Insert [[0, 0, 0], [1, 0, 0], [0, 1, 0], [0, 0, 1]] vector like in matrix.
    // It corresponds to slack variables (before 'b' vector).
    // First line we will get our certifies.

    // Iterate lines.
    for (int i = 0; i < this->m_; ++i) {

        // Iterate columns.
        for (int j = 0; j < this->m_-1; ++j) {

            // If line is 0, all columns is 0.
            // If line != 0, if column+1 == line then 1 else 0.
            long slack_var_num = ((i != 0) && (i == j+1)) ? 1 : 0;

            // Generate slack variable fraction.
            auto slack_var_element = new Fraction(slack_var_num, 1);

            // Insert it straight before 'b' element.
            this->cells_[i].insert(this->cells_[i].end() - 1, slack_var_element);
        }
    }

    // We add some columns, so, increment n value as much inserts by line (nested loop above).
    this->n_ += this->m_-1;

    // Mark Matrix is in FPI method.
    this->in_fpi_ = true;
}

void Matrix::updateCell(int i, int j, Fraction* cell)
{
    if (i >= this->m_) {
        std::cerr << "Error trying update matrix. Invalid line index " << i << std::endl;
        return;
    }

    if (j >= this->n_) {
        std::cerr << "Error trying update matrix. Invalid column index " << j << std::endl;
        return;
    }

    this->cells_[i][j] = cell;
}

std::string Matrix::toString() const
{
    std::string matrix_string = "[";
    for (int i = 0; i < this->getM(); ++i) {
        matrix_string += "[";
        for (int j = 0; j < this->getN(); ++j) {
            float value = this->cells_[i][j]->getFloatValue();
            matrix_string += std::to_string(value);
            if(j != this->getN()-1){
                matrix_string += ", ";
            }
        }
        matrix_string += "]";
        if(i != this->getM()-1){
            matrix_string += ", \n";
        }
    }
    matrix_string += "]\n";

    return matrix_string;
}

bool Matrix::isInFPI() const
{
    return this->in_fpi_;
}

long Matrix::getM() const
{
    return m_;
}

long Matrix::getN() const
{
    return n_;
}

std::vector<std::vector<Fraction*>> Matrix::getCells() const
{
    return cells_;
}