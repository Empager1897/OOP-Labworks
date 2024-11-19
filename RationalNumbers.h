#pragma once
#ifndef RATIONAL_NUM
#define RATIONAL_NUM

#include <iostream>
#include "BigNumbers.h"
using namespace std;

class RationalNum {
    friend RationalNum operator+(const RationalNum& left, const RationalNum& right);
    friend RationalNum operator-(const RationalNum& left, const RationalNum& right);
    friend RationalNum operator*(const RationalNum& left, const RationalNum& right);
    friend RationalNum operator/(const RationalNum& left, const RationalNum& right);
    friend bool operator==(const RationalNum& left, const RationalNum& right);
    friend bool operator!=(const RationalNum& left, const RationalNum& right);
    friend bool operator<(const RationalNum& left, const RationalNum& right);
    friend bool operator>(const RationalNum& left, const RationalNum& right);
    friend bool operator<=(const RationalNum& left, const RationalNum& right);
    friend bool operator>=(const RationalNum& left, const RationalNum& right);
    friend ostream& operator<<(ostream& out, const RationalNum& obj);
    friend istream& operator>>(istream& in, RationalNum& obj);

public:
    RationalNum();
    RationalNum(BigNumber x);
    RationalNum(BigNumber numerator_, BigNumber denominator_ );

    RationalNum& operator=(const RationalNum& obj);
    RationalNum& operator+=(const RationalNum& obj);
    RationalNum& operator-=(const RationalNum& obj);
    RationalNum& operator*=(const RationalNum& obj);
    RationalNum& operator/=(const RationalNum& obj);
    RationalNum& operator++();
    RationalNum operator++(int);
    RationalNum& operator--();
    RationalNum operator--(int);
    RationalNum operator+() const;
    RationalNum operator-() const;

    void setNumerator(BigNumber numerator_ = 0);
    BigNumber getNumerator() const;
    void setDenominator(BigNumber denominator_ = 1);
    BigNumber getDenominator() const;

private:
    BigNumber numerator;
    BigNumber denominator;
    void simplify();
};
#endif