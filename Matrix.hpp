//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_MATRIX_HPP
#define SIMPLEX_MATRIX_HPP

#include <vector>
#include "Fraction.hpp"

class Matrix {

private:

    long m_;
    long n_;
    std::vector<std::vector<Fraction*>> cells_;

public:

    Matrix(long m, long n, const std::vector<long>& cells): m_(m+1), n_(n+1)
    {
        int i, j;
        for (i = 0 ; i < m_; i++) {
            cells_.push_back({});
            for (j = 0 ; j < n_; j++) {
                cells_[i].push_back(new Fraction(cells[(i * n_) + j], 1));
            }
        }
    }
};


#endif //SIMPLEX_MATRIX_HPP
