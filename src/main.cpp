#include <fstream>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "engine/engine.h"

#include "engine/platform/sfml/render/sfml_renderer.h"

#include "camera_controller.h"
#include "menu.h"

#include "engine/math/matrix_transform.h"
#include "engine/obj_parser.h"
#include "engine/rigid_body.h"

static sf::Vector2i GetCenterPosition(sf::RenderWindow &window) {
    return sf::Vector2i(static_cast<int>(window.getSize().x / 2), static_cast<int>(window.getSize().y / 2));
}

static void SetMouseInCenter(sf::RenderWindow &window) {
    sf::Mouse::setPosition(GetCenterPosition(window), window);
}

static sf::Vector2i GetMouseMovement(sf::RenderWindow &window) {
    return sf::Mouse::getPosition(window) - GetCenterPosition(window);
}

std::unordered_map<std::string, CameraInfo> *cameras; // TODO: remove it

std::string ReadFile(const char *filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::ate);

    if (!file.is_open())
        return std::string();

    std::streamsize file_size = file.tellg();
    file.seekg(0);

    std::string data(file_size, '\0');
    file.read(&data[0], file_size);

    file.close();

    return data;
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 12;
    settings.antialiasingLevel = 1;

    sf::VideoMode video_mode(1280, 720);
    sf::RenderWindow window(video_mode, "Spinning dodecahedron", sf::Style::Default, settings);

    if (!window.isOpen()) {
        printf("Unable to open window.\n");
        return 1;
    }

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    Menu menu(window);
    menu.Initialize();

    Engine engine;
    ViewPort viewport(static_cast<float>(video_mode.width), static_cast<float>(video_mode.height));

    auto sfml_renderer = std::make_shared<render::SFMLRenderer>();
    sfml_renderer->SetRenderTarget(&window);

    std::shared_ptr<render::Renderer> renderer = sfml_renderer;

    engine.Initialize(viewport, renderer);

    auto camera_controller = std::make_shared<CameraController>();
    engine.AttachController(camera_controller);

    sf::Color background_color = sf::Color::White;

    Menu::DrawData menu_data;
    menu_data.window_background_color = &background_color;
    menu_data.engine = &engine;
    menu_data.cameras["main_camera"] = CameraInfo{
            .camera = engine.GetActiveCamera()
    };
    cameras = &menu_data.cameras; // TODO: remove it

    {
        std::string cube_obj_text = ReadFile("obj/armadillo.obj");
        assert(!cube_obj_text.empty());

        std::shared_ptr<Mesh> cube_mesh = ObjParser::Parse(cube_obj_text);
        assert(cube_mesh);

        cube_mesh->Transform(matrix::Scale(0.05f));

        auto cube = std::make_shared<RigidBody>();
        cube->SetMesh(cube_mesh);
        cube->SetWorldPosition(Vector3(10, 10, 10));

        engine.GetWorld()->AddObject(cube);
        engine.GetActiveCamera()->AttachTo(cube);
    }

    sf::Clock delta_clock;

    while (window.isOpen()) {
        sf::Time time_elapsed = delta_clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            menu.ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::F2) {
                    menu.Toggle();
                    window.setMouseCursorVisible(menu.IsActive());
                    if (!menu.IsActive())
                        SetMouseInCenter(window);
                } else if (event.key.code == sf::Keyboard::Key::Escape)
                    window.close();
            } else if (!menu.IsActive()) {
                std::shared_ptr<Camera> camera = engine.GetActiveCamera();
                if (camera) {
                    if (event.type == sf::Event::MouseMoved) {
                        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                        sf::Vector2i center_position = GetCenterPosition(window);
                        if (mouse_position != center_position)
                            sf::Mouse::setPosition(center_position, window);

                        sf::Vector2i mouse_movement = mouse_position - center_position;
                        camera_controller->HandleMouseMovement(mouse_movement.x, mouse_movement.y);
                    } else if (event.type == sf::Event::MouseEntered) {
                        SetMouseInCenter(window);
                    }
                }
            }
        }

        // Update controllers
        engine.Update(time_elapsed.asSeconds());
        menu.Update(time_elapsed);

        // Drawings
        window.clear(background_color);
        engine.Draw();

        menu.Draw(&menu_data);
        menu.Render();

        window.display();
    }

    menu.Shutdown();

    return 0;
}