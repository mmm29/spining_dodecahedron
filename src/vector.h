#pragma once

#include <cstdint>
#include <array>

template<size_t size>
class Vector {
public:
    Vector() = default;

    Vector(std::initializer_list<float> il) : Vector(il, std::make_index_sequence<size>()) {}

private:
    template<size_t... Is>
    Vector(std::initializer_list<float> il, std::index_sequence<Is...>) : values_{il.begin()[Is]...} {}

public:
    float &operator[](ptrdiff_t index) {
        return values_[index];
    }

    const float &operator[](ptrdiff_t index) const {
        return values_[index];
    }

private:
    std::array<float, size> values_;
};

class Vector2 : public Vector<2> {
public:
    Vector2(float x, float y) : Vector<2>({x, y}) {}
};

class Vector3 : public Vector<3> {
public:
    Vector3(float x, float y, float z) : Vector<3>({x, y, z}) {}
};

class Vector4 : public Vector<4> {
public:
    Vector4(float x, float y, float z, float w) : Vector<4>({x, y, z, w}) {}
};