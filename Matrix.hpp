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

    Matrix(long m, long n, const std::vector<long>& cells);

    long getM() const
    {
        return m_;
    }

    long getN() const
    {
        return n_;
    }

    std::vector<std::vector<Fraction*>> getCells() const
    {
        return cells_;
    }

};


#endif //SIMPLEX_MATRIX_HPP
