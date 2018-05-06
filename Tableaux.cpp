//
// Created by luiz0tavio on 5/5/18.
//

#include "Tableaux.hpp"


SolveMethod Tableaux::getWhichSolveMethodApplies() const
{
    // check 'c' elements. If less than zero, primal applies (with aux matrix or not).
    for (int j = 0; j < this->matrix_->getN(); j++) {
        if (*this->matrix_->getCells()[0][j] < 0) {
            return PRIMAL_METHOD;
        }
    }

    // if 'c' elements are greater than zero we have an 'b' element negative, dual applies.
    for (int i = 0; i < this->matrix_->getM(); i++) {
        if (*this->matrix_->getCells()[i][this->matrix_->getN() - 1] < 0) {
            return DUAL_METHOD;
        }
    }

    // otherwise, default primal method.
    return PRIMAL_METHOD;
}