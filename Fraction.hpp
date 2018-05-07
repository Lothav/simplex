//
// Created by luiz0tavio on 5/5/18.
//

#ifndef SIMPLEX_FRACTION_HPP
#define SIMPLEX_FRACTION_HPP

#include <algorithm>

class Fraction
{

private:

    long numerator_;
    long denominator_;

public:

    Fraction(long numerator, long denominator) : numerator_(numerator), denominator_(denominator)
    {
        simplify();
    }

    Fraction* operator *(const Fraction& fraction)
    {
        return new Fraction(this->numerator_ * fraction.getNumerator(), this->denominator_ * fraction.getDenominator());
    }

    // Boolean Operators
    bool operator <(const Fraction& fraction)
    {
        return this->numerator_ * fraction.getDenominator() < this->denominator_ * fraction.getNumerator();
    }

    bool operator >(const Fraction& fraction)
    {
        return this->numerator_ * fraction.getDenominator() > this->denominator_ * fraction.getNumerator();
    }

    bool operator <(const long& number)
    {
        return (this->numerator_ / this->denominator_) < number;
    }

    bool operator <=(const long& number)
    {
        return (this->numerator_ / this->denominator_) <= number;
    }

    bool operator >(const long& number)
    {
        return (this->numerator_ / this->denominator_) > number;
    }

    Fraction* operator /(const Fraction& fraction)
    {
        return new Fraction(this->numerator_ * fraction.getDenominator(), this->denominator_ * fraction.getNumerator());
    }

    Fraction* operator -(const Fraction& fraction)
    {
        return new Fraction((this->numerator_ * fraction.getDenominator()) - (fraction.getNumerator() * this->denominator_), this->denominator_ * fraction.getDenominator());
    }

    long getNumerator() const
    {
        return numerator_;
    }

    long getDenominator() const
    {
        return denominator_;
    }

private:

    long mmc(long a, long b) {

    }

    long gcd(long a, long b) {
        return b == 0 ? a : gcd(b, a % b);
    }


    void simplify()
    {
        long gcd_ = gcd(this->numerator_, this->denominator_);
        this->denominator_ /= gcd_;
        this->numerator_   /= gcd_;
    }

};

#endif //SIMPLEX_FRACTION_HPP
