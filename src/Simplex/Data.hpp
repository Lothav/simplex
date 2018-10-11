#ifndef SIMPLEX_DATA_HPP
#define SIMPLEX_DATA_HPP

namespace Simplex
{
    enum Type {
        NON_INT
    };

    enum SolveMethod {
        DUAL_METHOD,
        PRIMAL_METHOD,
        PRIMAL_AUX_METHOD
    };

    enum SolutionType {
        NONE,
        VIABLE,
        NON_VIABLE,
        UNBOUNDED
    };

    enum Operator {
        EQUAL          = 0,
        LESS_EQUAL     = 1,
        GREATER_EQUAL  = 2,
    };

    struct TableauxOutput {
        SolutionType             solution_type;
        long double              objective_value;
        std::vector<long double> solution;
        std::vector<long double> certify;
    };

    struct TableauxInput {
        // Tableaux lines
        const long m = 0;

        // Tableaux columns.
        const long n = 0;

        // Type default NON_INT: means that result can be float (non-integer).
        const Type type = Type::NON_INT;

        // Tableaux cells.
        // Must contain objective function and restrictions.
        const std::vector<double> cells = {};

        // Operators vector size must be equal 'm'-1.
        // It's restrictions lines operators.
        const std::vector<Operator> operators = {};

        // Operators vector size must be equal 'n'-1.
        // If empty, default true, for every variable.
        const std::vector<bool> is_non_negative = {};
    };
}

#endif //SIMPLEX_DATA_HPP
