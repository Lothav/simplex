//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_MATRIX_HPP
#define SIMPLEX_MATRIX_HPP

#include <vector>
#include <iostream>
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

    void updateCell(int i, int j, Fraction* cell)
    {
        if (i >= this->m_) {
            std::cerr << "Error trying update matrix. Invalid line index " << i << std::endl;
            return;
        }

        if (j >= this->n_) {
            std::cerr << "Error trying update matrix. Invalid column index " << j << std::endl;
            return;
        }

        free(this->cells_[i][j]);
        this->cells_[i][j] = cell;
    }

};


#endif //SIMPLEX_MATRIX_HPP
