//
// Created by tracksale on 5/7/18.
//

#include "Fraction.hpp"

Simplex::Fraction::Fraction(BigInt numerator, BigInt denominator) : numerator_(numerator), denominator_(denominator)
{
    if (denominator == 0) {
        throw "Denominator can't be zero!";
    }

    if (denominator < 0) {
        denominator_ *= -1;
        numerator_   *= -1;
    }

    simplify();
}

Simplex::Fraction* Simplex::Fraction::operator *(const Fraction& fraction)
{
    return new Fraction(this->numerator_ * fraction.getNumerator(), this->denominator_ * fraction.getDenominator());
}

// Boolean Operators
bool Simplex::Fraction::operator <(const Fraction& fraction)
{
    return (this->numerator_ * fraction.getDenominator()) < (this->denominator_ * fraction.getNumerator());
}

bool Simplex::Fraction::operator >(const Fraction& fraction)
{
    return (this->numerator_ * fraction.getDenominator()) > (this->denominator_ * fraction.getNumerator());
}

bool Simplex::Fraction::operator <(const BigInt& number)
{
    return this->numerator_ * this->denominator_ < number;
}

bool Simplex::Fraction::operator <=(const BigInt& number)
{
    return (this->numerator_ / this->denominator_) <= number;
}

bool Simplex::Fraction::operator >=(const BigInt& number)
{
    return (this->numerator_ / this->denominator_) >= number;
}

bool Simplex::Fraction::operator ==(const BigInt& number)
{
    return (this->numerator_ / this->denominator_) == number;

}

bool Simplex::Fraction::operator >(const BigInt& number)
{
    return (this->numerator_ / this->denominator_) > number;
}

Simplex::Fraction* Simplex::Fraction::operator /(const Fraction& fraction)
{
    if (this->denominator_ == 0 || fraction.getNumerator() == 0) {
        return new Fraction(0, 1);
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

Simplex::Fraction* Simplex::Fraction::operator *(const BigInt& number)
{
    return new Fraction(this->numerator_ * number, this->denominator_);
}

BigInt Simplex::Fraction::getNumerator() const
{
    return numerator_;
}

BigInt Simplex::Fraction::getDenominator() const
{
    return denominator_;
}

long double Simplex::Fraction::getFloatValue() const
{
    int division = 2;

    auto greater = this->numerator_ > this->denominator_ ? this->numerator_ : this->denominator_;
    auto lower   = this->numerator_ < this->denominator_ ? this->numerator_ : this->denominator_;

    while (greater > BigInt(LONG_LONG_MAX)) {
        greater /= division;
        lower /= division;
    }

    while (lower < BigInt(-LONG_LONG_MAX)) {
        greater /= division;
        lower /= division;
    }

    if (this->numerator_ > this->denominator_) {
        return static_cast<long double>(greater.toLongLong()) / static_cast<long double>(lower.toLongLong());
    } else {
        return static_cast<long double>(lower.toLongLong()) / static_cast<long double>(greater.toLongLong());
    }
}

BigInt Simplex::Fraction::gcd(const BigInt& a, const BigInt& b) const
{
    return b == 0 ? a : gcd(b, a % b);
}

BigInt Simplex::Fraction::gcm(const BigInt& a, const BigInt& b) const
{
    return (a*b)/gcd(a,b);
}

void Simplex::Fraction::simplify()
{
    BigInt gcd_ = gcd(this->numerator_, this->denominator_);

    this->denominator_ /= gcd_;
    this->numerator_   /= gcd_;
}
