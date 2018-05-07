//
// Created by tracksale on 5/7/18.
//

#include "Fraction.hpp"

Fraction::Fraction(int numerator, int denominator) : numerator_(numerator), denominator_(denominator)
{
    if (denominator == 0) {
        throw "Denominator can't be zero!";
    }

    simplify();
}

Fraction* Fraction::operator *(const Fraction& fraction)
{
    return new Fraction(this->numerator_ * fraction.getNumerator(), this->denominator_ * fraction.getDenominator());
}

// Boolean Operators
bool Fraction::operator <(const Fraction& fraction)
{
    return (this->numerator_ * fraction.getDenominator()) < (this->denominator_ * fraction.getNumerator());
}

bool Fraction::operator >(const Fraction& fraction)
{
    return (this->numerator_ * fraction.getDenominator()) > (this->denominator_ * fraction.getNumerator());
}

bool Fraction::operator <(const long& number)
{
    return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) < number;
}

bool Fraction::operator <=(const long& number)
{
    return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) <= number;
}

bool Fraction::operator >(const long& number)
{
    return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) > number;
}

Fraction* Fraction::operator /(const Fraction& fraction)
{
    if (this->denominator_ == 0 || fraction.getNumerator() == 0) {
        return new Fraction(0, 1);
    }

    return new Fraction(this->numerator_ * fraction.getDenominator(), this->denominator_ * fraction.getNumerator());
}

Fraction* Fraction::operator -(const Fraction& fraction)
{
    return new Fraction((this->numerator_ * fraction.getDenominator()) - (fraction.getNumerator() * this->denominator_), this->denominator_ * fraction.getDenominator());
}

Fraction* Fraction::operator +(const Fraction& fraction)
{
    return new Fraction((this->numerator_ * fraction.getDenominator()) + (fraction.getNumerator() * this->denominator_), this->denominator_ * fraction.getDenominator());
}

int Fraction::getNumerator() const
{
    return numerator_;
}

int Fraction::getDenominator() const
{
    return denominator_;
}

float Fraction::getFloatValue() const
{
    return static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_);
}


int Fraction::gcd(int a, int b) const
{
    return b == 0 ? a : gcd(b, a % b);
}

void Fraction::simplify()
{
    int gcd_ = gcd(this->numerator_, this->denominator_);
    this->denominator_ /= gcd_;
    this->numerator_   /= gcd_;
}
