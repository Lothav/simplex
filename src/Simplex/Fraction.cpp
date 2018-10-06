#include <cmath>
#include <stdexcept>
#include <iostream>
#include <climits>
#include "Fraction.hpp"

Simplex::Fraction::Fraction(long long numerator, long long denominator) : numerator_(numerator), denominator_(denominator)
{
    if (denominator_ == 0) {
        throw std::invalid_argument("Denominator can't be zero!");
    }

    if (denominator_ < 0) {
        denominator_ *= -1;
        numerator_   *= -1;
    }

    this->simplify();
}

Simplex::Fraction* Simplex::Fraction::operator *(const Fraction& fraction)
{
    return new Fraction(this->numerator_ * fraction.getNumerator(), this->denominator_ * fraction.getDenominator());
}

bool Simplex::Fraction::operator <(const Fraction& fraction)
{
    return (this->numerator_ * fraction.getDenominator()) < (this->denominator_ * fraction.getNumerator());
}

bool Simplex::Fraction::operator >(const Fraction& fraction)
{
    return (this->numerator_ * fraction.getDenominator()) > (this->denominator_ * fraction.getNumerator());
}

bool Simplex::Fraction::operator <(const long double& number)
{
    return this->getFloatValue() < number;
}

bool Simplex::Fraction::operator <=(const long double& number)
{
    return this->getFloatValue() <= number;
}

bool Simplex::Fraction::operator >=(const long double& number)
{
    return this->getFloatValue() >= number;
}

bool Simplex::Fraction::operator ==(const long double& number)
{
    return this->getFloatValue() == number;
}

bool Simplex::Fraction::operator >(const long double& number)
{
    return this->getFloatValue() > number;
}

Simplex::Fraction* Simplex::Fraction::operator /(const Fraction& fraction)
{
    if (fraction.getNumerator() == 0) {
        std::cerr << "err: division by zero!" << std::endl;
    }

    return new Simplex::Fraction(this->numerator_ * fraction.getDenominator(), this->denominator_ * fraction.getNumerator());
}

Simplex::Fraction* Simplex::Fraction::operator -(const Fraction& fraction)
{
    return new Simplex::Fraction((this->numerator_ * fraction.getDenominator()) - (fraction.getNumerator() * this->denominator_), this->denominator_ * fraction.getDenominator());
}

Simplex::Fraction* Simplex::Fraction::operator +(const Fraction& fraction)
{
    return new Fraction((this->numerator_ * fraction.getDenominator()) + (fraction.getNumerator() * this->denominator_), this->denominator_ * fraction.getDenominator());
}

Simplex::Fraction* Simplex::Fraction::operator *(const long long& number)
{
    return new Fraction(this->numerator_ * number, this->denominator_);
}

long long Simplex::Fraction::getNumerator() const
{
    return numerator_;
}

long long Simplex::Fraction::getDenominator() const
{
    return denominator_;
}

long double Simplex::Fraction::getFloatValue() const
{
    return static_cast<long double>(this->numerator_) / static_cast<long double>(this->denominator_);
}

long long Simplex::Fraction::gcd(const long long& a, const long long& b)
{
    return b == 0 ? a : gcd(b, a % b);
}

long long Simplex::Fraction::gcm(const long long& a, const long long& b)
{
    return (a * b) / gcd(a, b);
}

void Simplex::Fraction::simplify()
{
    long long gcd_ = gcd(this->numerator_, this->denominator_);

    this->denominator_ /= gcd_;
    this->numerator_   /= gcd_;
}
