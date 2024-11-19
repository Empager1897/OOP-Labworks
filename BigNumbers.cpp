#include "BigNumbers.h"
#include <sstream>
#include <stack>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <random>

BigNumber::BigNumber(): _numberString("")
{
}

BigNumber::BigNumber(std::string number) : _numberString(number)
{
}

BigNumber::BigNumber(long long number) :_numberString(std::to_string(number))
{
}

BigNumber BigNumber::add(BigNumber other) {
    BigNumber b1 = other > *this ? other : *this;
    BigNumber b2 = other > *this ? *this : other;
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            return b1.negate().add(b2.negate()).negate();
        }
        else if (b1.isNegative() && !b2.isNegative()) {
            return b1.negate().subtract(b2).negate();
        }
        else {
            return b2.negate().subtract(b1).negate();
        }
    }
    std::string results;
    int carry = 0;
    int diff = int(b1._numberString.size() - b2._numberString.size());
    for (int i = 0; i < diff; ++i) {
        b2._numberString.insert(b2._numberString.begin(), '0');
    }
    for (int i = int(b1._numberString.size() - 1); i >= 0; --i) {
        int sum = (b1._numberString[i] - '0') + (b2._numberString[i] - '0') + carry;
        carry = 0;
        if (sum <= 9 || i == 0) {
            results.insert(0, std::to_string(sum));
        }
        else {
            results.insert(0, std::to_string(sum % 10));
            carry = 1;
        }
    }
    return BigNumber(results);
}

BigNumber BigNumber::addll(const long long& other) {
    return this->add(BigNumber(other));
}

BigNumber BigNumber::addstr(const std::string& other) {
    return this->add(BigNumber(other));
}


BigNumber BigNumber::subtract(BigNumber other) {
    BigNumber b1 = *this, b2 = other;
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            return b1.negate().subtract(b2.negate()).negate();
        }
        else if (b1.isNegative() && !b2.isNegative()) {
            return b1.negate().add(b2).negate();
        }
        else {
            return b2.negate().add(b1);
        }
    }
    std::string results;
    int n = 0, p = 0;
    bool takeOffOne = false;
    bool shouldBeTen = false;

    if (b1 < b2) {
        //Negative answer
        std::string t = b2.subtract(*this).negate().getString();
        for (unsigned int i = 1; i < t.length(); ++i) {
            if (t[i] != '0') break;
            t.erase(1, 1);
        }
        return BigNumber(t);
    }

    //This next if-block fixes the case where the digit difference is greater than 1
    //100 - 5 is an example. This code adds 0's to make it, for example, 100 - 05, which
    //allows the rest of the subtraction code to work.
    if (b1._numberString.size() - b2.getString().size() > 1) {
        for (unsigned long i = 0; i < b1._numberString.size() - b2.getString().size() - 1; ++i) {
            b2._numberString.insert(b2._numberString.begin(), '0');
        }
    }
    int i = int(b1._numberString.size() - 1);
    for (int j = int(b2._numberString.size() - 1); j >= 0; --j) {
        if (((b1._numberString[i] - '0') < (b2._numberString[j] - '0')) && i > 0) {
            n = char((b1._numberString[i] - '0') + 10);
            takeOffOne = true;
            if (j > 0 || b1._numberString[i - 1] != '0') {
                p = char((b1._numberString[i - 1] - '0') - 1);
                if (p == -1) {
                    p = 9;
                    shouldBeTen = true;
                }
                takeOffOne = false;
            }
            if (shouldBeTen) {
                int index = i - 1;
                for (int a = i - 1; (b1._numberString[a] - '0') == 0; --a) {
                    b1._numberString[a] = static_cast<char>(p + '0');
                    --index;
                }
                int t = (b1._numberString[index] - '0') - 1;
                b1._numberString[index] = static_cast<char>(t + '0');
            }
            b1._numberString[i - 1] = static_cast<char>(p + '0');
            shouldBeTen = false;
        }
        std::stringstream ss;
        if (((b1._numberString[i] - '0') == (b2._numberString[j] - '0'))) {
            ss << "0";
        }
        else {
            if (n <= 0) {
                ss << ((b1._numberString[i] - '0') - (b2._numberString[j] - '0'));
            }
            else {
                ss << (n - (b2._numberString[j] - '0'));
            }
        }

        results.insert(0, ss.str());
        --i;
        n = 0;
    }
    if (takeOffOne) {
        std::string number = "";
        for (int j = b1._numberString.length() - b2._numberString.length() - 1; j >= 0; --j) {
            if (b1._numberString[j] == '0') {
                number += "0";
                continue;
            }
            else {
                number.insert(number.begin(), b1._numberString[j]);
                int t = atoi(number.c_str());
                --t;
                b1._numberString.replace(0, number.size(), std::to_string(t));
                break;
            }
        }
    }
    while (i >= 0) {
        std::stringstream ss;
        if (i == 0) {
            if (b1._numberString[i] - '0' != 0) {
                ss << (b1._numberString[i] - '0');
                results.insert(0, ss.str());
            }
        }
        else {
            ss << (b1._numberString[i] - '0');
            results.insert(0, ss.str());
        }

        --i;
    }
    //In the case of all 0's, we only want to return one of them
    if (results.find_first_not_of('0') == std::string::npos) {
        results = "0";
    }
    else if (results[0] == '0') {
        int index = results.find_first_not_of('0');
        results = results.substr(index, results.length() - 1);
    }
    return BigNumber(results);
}

BigNumber BigNumber::subtractll(const long long& other) {
    return this->subtract(BigNumber(other));
}

BigNumber BigNumber::subtractstr(const std::string& other) {
    return this->subtract(BigNumber(other));
}

BigNumber BigNumber::multiply(BigNumber other) {
    BigNumber b1 = other > *this ? other : *this;
    BigNumber b2 = other > *this ? *this : other;
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            return b1.negate().multiply(b2.negate());
        }
        else if (b1.isNegative() && !b2.isNegative()) {
            return b1.negate().multiply(b2).negate();
        }
        else {
            return b2.negate().multiply(b1).negate();
        }
    }
    if (b1 == 0 || b2 == 0) return 0;
    int carry = 0;
    int zeroCounter = 0;
    BigNumber b = 0;

    for (unsigned int i = 0; i < b1._numberString.size() - b2._numberString.size(); ++i) {
        b2._numberString.insert(b2._numberString.begin(), '0');
    }
    for (long long int i = (b2._numberString.size() - 1); i >= 0; --i) {
        std::string rr;
        for (long long int j = int(b1._numberString.size() - 1); j >= 0; --j) {
            int val = ((b2._numberString[i] - '0') * (b1._numberString[j] - '0')) + carry;
            carry = 0;
            if (val > 9 && j != 0) {
                carry = val / 10;
                rr.insert(0, std::to_string(val % 10));
            }
            else {
                rr.insert(0, std::to_string(val));
            }
        }
        if (zeroCounter > 0) {
            for (int x = 0; x < zeroCounter; ++x) {
                rr.append("0");
            }
        }
        ++zeroCounter;
        b += BigNumber(rr);
    }
    if (b._numberString.find_first_not_of('0') != std::string::npos) {
        b.setString(b._numberString.erase(0, b._numberString.find_first_not_of('0')));
    }
    else {
        //In the case of all 0's, we only want to return one of them
        b.setString("0");
    }
    return b;
}

BigNumber BigNumber::multiplyll(const long long& other) {
    if (other == 0)
        return 0;
    if (other == 1)
        return *this;
    auto original = *this;
    for (auto i = 0; i < other - 1; ++i) {
        *this += original;
    }
    return other < 0 ? -(*this) : *this;
}

BigNumber BigNumber::multiplystr(const std::string& other) {
    return this->multiply(BigNumber(other));
}

std::string& removeZero(std::string& str)
{
    // Count leading zeros 
    int i = 0;
    while (str[i] == '0')
        i++;

    // The erase function removes i characters 
    // from given index (0 here) 
    str.erase(0, i);

    return str;
}

BigNumber BigNumber::karatsuba_multiply(const BigNumber& other) const {
    BigNumber a = *this;
    BigNumber b = other;

    if (a == 0 || b == 0) return BigNumber("0");
    if (a < 10 || b < 10)
        return a * b;
    int maxlen = std::max(a.digits(), b.digits());
    int half = maxlen / 2;

    std::string high1 = a._numberString.substr(0, a.digits() - half);
    std::string low1 = a._numberString.substr(a.digits() - half);
    std::string high2 = b._numberString.substr(0, b.digits() - half);
    std::string low2 = b._numberString.substr(b.digits() - half);

    BigNumber high1Big(removeZero(high1)), low1Big(removeZero(low1));
    BigNumber high2Big(removeZero(high2)), low2Big(removeZero(low2));

    BigNumber z0 = low1Big.karatsuba_multiply(low2Big);                       
    BigNumber z1 = (low1Big + high1Big).karatsuba_multiply(low2Big + high2Big); 
    BigNumber z2 = high1Big.karatsuba_multiply(high2Big);                      

    BigNumber temp1("1" + std::string(2 * half, '0')); // 10^(2 * half)
    BigNumber temp2("1" + std::string(half, '0'));     // 10^half

    BigNumber result = (z2 * temp1) + ((z1 - z2 - z0) * temp2) + z0;
    return result;
}

/*BigNumber BigNumber::toomCook3Multiply(const BigNumber& other) const {
    BigNumber a = *this;
    BigNumber b = other;

    if (a.digits() <= 1 || b.digits() <= 1) {
        return a * b; 
    }

    int maxLen = std::max(a.digits(), b.digits());
    int splitSize = (maxLen + 2) / 3; 

    BigNumber a2(a._numberString.substr(0, a.digits() - 2 * splitSize));
    BigNumber a1(a._numberString.substr(a.digits() - 2 * splitSize, splitSize));
    BigNumber a0(a._numberString.substr(a.digits() - splitSize));

    BigNumber b2(b._numberString.substr(0, b.digits() - 2 * splitSize));
    BigNumber b1(b._numberString.substr(b.digits() - 2 * splitSize, splitSize));
    BigNumber b0(b._numberString.substr(b.digits() - splitSize));

    a2 = BigNumber(removeZero(a2._numberString));
    a1 = BigNumber(removeZero(a1._numberString));
    a0 = BigNumber(removeZero(a0._numberString));

    b2 = BigNumber(removeZero(b2._numberString));
    b1 = BigNumber(removeZero(b1._numberString));
    b0 = BigNumber(removeZero(b0._numberString));

    BigNumber p0 = a0.toomCook3Multiply(b0);                     // p0 = a0 * b0
    BigNumber p1 = (a0 + a1 + a2).toomCook3Multiply(b0 + b1 + b2); // p1 = (a0 + a1 + a2) * (b0 + b1 + b2)
    BigNumber pNeg1 = (a0 - a1 + a2).toomCook3Multiply(b0 - b1 + b2); // pNeg1 = (a0 - a1 + a2) * (b0 - b1 + b2)
    BigNumber pInf = a2.toomCook3Multiply(b2);                   // pInf = a2 * b2

    // Interpolation
    BigNumber r0 = p0;
    BigNumber r4 = pInf;
    BigNumber r3 = (pNeg1 - p1) / 2;
    BigNumber r1 = (p1 - p0 - pInf) / 2;
    BigNumber r2 = (p1 - r0 - r1 - r4) / 6;

    // Computing result and multiplying by 10^k
    BigNumber result = r4 * BigNumber("1" + std::string(4 * splitSize, '0')) +
        r3 * BigNumber("1" + std::string(3 * splitSize, '0')) +
        r2 * BigNumber("1" + std::string(2 * splitSize, '0')) +
        r1 * BigNumber("1" + std::string(splitSize, '0')) +
        r0;

    return result;
}*/

// Remove Zeros
static std::string removeZeroV2(const std::string& num) {
    size_t pos = num.find_first_not_of('0');
    if (pos != std::string::npos) {
        return num.substr(pos);
    }
    return "0";
}

//Add zeros
static std::string padZeros(const std::string& num, int newLength) {
    if (num.size() >= newLength) return num;
    return std::string(newLength - num.size(), '0') + num;
}

BigNumber BigNumber::toomCook3Multiply(const BigNumber& other) const {
    BigNumber a = *this;
    BigNumber b = other;

    if (a.digits() <= 1 || b.digits() <= 1) {
        return a * b;
    }

    int maxLen = std::max(a.digits(), b.digits());
    int splitSize = (maxLen + 2) / 3;

    a._numberString = padZeros(a._numberString, 3 * splitSize);
    b._numberString = padZeros(b._numberString, 3 * splitSize);

    BigNumber a2(a._numberString.substr(0, splitSize));
    BigNumber a1(a._numberString.substr(splitSize, splitSize));
    BigNumber a0(a._numberString.substr(2 * splitSize, splitSize));

    BigNumber b2(b._numberString.substr(0, splitSize));
    BigNumber b1(b._numberString.substr(splitSize, splitSize));
    BigNumber b0(b._numberString.substr(2 * splitSize, splitSize));

    BigNumber p0 = a0.toomCook3Multiply(b0);
    BigNumber p1 = (a0 + a1 + a2).toomCook3Multiply(b0 + b1 + b2);
    BigNumber pNeg1 = (a0 - a1 + a2).toomCook3Multiply(b0 - b1 + b2);
    BigNumber pInf = a2.toomCook3Multiply(b2);

    BigNumber r0 = p0;
    BigNumber r4 = pInf;
    BigNumber r3 = (pNeg1 - p1) / 2;
    BigNumber r1 = (p1 - p0 - pInf) / 2;
    BigNumber r2 = (p1 - r0 - r1 - r4) / 6;

    return r4 * BigNumber("1" + std::string(4 * splitSize, '0')) +
        r3 * BigNumber("1" + std::string(3 * splitSize, '0')) +
        r2 * BigNumber("1" + std::string(2 * splitSize, '0')) +
        r1 * BigNumber("1" + std::string(splitSize, '0')) +
        r0;
}

BigNumber BigNumber::ToomCook5_multiply(const BigNumber& other) const {
    BigNumber a = *this;
    BigNumber b = other;

    if (a.digits() <= 1 || b.digits() <= 1) {
        return a * b;
    }

    int maxLen = std::max(a.digits(), b.digits());
    int splitSize = (maxLen + 4) / 5;

    a._numberString = padZeros(a._numberString, 5 * splitSize);
    b._numberString = padZeros(b._numberString, 5 * splitSize);

    BigNumber a4(a._numberString.substr(0, a.digits() - 4 * splitSize));
    BigNumber a3(a._numberString.substr(a.digits() - 4 * splitSize, splitSize));
    BigNumber a2(a._numberString.substr(a.digits() - 3 * splitSize, splitSize));
    BigNumber a1(a._numberString.substr(a.digits() - 2 * splitSize, splitSize));
    BigNumber a0(a._numberString.substr(a.digits() - splitSize));

    BigNumber b4(b._numberString.substr(0, b.digits() - 4 * splitSize));
    BigNumber b3(b._numberString.substr(b.digits() - 4 * splitSize, splitSize));
    BigNumber b2(b._numberString.substr(b.digits() - 3 * splitSize, splitSize));
    BigNumber b1(b._numberString.substr(b.digits() - 2 * splitSize, splitSize));
    BigNumber b0(b._numberString.substr(b.digits() - splitSize));

    a4 = BigNumber(removeZero(a4._numberString));
    a3 = BigNumber(removeZero(a3._numberString));
    a2 = BigNumber(removeZero(a2._numberString));
    a1 = BigNumber(removeZero(a1._numberString));
    a0 = BigNumber(removeZero(a0._numberString));

    b4 = BigNumber(removeZero(b4._numberString));
    b3 = BigNumber(removeZero(b3._numberString));
    b2 = BigNumber(removeZero(b2._numberString));
    b1 = BigNumber(removeZero(b1._numberString));
    b0 = BigNumber(removeZero(b0._numberString));

    BigNumber p0 = a0.ToomCook5_multiply(b0);                               // p0 = a0 * b0
    BigNumber p1 = (a0 + a1 + a2 + a3 + a4).ToomCook5_multiply(b0 + b1 + b2 + b3 + b4); // p1 = (a0 + a1 + a2 + a3 + a4) * (b0 + b1 + b2 + b3 + b4)
    BigNumber pNeg1 = (a0 - a1 + a2 - a3 + a4).ToomCook5_multiply(b0 - b1 + b2 - b3 + b4); // pNeg1 = (a0 - a1 + a2 - a3 + a4) * (b0 - b1 + b2 - b3 + b4)
    BigNumber p2 = (a0 + 2 * a1 + 4 * a2 + 8 * a3 + 16 * a4).ToomCook5_multiply(b0 + 2 * b1 + 4 * b2 + 8 * b3 + 16 * b4); // p2
    BigNumber pNeg2 = (a0 - 2 * a1 + 4 * a2 - 8 * a3 + 16 * a4).ToomCook5_multiply(b0 - 2 * b1 + 4 * b2 - 8 * b3 + 16 * b4); // pNeg2
    BigNumber pInf = a4.ToomCook5_multiply(b4);                              // pInf = a4 * b4

    // Interpolation
    BigNumber r0 = p0;
    BigNumber r4 = pInf;
    BigNumber r3 = (pNeg1 - p1) / 2;
    BigNumber r1 = (p1 - pNeg1) / 2;
    BigNumber r2 = (p2 - pNeg2) / 8;

    // Equalizing
    r1 = (r1 - r0 - r4) / 2;
    r3 = (r3 - r1 - r4) / 4;
    r2 = r2 - r3 - r4;

    // Computing resulting value
    BigNumber result = r4 * BigNumber("1" + std::string(8 * splitSize, '0')) +
        r3 * BigNumber("1" + std::string(6 * splitSize, '0')) +
        r2 * BigNumber("1" + std::string(4 * splitSize, '0')) +
        r1 * BigNumber("1" + std::string(2 * splitSize, '0')) +
        r0;

    return result;
}

int jacobi(const BigNumber& a, const BigNumber& n) {
    BigNumber a_abs = a.abs();
    BigNumber n_abs = n.abs();

    if (n_abs == BigNumber("1")) return 1;

    int result = 1;
    BigNumber a_mod_n = a_abs % n_abs;

    while (a_mod_n != BigNumber("0")) {
        // if a is even use Jacobi properties
        while (a_mod_n.isEven()) {
            a_mod_n /= BigNumber("2");
            BigNumber n_mod_8 = n_abs % BigNumber("8");

            if (n_mod_8 == BigNumber("3") || n_mod_8 == BigNumber("5")) {
                result = -result;
            }
        }

        // Swap a and n
        std::swap(a_mod_n, n_abs);

        // if a && n =3(mod4)
        if ((a_mod_n % BigNumber("4") == BigNumber("3")) && (n_abs % BigNumber("4") == BigNumber("3"))) {
            result = -result;
        }

        a_mod_n = a_mod_n % n_abs;
    }

    return n_abs == BigNumber("1") ? result : 0;
}

// Power by module
BigNumber mod_exp(const BigNumber& base, const BigNumber& exp, const BigNumber& mod) {
    BigNumber result("1");
    BigNumber base_mod = base % mod;
    BigNumber exponent = exp;

    while (exponent > BigNumber("0")) {
        if (exponent.isOdd()) {
            result = (result * base_mod) % mod;
        }
        exponent /= BigNumber("2");
        base_mod = (base_mod * base_mod) % mod;
    }

    return result;
}

bool BigNumber::Solovay_Strassen(int iterations) const {
    if (*this < BigNumber("2")) return false;
    if (*this == BigNumber("2")) return true;
    if (this->isEven()) return false;

    BigNumber n = *this;
    BigNumber n_minus_1 = n - BigNumber("1");

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < iterations; ++i) {
        // generating random a from [2, n-1]
        BigNumber a;
        do {
            a = BigNumber((gen() % (n_minus_1 - BigNumber("2") + BigNumber("1")) + 2));
        } while (a < BigNumber("2") || a > n_minus_1);

        int jacobi_symbol = jacobi(a, n);
        if (jacobi_symbol == 0) return false;

        BigNumber exp = n_minus_1 / BigNumber("2");
        BigNumber mod_exp_result = mod_exp(a, exp, n);

        if ((mod_exp_result != BigNumber(std::to_string(jacobi_symbol))) &&
            (mod_exp_result != n + BigNumber(std::to_string(jacobi_symbol)))) {
            return false;
        }
    }

    return true;
}

bool BigNumber::Rabin_Miller(int iterations) const {
    if (*this < BigNumber("2")) return false;
    if (*this == BigNumber("2")) return true;
    if (this->isEven()) return false;

    BigNumber n = *this;
    BigNumber n_minus_1 = n - BigNumber("1");

    BigNumber d = n_minus_1;
    int s = 0;
    while (d.isEven()) {
        d /= BigNumber("2");
        ++s;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < iterations; ++i) {
        // generating random a in range [2, n-2]
        BigNumber a;
        do {
            a = BigNumber((gen() % (n_minus_1 - BigNumber("2") + BigNumber("1")) + 2));
        } while (a < BigNumber("2") || a > n_minus_1);

        BigNumber x = mod_exp(a, d, n);

        if (x == BigNumber("1") || x == n_minus_1) continue;

        bool found = false;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;

            if (x == n_minus_1) {
                found = true;
                break;
            }
            if (x == BigNumber("1")) return false;
        }

        if (!found) return false;
    }

    return true;
}

bool BigNumber::Luke_Lemer() const {
    BigNumber Mp = *this + BigNumber("1");
    int p = 0;

    BigNumber power_of_two("1");
    while (power_of_two < Mp) {
        power_of_two = power_of_two * BigNumber("2");
        p++;
    }

    if (power_of_two != Mp) {
        std::cerr << "Not a mersen number: (2^p - 1)" << std::endl;
        return false;
    }

    BigNumber S("4");
    BigNumber Mp_mod = *this;

    for (int i = 0; i < p - 2; ++i) {
        S = (S * S - BigNumber("2")) % Mp_mod;
    }

    return S == BigNumber("0");
}

BigNumber BigNumber::divide(BigNumber other) {
    if (other == 0) {
        std::cerr << "You cannot divide by 0!" << std::endl;
    }
    BigNumber b1 = *this, b2 = other;
    bool sign = false;
    if (b1.isNegative() && b2.isNegative()) {
        b1.negate();
        b2.negate();
    }
    else if (b1.isNegative() && !b2.isNegative()) {
        b1.negate();
        sign = true;
    }
    else if (!b1.isNegative() && b2.isNegative()) {
        b2.negate();
        sign = true;
    }
    BigNumber quotient = 0;
    while (b1 >= b2) {
        b1 -= b2;
        ++quotient;
    }
    if (sign) quotient.negate();
    return quotient;
}

BigNumber BigNumber::dividell(const long long& other) {
    return this->divide(BigNumber(other));
}

BigNumber BigNumber::dividestr(const std::string& other) {
    return this->divide(BigNumber(other));
}

BigNumber BigNumber::pow(int exponent) {
    if (exponent < 0) std::cerr << "Powers less than 0 are not supported" << std::endl;
    if (exponent == 0) return BigNumber("1");
    if (exponent == 1) return *this;
    BigNumber result = 1, base = *this;
    while (exponent) {
        if (exponent & 1) {
            result *= base;
        }
        exponent >>= 1;
        base *= base;
    }
    return result;
}

std::string BigNumber::getString() {
    return this->_numberString;
}

BigNumber BigNumber::setString(const std::string& newStr) {
    this->_numberString = newStr;
    return *this;
}

BigNumber BigNumber::negate() {
    if (this->_numberString[0] == '-') {
        this->_numberString.erase(0, 1);
    }
    else {
        this->_numberString.insert(this->_numberString.begin(), '-');
    }
    return *this;
}
BigNumber BigNumber::trimLeadingZeros() {
    BigNumber b = *this;
    if (b._numberString.find_first_not_of('0') != std::string::npos) {
        b.setString(b._numberString.erase(0, b._numberString.find_first_not_of('0')));
    }
    return b;
}

bool BigNumber::equals(const BigNumber& other) {
    return this->_numberString == other._numberString;
}

bool BigNumber::equals(const long long& other) {
    return this->getString() == std::to_string(other);
}

bool BigNumber::equals(const std::string& other) {
    return this->getString() == other;
}

unsigned int BigNumber::digits() {
    return this->_numberString.length() - static_cast<int>(this->isNegative());
}

bool BigNumber::isNegative() const {
    return this->_numberString[0] == '-';
}

bool BigNumber::isPositive() {
    return !this->isNegative();
}

bool BigNumber::isEven() const {
    return this->_numberString[this->_numberString.length() - 1] % 2 == 0;
}

bool BigNumber::isOdd() const {
    return !this->isEven();
}

BigNumber BigNumber::abs() const {
    return BigNumber(this->_numberString.substr(static_cast<unsigned int>(this->isNegative())));
}

std::ostream& operator<<(std::ostream& os, const BigNumber& num) {
    os << num._numberString;
    return os;
}

BigNumber operator+(BigNumber b1, const BigNumber& b2) {
    return b1.add(b2);
}

BigNumber operator+(BigNumber b1, const long long& b2) {
    return b1.addll(b2);
}

BigNumber operator+(BigNumber b1, const std::string& b2) {
    return b1.addstr(b2);
}

BigNumber operator-(BigNumber b1, const BigNumber& b2) {
    return b1.subtract(b2);
}

BigNumber operator-(BigNumber b1, const long long& b2) {
    return b1.subtractll(b2);
}

BigNumber operator-(BigNumber b1, const std::string& b2) {
    return b1.subtractstr(b2);
}

BigNumber operator*(BigNumber b1, const BigNumber& b2) {
    return b1.multiply(b2);
}

BigNumber operator*(BigNumber b1, const long long& b2) {
    return b1.multiplyll(b2);
}

BigNumber operator*(BigNumber b1, const std::string& b2) {
    return b1.multiplystr(b2);
}

BigNumber operator/(BigNumber b1, const BigNumber& b2) {
    return b1.divide(b2);
}

BigNumber operator/(BigNumber b1, const long long& b2) {
    return b1.dividell(b2);
}

BigNumber operator/(BigNumber b1, const std::string& b2) {
    return b1.dividestr(b2);
}

BigNumber operator%(BigNumber b1, const BigNumber& b2) {
    return b1 - b1.divide(b2);
}
BigNumber operator%(BigNumber b1, const long long& b2) {
    return b1 - b1.dividell(b2);
}
BigNumber operator%(BigNumber b1, const std::string& b2) {
    return b1 - b1.dividestr(b2);
}

BigNumber operator^(BigNumber b1, const int& b2) {
    return b1.pow(b2);
}

bool operator==(BigNumber b1, const BigNumber& b2) {
    return b1.equals(b2);
}

bool operator==(BigNumber b1, const long long& b2) {
    return b1.equals(b2);
}

bool operator==(BigNumber b1, const std::string& b2) {
    return b1.equals(b2);
}

bool operator!=(BigNumber b1, const BigNumber& b2) {
    return !(b1.equals(b2));
}

bool operator!=(BigNumber b1, const long long& b2) {
    return !(b1.equals(b2));
}

bool operator!=(BigNumber b1, const std::string& b2) {
    return !(b1.equals(b2));
}

bool operator>(BigNumber b1, const BigNumber& b2) {
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            BigNumber bt = b2;
            b1._numberString.erase(0, 1);
            bt._numberString.erase(0, 1);
            return b1 < bt;
        }
        else {
            return !(b1.isNegative() && !b2.isNegative());
        }
    }
    b1 = b1.trimLeadingZeros();
    auto c = BigNumber(b2);
    c = c.trimLeadingZeros();
    if (b1 == c) {
        return false;
    }
    if (b1._numberString.size() > c._numberString.size()) {
        return true;
    }
    else if (c._numberString.size() > b1._numberString.size()) {
        return false;
    }
    else {
        for (unsigned int i = 0; i < b1._numberString.size(); ++i) {
            if (b1[i] == static_cast<unsigned int>(c._numberString[i] - '0')) {
                continue;
            }
            return b1[i] > static_cast<unsigned int>(c._numberString[i] - '0');
        }
    }
    return false;
}

bool operator<(BigNumber b1, const BigNumber& b2) {
    return !(b1 == b2) && !(b1 > b2);
}

bool operator>=(BigNumber b1, const BigNumber& b2) {
    return b1 > b2 || b1 == b2;
}

bool operator<=(BigNumber b1, const BigNumber& b2) {
    return b1 < b2 || b1 == b2;
}

unsigned int BigNumber::operator[](int index) {
    if (this->_numberString[index] == '-') {
        std::cerr << "You cannot get the negative sign from the number" << std::endl;
    }
    return static_cast<unsigned int>(this->_numberString[index] - '0');
}

BigNumber& BigNumber::operator=(const BigNumber& other) {
    this->_numberString = other._numberString;
    return *this;
}

BigNumber& BigNumber::operator=(const long long& other) {
    this->_numberString = std::to_string(other);
    return *this;
}

BigNumber& BigNumber::operator=(const std::string& other) {
    this->_numberString = other;
    return *this;
}

BigNumber& BigNumber::operator+=(const BigNumber& other) {
    *this = *this + other;
    return *this;
}

BigNumber& BigNumber::operator+=(const long long& other) {
    *this = *this + other;
    return *this;
}

BigNumber& BigNumber::operator+=(const std::string& other) {
    *this = *this + other;
    return *this;
}

BigNumber& BigNumber::operator-=(const BigNumber& other) {
    *this = *this - other;
    return *this;
}

BigNumber& BigNumber::operator-=(const long long& other) {
    *this = *this - other;
    return *this;
}

BigNumber& BigNumber::operator-=(const std::string& other) {
    *this = *this - other;
    return *this;
}

BigNumber& BigNumber::operator*=(const BigNumber& other) {
    *this = *this * other;
    return *this;
}

BigNumber& BigNumber::operator*=(const long long& other) {
    *this = *this * other;
    return *this;
}

BigNumber& BigNumber::operator*=(const std::string& other) {
    *this = *this * other;
    return *this;
}

BigNumber& BigNumber::operator/=(const BigNumber& other) {
    *this = *this / other;
    return *this;
}

BigNumber& BigNumber::operator/=(const long long& other) {
    *this = *this / other;
    return *this;
}

BigNumber& BigNumber::operator/=(const std::string& other) {
    *this = *this / other;
    return *this;
}

BigNumber& BigNumber::operator++() {
    *this += BigNumber("1");
    return *this;
}

BigNumber& BigNumber::operator--() {
    *this -= BigNumber("1");
    return *this;
}

BigNumber BigNumber::operator++(int) {
    BigNumber t(this->getString());
    ++(*this);
    return t;
}

BigNumber BigNumber::operator--(int) {
    BigNumber t(this->getString());
    --(*this);
    return t;
}

BigNumber BigNumber::operator+() const {
    return *this;
}

BigNumber BigNumber::operator-() const {
    BigNumber temp = *this;
    temp.negate();
    return temp;
}