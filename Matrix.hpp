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

    void putInFPI()
    {
        // Insert [[0, 0, 0], [1, 0, 0], [0, 1, 0], [0, 0, 1]] vector like.
        // It corresponds to slack variables in matrix (before 'b' vector).
        // First line we will get our certifies.

        // Iterate A lines.
        for (int i = 0; i < this->m_; ++i) {

            // Iterate A columns.
            for (int j = 0; j < this->m_-1; ++j) {

                // If line is 0, all columns is 0.
                // If line != 0, if column+1 == line then 1 else 0.
                long slack_var_numerator = ((i != 0) && (i == j+1)) ? 1 : 0;

                // Generate slack variable fraction.
                auto slack_var_element = new Fraction(slack_var_numerator, 1);

                // Insert it straight before 'b' element.
                this->cells_[i].insert(this->cells_[i].end() - 1, slack_var_element);
            }
        }

        // We add some columns, so, increment n value as much inserts by line (nested loop above).
        this->n_ += this->m_-1;
    }

    bool isInFPI()
    {
        return this->in_fpi_;
    }

};


#endif //SIMPLEX_MATRIX_HPP
