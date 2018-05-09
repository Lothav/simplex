//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_FRACTION_HPP
#define SIMPLEX_FRACTION_HPP

#include <algorithm>
#include "../external/Inifity.hpp"

typedef InfInt BigInt;

namespace Simplex {

    class Fraction
    {

    private:

        BigInt numerator_;
        BigInt denominator_;

    public:

        Fraction(BigInt numerator, BigInt denominator);

        // Boolean Operators
        bool operator <(const BigInt& number);

        bool operator <=(const BigInt& number);

        bool operator >=(const BigInt& number);

        bool operator >(const BigInt& number);

        bool operator ==(const BigInt& number);

        bool operator <(const Fraction& fraction);

        bool operator >(const Fraction& fraction);

        Fraction* operator *(const BigInt& number);

        Fraction* operator *(const Fraction& fraction);

        Fraction* operator /(const Fraction& fraction);

        Fraction* operator -(const Fraction& fraction);

        Fraction* operator +(const Fraction& fraction);

        BigInt getNumerator() const;

        BigInt getDenominator() const;

        long double getFloatValue() const;

    private:

        BigInt gcd(const BigInt& a, const BigInt& b) const;
        BigInt gcm(const BigInt& a, const BigInt& b) const;

        void simplify();

    };

}

#endif //SIMPLEX_FRACTION_HPP
