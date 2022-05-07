//#include <cassert>
//
//#include "screen.h"
//
//SFMLScreen::SFMLScreen(const WindowPreferences &window_preferences)
//        : size_(window_preferences.window_size),
//          background_color_(window_preferences.background_color),
//          title_(window_preferences.window_title),
//          window_style_(window_preferences.style) {}
//
//bool SFMLScreen::Open() {
//    sf::ContextSettings settings;
//    settings.depthBits = 12;
//    settings.antialiasingLevel = 1;
//
//    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(size_.x, size_.y), title_, window_style_, settings);
//    window_->setVerticalSyncEnabled(true);
//    window_->setFramerateLimit(60);
//
//    return window_->isOpen();
//}
//
//void SFMLScreen::Clear() {
//    window_->clear(background_color_);
//}
//
//bool SFMLScreen::IsOpen() const {
//    return window_->isOpen();
//}
//
//void SFMLScreen::Display() {
//    window_->display();
//}
//
//void SFMLScreen::Close() {
//    window_->close();
//    window_ = nullptr;
//}
//
//void SFMLScreen::PollEvents() {
//    sf::Event event;
//    while (window_->pollEvent(event)) {
//        if (event.type == sf::Event::Closed)
//            window_->close();
//    }
//}
//
//static sf::Color ColorToSfmlColor(const sf::Color &color) {
//    return sf::Color(color.r, color.g, color.b, color.a);
//}
//
//void SFMLScreen::DrawLine(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Color &color, float thickness) {
//    sf::Color sfml_color = ColorToSfmlColor(color);
//
//    if (thickness == 1.f) {
//        sf::Vertex line[] = {
//                sf::Vertex({a[0], a[1]}, sfml_color),
//                sf::Vertex({b[0], b[1]}, sfml_color)
//        };
//
//        window_->draw(line, 2, sf::Lines);
//    } else {
//        // TODO: implement
//        assert(0);
//    }
//}
//
//void SFMLScreen::DrawTriangle(const Triangle &triangle) {
//    sf::Color sfml_color = ColorToSfmlColor(triangle.color);
//
//    sf::Vertex tris[3] = {
//            sf::Vertex({triangle.positions[0][0], triangle.positions[0][1]}, sfml_color),
//            sf::Vertex({triangle.positions[1][0], triangle.positions[1][1]}, sfml_color),
//            sf::Vertex({triangle.positions[2][0], triangle.positions[2][1]}, sfml_color)
//    };
//
//    window_->draw(tris, 3, sf::Triangles);
//
//    if (triangle.outline != 0.f) {
//        sf::Vertex lines[4] = {
//                sf::Vertex({triangle.positions[0][0], triangle.positions[0][1]}, sf::Color(0, 0, 0, 255)),
//                sf::Vertex({triangle.positions[1][0], triangle.positions[1][1]}, sf::Color(0, 0, 0, 255)),
//                sf::Vertex({triangle.positions[2][0], triangle.positions[2][1]}, sf::Color(0, 0, 0, 255)),
//                sf::Vertex({triangle.positions[0][0], triangle.positions[0][1]}, sf::Color(0, 0, 0, 255)),
//        };
//
//        window_->draw(lines, 4, sf::LineStrip);
//    }
//}