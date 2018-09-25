//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_FRACTION_HPP
#define SIMPLEX_FRACTION_HPP

#include <algorithm>

namespace Simplex
{
    class Fraction
    {

    private:

        long long numerator_;
        long long denominator_;

    public:

        Fraction(long long numerator, long long denominator);

        bool operator <(const long double& number);

        bool operator <=(const long double& number);

        bool operator >=(const long double& number);

        bool operator >(const long double& number);

        bool operator ==(const long double& number);

        bool operator <(const Fraction& fraction);

        bool operator >(const Fraction& fraction);

        Fraction* operator *(const long long& number);

        Fraction* operator *(const Fraction& fraction);

        Fraction* operator /(const Fraction& fraction);

        Fraction* operator -(const Fraction& fraction);

        Fraction* operator +(const Fraction& fraction);

        long long getNumerator() const;

        long long getDenominator() const;

        long double getFloatValue() const;

        static long long gcd(const long long& a, const long long& b);

        static long long gcm(const long long& a, const long long& b);

    private:

        void simplify();

    };

}

#endif //SIMPLEX_FRACTION_HPP
