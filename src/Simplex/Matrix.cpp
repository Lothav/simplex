//
// Created by luiz0tavio on 5/5/18.
//

#include "Matrix.hpp"

Simplex::Matrix::Matrix(long m, long n, const std::vector<BigInt> &cells): m_(m), n_(n)
{
    int i, j;
    for (i = 0 ; i < m_; i++) {
        cells_.push_back({});
        for (j = 0 ; j < n_; j++) {
            cells_[i].push_back(new Fraction(cells[(i * n_) + j], 1));
        }
    }
}

void Simplex::Matrix::updateCell(int i, int j, Fraction* cell)
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

void Simplex::Matrix::insertCell(long i, std::vector<Fraction *>::const_iterator j, Fraction* cell)
{
    this->cells_[i].insert(this->cells_[i].end() - 1 , cell);
}

void Simplex::Matrix::addColumn(long pos, std::vector<Fraction*> column)
{
    if (pos > this->getN()-1 || column.size() != this->getM()) {
        return;
    }

    for (int i = 0; i < this->getM(); ++i) {
        this->cells_[i].insert(this->cells_[i].begin() + pos, column[i]);
    }

    this->n_ += 1;
}

void Simplex::Matrix::removeColumn(long column)
{
    for (int k = 0; k < this->getM()-1; ++k) {
        this->cells_[k].erase(this->cells_[k].begin() + column);
    }

    this->n_ -= 1;
}

std::string Simplex::Matrix::toString() const
{
    std::string matrix_string = "[";

    for (int i = 0; i < this->getM(); ++i) {

        matrix_string += "[";

        for (int j = 0; j < this->getN(); ++j) {
            long double value = this->cells_[i][j]->getFloatValue();
            matrix_string += std::to_string(value);
            if (j != this->getN()-1) {
                matrix_string += ", ";
            }
        }

        matrix_string += "]";

        if (i != this->getM()-1) {
            matrix_string += ", \n";
        }
    }

    matrix_string += "]";

    return matrix_string;
}

long Simplex::Matrix::getM() const
{
    return m_;
}

long Simplex::Matrix::getN() const
{
    return n_;
}

void Simplex::Matrix::setM(long m)
{
    this->m_ = m;
}

void Simplex::Matrix::setN(long n)
{
    this->n_ = n;
}

std::vector<std::vector<Simplex::Fraction*>> Simplex::Matrix::getCells() const
{
    return cells_;
}