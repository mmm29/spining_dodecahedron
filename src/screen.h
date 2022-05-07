#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

struct WindowPreferences {
    sf::Vector2f window_size;

    std::string window_title;

    sf::Color background_color = sf::Color(0, 0, 0);

    sf::Uint32 style = sf::Style::Default;
};

class SFMLScreen {
public:
    explicit SFMLScreen(const WindowPreferences &window_preferences);

    bool Open();

    void Close();

    void PollEvents();

    void Clear();

    bool IsOpen() const;

    void Display();

private:
    // Drawing

private:
    const sf::Vector2f size_;
    const std::string title_;
    const sf::Color background_color_;
    const sf::Uint32 window_style_;

private:
    std::unique_ptr<sf::RenderWindow> window_;
};