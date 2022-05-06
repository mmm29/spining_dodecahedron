#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "vector.h"

#include "drawer.h"

struct WindowPreferences {
    sf::Vector2f window_size;

    std::string window_title;

    sf::Color background_color = sf::Color(0, 0, 0);

    sf::Uint32 style = sf::Style::Default;
};

class Screen : public Drawer {
public:
    explicit Screen(const WindowPreferences &window_preferences);

    bool Open();

    void Close();

    void PollEvents();

    void Clear();

    bool IsOpen() const;

    void Display();

    const sf::Vector2f &GetSize() const {
        return size_;
    }

    float GetAspectRatio() const {
        return size_.x / size_.y;
    }

    // Drawing methods
    void DrawLine(const Vector2 &a, const Vector2 &b, const Color &color, float thickness = 1.f) override;

    void DrawTriangle(const Triangle &triangle) override;

private:
    const sf::Vector2f size_;
    const std::string title_;
    const sf::Color background_color_;
    const sf::Uint32 window_style_;

private:
    std::unique_ptr<sf::RenderWindow> window_;
};