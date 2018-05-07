//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_FRACTION_HPP
#define SIMPLEX_FRACTION_HPP

#include <algorithm>

class Fraction
{

private:

    long long numerator_;
    long long denominator_;

public:

    Fraction(long long numerator, long long denominator);

    // Boolean Operators
    bool operator <(const long long& number);

    bool operator <=(const long long& number);

    bool operator >=(const long long& number);

    bool operator >(const long long& number);

    Fraction* operator *(const long long& number);

    bool operator <(const Fraction& fraction);

    bool operator >(const Fraction& fraction);

    Fraction* operator *(const Fraction& fraction);

    Fraction* operator /(const Fraction& fraction);

    Fraction* operator -(const Fraction& fraction);

    Fraction* operator +(const Fraction& fraction);

    long long getNumerator() const;

    long long getDenominator() const;

    float getFloatValue() const;

private:

    long long gcd(long long a, long long b) const;

    void simplify();

};

#endif //SIMPLEX_FRACTION_HPP
