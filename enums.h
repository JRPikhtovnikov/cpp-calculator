#pragma once

enum class Operation {
    MULTIPLICATION,
    DIVISION,
    SUBTRACTION,
    ADDITION,
    POWER,
};

enum class ControlKey {
    EQUALS,
    CLEAR,
    MEM_SAVE,
    MEM_LOAD,
    MEM_CLEAR,
    PLUS_MINUS,
    BACKSPACE,
    EXTRA_KEY,
};

enum class ControllerType {
    DOUBLE,
    FLOAT,
    UINT8_T,
    INT,
    INT64_T,
    SIZE_T,
    RATIONAL
};
