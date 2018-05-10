//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_TABLEAUX_HPP
#define SIMPLEX_TABLEAUX_HPP

#include <array>
#include "Matrix.hpp"
#include "File.hpp"

enum SolveMethod {
    DUAL_METHOD,
    PRIMAL_METHOD,
    PRIMAL_AUX_METHOD
};

const std::array<int, 2> EMPTY_INDEXES = {-1, -1};

namespace Simplex {

    class Tableaux
    {

    private:

        Matrix* matrix_;
        SolveMethod solve_method_;

    public:

        Tableaux(long m, long n, const std::vector<long> &cells);

        void solve(std::string file_output_steps, std::string file_output_result);

    private:

        SolveMethod getWhichSolveMethodApplies() const;

        std::array<int, 2> getPrimalMatrixIndex() const;

        std::array<int, 2> getDualMatrixIndex() const;

        bool stepPrimal(std::string file_output_steps, std::string file_output_result);

        bool stepDual(std::string file_output_steps, std::string file_output_result);

        void pivot(const std::array<int, 2>& indexes);

    };

}
#endif //SIMPLEX_TABLEAUX_HPP
