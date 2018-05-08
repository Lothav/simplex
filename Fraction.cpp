//
// Created by tracksale on 5/7/18.
//

#include "Fraction.hpp"

Fraction::Fraction(long long numerator, long long denominator) : numerator_(numerator), denominator_(denominator)
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

bool Fraction::operator <(const long long& number)
{
    return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) < number;
}

bool Fraction::operator <=(const long long& number)
{
    return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) <= number;
}

bool Fraction::operator >=(const long long& number)
{
    return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) >= number;
}

bool Fraction::operator ==(const long long& number)
{
    return (static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_)) == number;

}

bool Fraction::operator >(const long long& number)
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

Fraction* Fraction::operator *(const long long& number)
{
    return new Fraction(this->numerator_ * number, this->denominator_);
}

long long Fraction::getNumerator() const
{
    return numerator_;
}

long long Fraction::getDenominator() const
{
    return denominator_;
}

float Fraction::getFloatValue() const
{
    return static_cast<float>(this->numerator_) / static_cast<float>(this->denominator_);
}


long long Fraction::gcd(long long a, long long b) const
{
    return b == 0 ? a : gcd(b, a % b);
}

void Fraction::simplify()
{
    long long gcd_ = gcd(this->numerator_, this->denominator_);
    this->denominator_ /= gcd_;
    this->numerator_   /= gcd_;
}
