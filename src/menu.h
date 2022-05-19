#pragma once

#include <SFML/Graphics.hpp>

class Engine;

class Menu {
public:
    explicit Menu(sf::RenderWindow &window) : window_(window) {}

    void Initialize();

    void Shutdown();

    struct DrawData {
        sf::Color *window_background_color;
        Engine *engine;
    };

    void Draw(DrawData *data);

    void Render();

    void Update(sf::Time time_elapsed);

    void ProcessEvent(sf::Event &event);

    void Show();

    void Hide();

    bool IsActive() const;

    void Toggle();

private:
    sf::RenderWindow &window_;

    bool menu_active_ = false;
};