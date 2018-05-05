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

private:

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
