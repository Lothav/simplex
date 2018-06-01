//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_MATRIX_HPP
#define SIMPLEX_MATRIX_HPP

#include "Fraction.hpp"
#include <vector>
#include <iostream>

namespace Simplex
{
    class Matrix
    {

    private:

        long m_;
        long n_;
        std::vector<std::vector<Fraction*>> cells_;

    public:

        Matrix(long m, long n, const std::vector<BigInt>& cells);

        long getM() const;

        long getN() const;

        std::vector<std::vector<Fraction*>> getCells() const;

        void updateCell(long i, long j, Fraction* cell);

        void addColumn(long pos, std::vector<Fraction*> column);

        void addLine(long pos, std::vector<Fraction*> line);

        void removeColumn(long column);

        std::string toString() const;

    };
}

#endif //SIMPLEX_MATRIX_HPP
