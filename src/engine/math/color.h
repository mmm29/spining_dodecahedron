#pragma once

#include <cstdint>

struct Color {
public:
    static Color Red() {
        return Color::RGBA(255, 0, 0);
    }

    static Color Green() {
        return Color::RGBA(0, 255, 0);
    }

    static Color Blue() {
        return Color::RGBA(0, 0, 255);
    }

    static Color White() {
        return Color::RGBA(255, 255, 255);
    }

    static Color Black() {
        return Color::RGBA(0, 0, 0);
    }

    static Color Transparent() {
        return Color::RGBA(0, 0, 0, 0);
    }

    static Color RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        return Color{r, g, b, a};
    }

    Color() = default;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};