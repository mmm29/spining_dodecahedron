#include <SFML/Graphics.hpp>

#include "screen.h"

int main() {
    WindowPreferences window_preferences;

    window_preferences.window_size = sf::Vector2f(1280, 720);
    window_preferences.background_color = sf::Color(255, 255, 255);
    window_preferences.window_title = "Dodecahedron";

    Screen screen(window_preferences);

    if (!screen.Open()) {
        printf("Cannot open window.\n");
        return 1;
    }

    while (screen.IsOpen()) {
        screen.PollEvents();

        screen.Clear();
        screen.DrawLine(Vector2(0, 0), Vector2(50, 50), Color::Red());
        screen.DrawTriangle(Triangle(Vector2(200, 50), Vector2(100, 150), Vector2(300, 150), Color::Blue(), 1));
        screen.Display();
    }

    screen.Close();

//    std::array<float, 2> arr({7, 7});

//    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }

    return 0;
}