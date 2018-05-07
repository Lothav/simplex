//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_FRACTION_HPP
#define SIMPLEX_FRACTION_HPP

#include <algorithm>

class Fraction
{

private:

    int numerator_;
    int denominator_;

public:

    Fraction(int numerator, int denominator);

    // Boolean Operators
    bool operator <(const Fraction& fraction);

    bool operator >(const Fraction& fraction);

    bool operator <(const long& number);

    bool operator <=(const long& number);

    bool operator >(const long& number);

    Fraction* operator *(const Fraction& fraction);

    Fraction* operator /(const Fraction& fraction);

    Fraction* operator -(const Fraction& fraction);

    Fraction* operator +(const Fraction& fraction);

    int getNumerator() const;

    int getDenominator() const;

    float getFloatValue() const;

private:

    int gcd(int a, int b) const;

    void simplify();

};

#endif //SIMPLEX_FRACTION_HPP
