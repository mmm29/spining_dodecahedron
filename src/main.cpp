#include <map>

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "engine/engine.h"

#include "engine/matrix_transform.h"

static sf::Color ColorToSfmlColor(const Color &color) {
    return sf::Color(color.r, color.g, color.b, color.a);
}

class Renderer {
public:
    explicit Renderer(sf::RenderWindow &window) : window_(window) {}

    void Render(draw::DrawList *draw_list) {
        for (const draw::Primitive &primitive : draw_list->primitives) {
            static const std::map<draw::PrimitiveType, sf::PrimitiveType> primitive_type_assoc = {
                    {draw::kPrimitiveLine,     sf::PrimitiveType::Lines},
                    {draw::kPrimitiveTriangle, sf::PrimitiveType::Triangles}
            };

            assert(primitive_type_assoc.count(primitive.type) != 0);

            sf::PrimitiveType primitive_type = primitive_type_assoc.at(primitive.type);
            sf::VertexArray vertices(primitive_type, primitive.vertices.size());

            for (size_t i = 0; i < primitive.vertices.size(); i++) {
                const draw::Vertex &vertex = primitive.vertices[i];
                vertices[i] = sf::Vertex{sf::Vector2f(vertex.position[0], vertex.position[1]),
                                         ColorToSfmlColor(vertex.color)};
            }

            window_.draw(vertices);
        }
    }

private:
    sf::RenderWindow &window_;
};

static sf::Vector2i GetCenterPosition(sf::RenderWindow &window) {
    return sf::Vector2i(static_cast<int>(window.getSize().x / 2), static_cast<int>(window.getSize().y / 2));
}

static void SetMouseInCenter(sf::RenderWindow &window) {
    sf::Mouse::setPosition(GetCenterPosition(window), window);
}

static sf::Vector2i GetMouseMovement(sf::RenderWindow &window) {
    return sf::Mouse::getPosition(window) - GetCenterPosition(window);
}

class CameraController {
public:
    explicit CameraController(Engine *engine) : engine_(engine) {}

    void HandleMouseMovement(int x_offset, int y_offset) {
        auto camera = engine_->GetActiveCamera();
        assert(camera && "No active camera.");

        Vector2 rotation_angeles_change(x_offset, -y_offset);

        static const float kDefaultCameraRotation = Radians(9.f / 50.f);
        rotation_angeles_change *= kDefaultCameraRotation * mouse_sensitivity_;

        camera->SetRotationAngles(camera->GetRotationAngles() + rotation_angeles_change);
    }

    void Update(float time_elapsed) {
        auto camera = engine_->GetActiveCamera();
        assert(camera && "No active camera.");

        Vector3 direction = camera->GetDirection();
        assert(direction.IsNormalized() && "Rotation vector is not normalized.");

        Vector3 offset;

        // Forward
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            offset += direction;

        // Backward
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            offset -= direction;

        // Left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            offset -= Vector3(direction[2], 0, -direction[0]).Normalize();

        // Right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            offset += Vector3(direction[2], 0, -direction[0]).Normalize();

        // Up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            offset += Vector3(0, 1, 0);

        // Down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            offset -= Vector3(0, 1, 0);

        if (!offset.IsZero()) {
            offset *= moving_speed_ * time_elapsed;

            // Fast
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                offset *= fast_moving_speed_multiplier_;

            camera->SetPosition(camera->GetPosition() + offset);
        }
    }

private:
    Engine *const engine_;

private:
    float mouse_sensitivity_ = 1.f;

    float moving_speed_ = 2.f;
    float fast_moving_speed_multiplier_ = 3.f;
};

class Menu {
public:
    explicit Menu(sf::RenderWindow &window) : window_(window) {}

    void Initialize() {
        ImGui::SFML::Init(window_);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    }

    void Shutdown() {
        ImGui::SFML::Shutdown();
    }

    struct DrawData {
        sf::Color *window_background_color;
        Engine *engine;
    };

    void Draw(DrawData *data) {
        if (!menu_active_)
            return;

        ImGui::Begin("Menu");

        float background_color_tmp[3] = {
                static_cast<float>(data->window_background_color->r) / 255.f,
                static_cast<float>(data->window_background_color->g) / 255.f,
                static_cast<float>(data->window_background_color->b) / 255.f
        };

        if (ImGui::ColorEdit3("Background color", background_color_tmp)) {
            data->window_background_color->r = static_cast<sf::Uint8>(background_color_tmp[0] * 255.f);
            data->window_background_color->g = static_cast<sf::Uint8>(background_color_tmp[1] * 255.f);
            data->window_background_color->b = static_cast<sf::Uint8>(background_color_tmp[2] * 255.f);
        }

        std::shared_ptr<Camera> active_camera = data->engine->GetActiveCamera();
        if (active_camera) {
            Vector3 camera_position = active_camera->GetPosition();

            if (ImGui::DragFloat3("Camera position", &camera_position[0], 0.01))
                active_camera->SetPosition(camera_position);

            {
                Vector2 rotation_angles = active_camera->GetRotationAngles() * Degree(1);
                if (ImGui::DragFloat2("Camera rotation", &rotation_angles[0], 1, -360, 360))
                    active_camera->SetRotationAngles(rotation_angles * Radians(1));
            }

            Vector2 rotation_angles = active_camera->GetRotationAngles();
            ImGui::InputFloat2("Rotation angles", &rotation_angles[0], "%.8f", ImGuiInputTextFlags_ReadOnly);

            Vector3 direction = active_camera->GetDirection();
            ImGui::InputFloat3("Direction", &direction[0], "%.8f", ImGuiInputTextFlags_ReadOnly);

            {
                Matrix4 rotate_around_x = matrix::RotateAroundX(rotation_angles[1]);
                Matrix4 rotate_around_y = matrix::RotateAroundY(rotation_angles[0]);
                Matrix4 rotation_matrix = rotate_around_x * rotate_around_y;

                ImGui::Text("Matrix:");
                ImGui::InputFloat4("##first", &rotation_matrix[0][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##second", &rotation_matrix[1][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##third", &rotation_matrix[2][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##forth", &rotation_matrix[3][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
            }

            {
                Matrix4 rotate_around_x = matrix::RotateAroundX(rotation_angles[1]);
                Matrix4 rotate_around_y = matrix::RotateAroundY(-rotation_angles[0]);
                Matrix4 rotation_matrix = rotate_around_x * rotate_around_y;

                ImGui::Text("Matrix:");
                ImGui::InputFloat4("##2first", &rotation_matrix[0][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##2second", &rotation_matrix[1][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##2third", &rotation_matrix[2][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##2forth", &rotation_matrix[3][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
            }

            {
                Matrix4 rotate_around_x = matrix::RotateAroundX(-rotation_angles[1]);
                Matrix4 rotate_around_y = matrix::RotateAroundY(rotation_angles[0]);
                Matrix4 rotation_matrix = rotate_around_x * rotate_around_y;

                ImGui::Text("Matrix:");
                ImGui::InputFloat4("##3first", &rotation_matrix[0][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##3second", &rotation_matrix[1][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##3third", &rotation_matrix[2][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("##3forth", &rotation_matrix[3][0], "%.8f", ImGuiInputTextFlags_ReadOnly);
            }
        }

        ImGui::End();
    }

    void Render() {
        ImGui::SFML::Render(window_);
    }

    void Update(sf::Time time_elapsed) {
        ImGui::SFML::Update(window_, time_elapsed);
    }

    void Show() {
        menu_active_ = true;
    }

    void Hide() {
        menu_active_ = false;
    }

    bool IsActive() const {
        return menu_active_;
    }

    void Toggle() {
        if (menu_active_)
            Hide();
        else
            Show();
    }

private:
    sf::RenderWindow &window_;

    bool menu_active_ = false;
};

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
    engine.Initialize(ViewPort(static_cast<float>(video_mode.width), static_cast<float>(video_mode.height)));

    CameraController camera_controller(&engine);

    Renderer renderer(window);

    sf::Color background_color = sf::Color::White;

    Menu::DrawData menu_data;
    menu_data.window_background_color = &background_color;
    menu_data.engine = &engine;

    sf::Clock delta_clock;

    while (window.isOpen()) {
        sf::Time time_elapsed = delta_clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

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
                auto camera = engine.GetActiveCamera();
                if (camera) {
                    if (event.type == sf::Event::MouseMoved) {
                        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                        sf::Vector2i center_position = GetCenterPosition(window);
                        if (mouse_position != center_position)
                            sf::Mouse::setPosition(center_position, window);

                        sf::Vector2i mouse_movement = mouse_position - center_position;
                        camera_controller.HandleMouseMovement(mouse_movement.x, mouse_movement.y);
                    } else if (event.type == sf::Event::MouseEntered) {
                        SetMouseInCenter(window);
                    }
                }
            }
        }

        // Update controllers
        menu.Update(time_elapsed);
        camera_controller.Update(time_elapsed.asSeconds());

        // Drawings
        engine.Draw();
        menu.Draw(&menu_data);

        // Rendering
        window.clear(background_color);
        renderer.Render(engine.GetDrawList());
        menu.Render();

        window.display();
    }

    menu.Shutdown();

    return 0;
}