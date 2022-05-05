#pragma once

#include <cstdint>
#include <array>

template<size_t rows, size_t cols>
class Matrix {
public:
    Matrix(std::initializer_list<std::initializer_list<float>> il)
            : Matrix(il, std::make_index_sequence<rows * cols>()) {}

private:
    template<size_t... FlatIs>
    Matrix(std::initializer_list<std::initializer_list<float>> il,
           std::index_sequence<FlatIs...>)
            : values_{il.begin()[FlatIs / rows].begin()[FlatIs % cols]...} {}

public:
    std::array<std::array<float, cols>, rows> values_;
};

using Matrix4x4 = Matrix<4, 4>;