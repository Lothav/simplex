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

    Fraction(int numerator, int denominator) : numerator_(numerator), denominator_(denominator)
    {
        if (denominator == 0) {
            throw "Denominator can't be zero!";
        }

        simplify();
    }

    Fraction* operator *(const Fraction& fraction)
    {
        return new Fraction(this->numerator_ * fraction.getNumerator(), this->denominator_ * fraction.getDenominator());
    }

    // Boolean Operators
    bool operator <(const Fraction& fraction)
    {
        return (this->numerator_ * fraction.getDenominator()) < (this->denominator_ * fraction.getNumerator());
    }

    bool operator >(const Fraction& fraction)
    {
        return (this->numerator_ * fraction.getDenominator()) > (this->denominator_ * fraction.getNumerator());
    }

    bool operator <(const long& number)
    {
        return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) < number;
    }

    bool operator <=(const long& number)
    {
        return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) <= number;
    }

    bool operator >(const long& number)
    {
        return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) > number;
    }

    Fraction* operator /(const Fraction& fraction)
    {
        if (this->denominator_ == 0 || fraction.getNumerator() == 0) {
            return new Fraction(0, 1);
        }

        return new Fraction(this->numerator_ * fraction.getDenominator(), this->denominator_ * fraction.getNumerator());
    }

    Fraction* operator -(const Fraction& fraction)
    {
        return new Fraction((this->numerator_ * fraction.getDenominator()) - (fraction.getNumerator() * this->denominator_), this->denominator_ * fraction.getDenominator());
    }

    Fraction* operator +(const Fraction& fraction)
    {
        return new Fraction((this->numerator_ * fraction.getDenominator()) + (fraction.getNumerator() * this->denominator_), this->denominator_ * fraction.getDenominator());
    }

    int getNumerator() const
    {
        return numerator_;
    }

    int getDenominator() const
    {
        return denominator_;
    }

    float getFloatValue() const
    {
        return static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_);
    }

private:

    int gcd(int a, int b) const
    {
        return b == 0 ? a : gcd(b, a % b);
    }

    void simplify()
    {
        int gcd_ = gcd(this->numerator_, this->denominator_);
        this->denominator_ /= gcd_;
        this->numerator_   /= gcd_;
    }

};

#endif //SIMPLEX_FRACTION_HPP
