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
    // We want to cast our division to 'long double'.
    // But it's possible only with primitive types (not BigInt).
    // So, we need check if both numerator and denominator fits into a 'long long int' (biggest type).
    // If doesn't, truncate both until it fits!

    int truncate_rate = 2;

    auto num_aux = this->numerator_;
    auto den_aux = this->denominator_;

    while ( num_aux > BigInt(LLONG_MAX) || den_aux > BigInt(LLONG_MAX) ||
            num_aux < BigInt(LLONG_MIN) || den_aux < BigInt(LLONG_MIN))
    {
        num_aux /= truncate_rate;
        den_aux /= truncate_rate;
    }

    return static_cast<long double>(num_aux.toLongLong()) / static_cast<long double>(den_aux.toLongLong());
}

BigInt Simplex::Fraction::gcd(const BigInt& a, const BigInt& b) const
{
    return b == 0 ? a : gcd(b, a % b);
}

BigInt Simplex::Fraction::gcm(const BigInt& a, const BigInt& b) const
{
    return (a * b) / gcd(a, b);
}

void Simplex::Fraction::simplify()
{
    BigInt gcd_ = gcd(this->numerator_, this->denominator_);

    this->denominator_ /= gcd_;
    this->numerator_   /= gcd_;
}
