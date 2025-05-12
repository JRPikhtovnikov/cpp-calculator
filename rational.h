#pragma once

#include <numeric>
#include <cstdint>
#include <compare>
#include <iostream>

class Rational {
public:
    Rational() : numerator_(0), denominator_(1) {}

    Rational(int numerator) : numerator_(numerator), denominator_(1) {}

    Rational(int numerator, int denominator) : numerator_(numerator), denominator_(denominator) {
        if (denominator_ == 0) {
            std::abort();
        }
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        Reduction();
    }

    Rational(const Rational& rational) = default;
    Rational& operator=(const Rational& other) = default;

    Rational& operator=(int value) {
        numerator_ = value;
        denominator_ = 1;
        return *this;
    }

    int GetNumerator() const {
        return numerator_;
    }

    int GetDenominator() const {
        return denominator_;
    }

    void Reduction() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int gcd = std::gcd(numerator_, denominator_);
        numerator_ /= gcd;
        denominator_ /= gcd;
    }


    Rational operator+(const Rational& other) const {
        int64_t new_num = static_cast<int64_t>(numerator_) * other.denominator_ +
                          static_cast<int64_t>(other.numerator_) * denominator_;
        int64_t new_denom = static_cast<int64_t>(denominator_) * other.denominator_;

        Rational result(static_cast<int>(new_num), static_cast<int>(new_denom));
        result.Reduction();
        return result;
    }

    Rational operator-(const Rational& other) const {
        int64_t new_num = static_cast<int64_t>(numerator_) * other.denominator_ -
                          static_cast<int64_t>(other.numerator_) * denominator_;
        int64_t new_denom = static_cast<int64_t>(denominator_) * other.denominator_;

        Rational result(static_cast<int>(new_num), static_cast<int>(new_denom));
        result.Reduction();
        return result;
    }

    Rational operator*(const Rational& other) const {
        Rational result(numerator_ * other.numerator_, denominator_ * other.denominator_);
        result.Reduction();
        return result;
    }

    Rational operator/(const Rational& other) const {
        if (other.numerator_ == 0) throw std::runtime_error("Division by zero");

        Rational result(numerator_ * other.denominator_, denominator_ * other.numerator_);
        result.Reduction();
        return result;
    }

    Rational& operator+=(const Rational& rat) {
        *this = *this + rat;
        return *this;
    }

    Rational& operator-=(const Rational& rat) {
        *this = *this - rat;
        return *this;
    }

    Rational& operator*=(const Rational& rat) {
        *this = *this * rat;
        return *this;
    }

    Rational& operator/=(const Rational& rat) {
        *this = *this / rat;
        return *this;
    }

    Rational operator+() const {
        return *this;
    }

    Rational operator-() const {
        return {-numerator_, denominator_};
    }

    Rational Inv() const {
        return {denominator_, numerator_};
    }

    friend std::ostream& operator<<(std::ostream& out, const Rational& rational);
    friend std::istream& operator>>(std::istream& is, Rational& rational);

private:
    int numerator_;
    int denominator_;
};

std::ostream& operator<<(std::ostream &out, const Rational &rational) {
    if (rational.GetDenominator() == 1) {
        out << rational.GetNumerator();
    } else {
        out << rational.GetNumerator() << " / " << rational.GetDenominator();
    }
    return out;
}


inline std::istream& operator>>(std::istream& is, Rational& rational) {
    int n, d;
    char div;

    if (!(is >> n)) {
        return is;
    }

    if (!(is >> std::ws >> div)) {
        rational = Rational(n, 1);
        is.clear();
        return is;
    }

    if (div != '/') {
        rational = Rational(n, 1);
        is.unget();
        return is;
    }

    if (!(is >> d) || (d == 0)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    rational = Rational(n, d);
    return is;
}

inline auto operator<=>(const Rational& r1, const Rational& r2) {
    std::int64_t c1 = static_cast<std::int64_t>(r1.GetNumerator()) * r2.GetDenominator();
    std::int64_t c2 = static_cast<std::int64_t>(r2.GetNumerator()) * r1.GetDenominator();
    return c1 <=> c2;
}

inline bool operator==(const Rational& r1, const Rational& r2) {
    return (r1 <=> r2) == 0;
}
