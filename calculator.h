#pragma once

#include <optional>
#include <cmath>
#include <string>
#include <type_traits>
#include "rational.h"
#include "pow.h"

using Error = std::string;

template<typename Number>
class Calculator {
public:
    Calculator() = default;

    void Set(Number value) {
        mem_ = value;
    }

    Number GetNumber() const {
        return mem_.value_or(Number{});
    }

    std::optional<Error> Add(Number value) {
        mem_ = GetNumber() + value;
        return std::nullopt;
    }

    std::optional<Error> Sub(Number value) {
        if constexpr (std::is_same_v<Number, int>) {
            int64_t result = static_cast<int64_t>(GetNumber()) - static_cast<int64_t>(value);
            if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min()) {
                return "Integer underflow";
            }
            mem_ = static_cast<int>(result);
        } else {
            mem_ = GetNumber() - value;
        }
        return std::nullopt;
    }

    std::optional<Error> Mul(Number value) {
        mem_ = GetNumber() * value;
        return std::nullopt;
    }

    std::optional<Error> Div(Number value) {
        if constexpr (std::is_integral_v<Number> || std::is_same_v<Number, Rational>) {
            if (value == 0) {
                return "Division by zero";
            }
        }
        mem_ = GetNumber() / value;
        return std::nullopt;
    }

    std::optional<Error> Pow(Number exponent) {
        if (GetNumber() == 0 && exponent == 0) {
            return "Zero power to zero";
        }

        if constexpr (std::is_integral_v<Number>) {
            if (exponent < 0) {
                return "Integer negative power";
            }
            mem_ = IntegerPow(GetNumber(), exponent);
        }
        else if constexpr (std::is_floating_point_v<Number>) {
            mem_ = std::pow(GetNumber(), exponent);
        }
        else if constexpr (std::is_same_v<Number, Rational>) {
            if (exponent.GetDenominator() != 1) {
                return "Fractional power is not supported";
            }
            mem_ = ::Pow(GetNumber(), Rational(exponent.GetNumerator(), 1));
        }
        return std::nullopt;
    }

    void Save() {
        saved_mem_ = mem_;
    }

    void Load() {
        if (saved_mem_) {
            mem_ = saved_mem_;
        }
    }

    bool GetHasMem() const {
        return saved_mem_.has_value();
    }

private:
    std::optional<Number> mem_;
    std::optional<Number> saved_mem_;
};

