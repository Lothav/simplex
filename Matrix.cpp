//
// Created by luiz0tavio on 5/5/18.
//

#include "Matrix.hpp"

Matrix::Matrix(long m, long n, const std::vector<long> &cells): m_(m), n_(n)
{
    int i, j;
    for (i = 0 ; i < m_; i++) {
        cells_.push_back({});
        for (j = 0 ; j < n_; j++) {
            cells_[i].push_back(new Fraction(cells[(i * n_) + j], 1));
        }
    }
}