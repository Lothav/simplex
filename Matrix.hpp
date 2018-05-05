//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_MATRIX_HPP
#define SIMPLEX_MATRIX_HPP


#include <vector>
#include "Fraction.hpp"

class Matrix {

private:

    int m_;
    int n_;
    std::vector<Fraction> cells_;

public:

    Matrix() {

    }
};


#endif //SIMPLEX_MATRIX_HPP
