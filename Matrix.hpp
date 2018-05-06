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

    bool in_fpi_;

public:

    Matrix(long m, long n, const std::vector<long>& cells);

    long getM() const;

    long getN() const;

    std::vector<std::vector<Fraction*>> getCells() const;

    void putInFPI();

    bool isInFPI();

};


#endif //SIMPLEX_MATRIX_HPP
