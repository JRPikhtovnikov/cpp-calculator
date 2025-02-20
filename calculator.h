#pragma once

#include <string>
#include <optional>

using Number = double;

enum class Operation {
    NO_OPERATION,
    MULTIPLICATION,
    DIVISION,
    SUBTRACTION,
    ADDITION,
    POWER,
};

class Calculator {
public:
    Calculator();

    void Set(Number n);
    Number GetNumber() const;
    void Add(Number n);
    void Sub(Number n);
    void Div(Number n);
    void Mul(Number n);
    void Pow(Number n);

    void SaveToMemory();
    Number LoadFromMemory() const;
    bool IsMemoryEmpty() const;
    void ClearMemory();

    std::string GetNumberRepr() const;

private:
    Number current_result_;
    std::optional<Number> memory_cell_;
};
