#include "calculator.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

Calculator::Calculator() : current_result_(0), memory_cell_(std::nullopt) {}

void Calculator::Set(Number n) {
    current_result_ = n;
}

Number Calculator::GetNumber() const {
    return current_result_;
}

void Calculator::Add(Number n) {
    if (std::isinf(current_result_)) {
        return;
    }
    current_result_ += n;
}

void Calculator::Sub(Number n) {
    if (std::isinf(current_result_)) {
        return;
    }
    current_result_ -= n;
}

void Calculator::Div(Number n) {
    if (n == 0) {
        current_result_ = std::numeric_limits<Number>::infinity();
    } else {
        current_result_ /= n;
    }
}

void Calculator::Mul(Number n) {
    current_result_ *= n;
}

void Calculator::Pow(Number n) {
    current_result_ = std::pow(current_result_, n);
}

void Calculator::SaveToMemory() {
    memory_cell_ = current_result_;
}

Number Calculator::LoadFromMemory() const {
    if (!memory_cell_.has_value()) {
        throw std::runtime_error("Memory is empty");
    }
    return *memory_cell_;
}

bool Calculator::IsMemoryEmpty() const {
    return !memory_cell_.has_value();
}

void Calculator::ClearMemory() {
    memory_cell_.reset();
}

std::string Calculator::GetNumberRepr() const {
    return std::to_string(current_result_);
}
