#include "RationalNumbers.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <climits>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

BigNumber& absInt(BigNumber& x) {
    if (x >= 0) {
        return x;
    }
    else {
        x = x * -1;
        return x;
    }
}

void getFactors(BigNumber num, vector<BigNumber>& factorSet) {
    if (num != BigNumber(1)) {
        factorSet.push_back(num);
    }
    for (BigNumber i = 2; i <= num / 2; i++) {
        if (num % i == 0){
            factorSet.push_back(i);
            factorSet.push_back(num / i);
        }
    }
}

void simplifyFun(BigNumber& a, BigNumber& b) {
    BigNumber tempN = a;
    BigNumber tempD = b;
    BigNumber small, temp;
    vector<BigNumber> factorSet;
    if (tempN == tempD) {
        a = 1;
        b = 1;
        return;
    }
    else if (tempN == -tempD ) {
        a = -1;
        b = 1;
        return;
    }
    else if (tempN == 0) {
        b = 1;
        return;
    }


    if (absInt(tempN) < absInt(tempD)) {
        small = absInt(tempN);
    }
    else {
        small = absInt(tempD);
    }

    getFactors(small, factorSet);
    for (int i = 0; i < factorSet.size(); i++) {
        temp = factorSet[i];
        while (tempN % temp == 0 && tempD % temp == 0) {
            tempN /= temp;
            tempD /= temp;
        }
    }
    a = a < 0 ? -tempN : tempN;
    b = b < 0 ? -tempD : tempD;
}


//friend functions definitions
RationalNum operator+(const RationalNum& left, const RationalNum& right) {
    RationalNum temp;
    BigNumber tempLD = left.getDenominator();
    BigNumber tempRD = right.getDenominator();
    simplifyFun(tempLD, tempRD);
    temp.setDenominator(left.getDenominator() * tempRD);
    temp.setNumerator(left.getNumerator() * tempRD + right.getNumerator() * tempLD);
    temp.simplify();
    return temp;
}

RationalNum operator-(const RationalNum& left, const RationalNum& right) {
    return left + (-right);
}

RationalNum operator*(const RationalNum& left, const RationalNum& right) {
    RationalNum temp;
    RationalNum temp_2(right.getNumerator(), left.getDenominator());
    RationalNum temp_3(left.getNumerator(), right.getDenominator());
    BigNumber a = temp_2.getDenominator();
    BigNumber b = temp_2.getNumerator();
    BigNumber c = temp_3.getDenominator();
    BigNumber d = temp_3.getNumerator();
    temp.setNumerator(b * d);
    temp.setDenominator(a * c);
    return temp;
}

RationalNum operator/(const RationalNum& left, const RationalNum& right) {
    RationalNum temp_1(left.getNumerator(), left.getDenominator());
    RationalNum temp_2(right.getDenominator(), right.getNumerator());
    return temp_1 * temp_2;
}

bool operator==(const RationalNum& left, const RationalNum& right) {
    BigNumber lside = left.getNumerator() * right.getDenominator();
    BigNumber rside = left.getDenominator() * right.getNumerator();
    return (lside == rside);
}

bool operator!=(const RationalNum& left, const RationalNum& right) {
    BigNumber lside = left.getNumerator() * right.getDenominator();
    BigNumber rside = left.getDenominator() * right.getNumerator();
    return !(lside == rside);
}

bool operator<(const RationalNum& left, const RationalNum& right) {
    BigNumber lside = left.getNumerator() * right.getDenominator();
    BigNumber rside = left.getDenominator() * right.getNumerator();
    return (lside < rside);
}

bool operator>(const RationalNum& left, const RationalNum& right) {
    BigNumber lside = left.getNumerator() * right.getDenominator();
    BigNumber rside = left.getDenominator() * right.getNumerator();
    return (lside > rside);
}

bool operator<=(const RationalNum& left, const RationalNum& right) {
    return ((left < right) || (left == right));
}

bool operator>=(const RationalNum& left, const RationalNum& right) {
    return ((left > right) || (left == right));
}

ostream& operator<<(ostream& out, const RationalNum& obj) {
    out << obj.numerator;
    if (obj.numerator != RationalNum(0) && obj.denominator != RationalNum(1)) {
        out << "/" << obj.denominator;
    }
    return out;
}

istream& operator>>(istream& in, RationalNum& obj) {
    string inputstr;
    int num = 0;
    int sign = 1;
    bool slashExist = false;
    bool dotExist = false;
    bool validInput = true;
    int virtualDenominator = 1;
    cin >> inputstr;
    for (int i = 0; i < inputstr.size(); i++) {
        char temp = inputstr[i];
        if (temp == '.') {
            if (dotExist == false && slashExist == false && i != 0) {
                dotExist = true;
            }
            else {
                validInput = false;
                break;
            }
        }
        else if (temp == '/') {
            if (dotExist == false && slashExist == false && i != 0) {
                slashExist = true;
                obj.setNumerator(sign * num);
                num = 0;
                sign = 1;
            }
            else {
                validInput = false;
                break;
            }
        }
        else if (temp == '-') {
            if (i == 0) {
                sign = -sign;
            }
            else if (inputstr[i - 1] == '/') {
                sign = -sign;
            }
            else {
                validInput = false;
                break;
            }
        }
        else if (temp <= '9' && temp >= '0') {
            if (dotExist) {
                if (virtualDenominator > INT_MAX / 10) {
                    cout << "this frational is too long to handle.";
                    validInput = false;
                    break;
                }
                else {
                    virtualDenominator *= 10;
                }
            }
            if (num > INT_MAX / 10) {
                cout << "this number is too long to handle.";
                validInput = false;
                break;
            }
            num *= 10;
            num += inputstr[i] - '0';
        }
        else {
            validInput = false;
            break;
        }
    }

    if (validInput == false) {
        obj.setNumerator(0);
        obj.setDenominator(1);
        cout << "Input is not valid! The whole set to 0" << endl;
    }
    else {
        if (slashExist == true) {
            obj.setDenominator(sign * num);
        }
        else if (dotExist) {
            obj.setNumerator(sign * num);
            obj.setDenominator(virtualDenominator);
        }
        else {
            obj.setNumerator(sign * num);
            obj.setDenominator(1);
        }
    }
    obj.simplify();
    return in;
}




//member function definition
RationalNum::RationalNum() {
    setNumerator(0);
    setDenominator(1);
}

RationalNum::RationalNum(BigNumber x) {
    /*int i = 1;
    while (x * i - static_cast<int>(x * i) != 0) {
        if (i > INT_MAX / 10) {
            cout << "this frational number : " << x << " can not be transfer to rational number, it's too long, now set it 0." << endl;
            setNumerator(0);
            setDenominator(1);
            return;
        }
        else {
            i *= 10;
        }
    }*/
    setNumerator(x);
    setDenominator();
    simplify();
}

RationalNum::RationalNum(BigNumber numerator_, BigNumber denominator_) {
    setNumerator(numerator_);
    setDenominator(denominator_);
    simplify();
}

RationalNum& RationalNum::operator=(const RationalNum& obj) {
    setNumerator(obj.getNumerator());
    setDenominator(obj.getDenominator());
    return *this;
}

RationalNum& RationalNum::operator+=(const RationalNum& obj) {
    *this = *this + obj;
    return *this;
}

RationalNum& RationalNum::operator-=(const RationalNum& obj) {
    *this = *this - obj;
    return *this;
}

RationalNum& RationalNum::operator*=(const RationalNum& obj) {
    *this = *this * obj;
    return *this;
}

RationalNum& RationalNum::operator/=(const RationalNum& obj) {
    *this = *this / obj;
    return *this;
}
RationalNum& RationalNum::operator++() {
    *this = *this + RationalNum(1);
    return *this;
}

RationalNum RationalNum::operator++(int) {
    RationalNum before = *this;
    *this = *this + RationalNum(1);
    return before;
}

RationalNum& RationalNum::operator--() {
    *this = *this - RationalNum(1);
    return *this;
}

RationalNum RationalNum::operator--(int) {
    RationalNum before = *this;
    *this = *this - RationalNum(1);
    return before;
}

RationalNum RationalNum::operator+() const {
    return *this;
}

RationalNum RationalNum::operator-() const {
    RationalNum temp;
    temp.setNumerator(-getNumerator());
    temp.setDenominator(getDenominator());
    return temp;
}

void RationalNum::setNumerator(BigNumber numerator_) {
    numerator = numerator_;
}

BigNumber RationalNum::getNumerator() const {
    return numerator;
}

void RationalNum::setDenominator(BigNumber denominator_) {
    if (denominator_ == 0) {
        denominator = 1;
        numerator = 0;
        cout << "Denominator is 0! Not good! THe whole is set to 0." << endl;
    }
    else {
        denominator = denominator_;
    }
}

BigNumber RationalNum::getDenominator() const {
    return denominator;
}


void RationalNum::simplify() {
    BigNumber tempN = numerator;
    BigNumber tempD = denominator;
    simplifyFun(tempN, tempD);
    setNumerator(tempN);
    setDenominator(tempD);
}