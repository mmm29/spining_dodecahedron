#include <map>

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "engine/engine.h"

#include "engine/matrix_transform.h"

template<size_t Rows, size_t Cols>
void Print(const Matrix<Rows, Cols> &matrix) {
    printf("{\n");

    if (Rows == 0) {
        putchar('\n');
    } else {
        for (size_t row = 0; row < Rows; row++) {
            printf("\t");

            for (size_t col = 0; col < Cols; col++) {
                printf("%.2f", matrix[row][col]);

                if (row + 1 != Rows || col + 1 != Cols)
                    putchar(',');

                if (col + 1 != Cols)
                    putchar(' ');
            }

            putchar('\n');
        }
    }

    printf("}\n");
}

static sf::Color ColorToSfmlColor(const Color &color) {
    return sf::Color(color.r, color.g, color.b, color.a);
}

void RenderDrawList(sf::RenderWindow *window, draw::DrawList *draw_list) {
    for (const draw::Primitive &primitive : draw_list->primitives) {
        static const std::map<draw::PrimitiveType, sf::PrimitiveType> primitive_type_assoc = {
                {draw::kPrimitiveLine,     sf::PrimitiveType::Lines},
                {draw::kPrimitiveTriangle, sf::PrimitiveType::Triangles}
        };

        if (!primitive_type_assoc.count(primitive.type))
            continue; // Unknown primitive type, SKip

        sf::PrimitiveType primitive_type = primitive_type_assoc.at(primitive.type);
        sf::VertexArray vertices(primitive_type, primitive.vertices.size());

        for (size_t i = 0; i < primitive.vertices.size(); i++) {
            const draw::Vertex &vertex = primitive.vertices[i];
            vertices[i] = sf::Vertex{sf::Vector2f(vertex.position[0], vertex.position[1]),
                                     ColorToSfmlColor(vertex.color)};
        }

        window->draw(vertices);
    }
}

struct MenuData {
    sf::Color *window_background_color;
    Engine *engine;
};

void DrawMenu(MenuData *data) {
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
            Vector2 rotation_angles = active_camera->GetRotationAngles() * (180 / M_PI);
            if (ImGui::DragFloat2("Camera rotation", &rotation_angles[0], 1, -360, 360))
                active_camera->SetRotationAngles(rotation_angles * (M_PI / 180));
        }

        Vector2 rotation_angles = active_camera->GetRotationAngles();
        ImGui::InputFloat2("Rotation angles", &rotation_angles[0], "%.8f", ImGuiInputTextFlags_ReadOnly);

        Vector3 rotation = active_camera->GetRotation();
        ImGui::InputFloat3("Rotation", &rotation[0], "%.8f", ImGuiInputTextFlags_ReadOnly);

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

static sf::Vector2i GetCenterPosition(sf::RenderWindow &window) {
    return sf::Vector2i(static_cast<int>(window.getSize().x / 2), static_cast<int>(window.getSize().y / 2));
}

static void SetMouseInCenter(sf::RenderWindow &window) {
    sf::Mouse::setPosition(GetCenterPosition(window), window);
}

static sf::Vector2i GetMouseMovement(sf::RenderWindow &window) {
    return sf::Mouse::getPosition(window) - GetCenterPosition(window);
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 12;
    settings.antialiasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Spinning dodecahedron", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    if (!window.isOpen()) {
        printf("Unable to open window.\n");
        return 1;
    }

    Engine engine;
    engine.Initialize(ViewPort(1280, 720));

    sf::Color background_color = sf::Color::White;

    MenuData menu_data;
    menu_data.window_background_color = &background_color;
    menu_data.engine = &engine;

    bool menu_active = false;
    window.setMouseCursorVisible(false);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

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
                    menu_active ^= true;
                    window.setMouseCursorVisible(menu_active);
                    if (!menu_active)
                        SetMouseInCenter(window);
                } else if (event.key.code == sf::Keyboard::Key::Escape)
                    window.close();
            } else if (!menu_active) {
                auto camera = engine.GetActiveCamera();
                if (camera) {
                    if (event.type == sf::Event::MouseMoved) {
                        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                        sf::Vector2i center_position = GetCenterPosition(window);
                        if (mouse_position != center_position)
                            sf::Mouse::setPosition(center_position, window);

                        sf::Vector2i mouse_movement = mouse_position - center_position;

                        static const float mouse_sensitivity = 1.f; // TODO: use it

                        Vector2 rotation_angeles_change(mouse_movement.x, -mouse_movement.y);
                        rotation_angeles_change *= (M_PI / 1000.f);

                        camera->SetRotationAngles(camera->GetRotationAngles() + rotation_angeles_change);
                    } else if (event.type == sf::Event::MouseEntered) {
                        SetMouseInCenter(window);
                    }
                }
            }
        }

        {
            static const float moving_speed = 2.f;

            auto camera = engine.GetActiveCamera();
            if (camera) {
                Vector3 rotation = camera->GetRotation();
                assert(rotation.IsNormalized() && "Rotation vector is not normalized.");

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    camera->SetPosition(
                            camera->GetPosition() + rotation * moving_speed * time_elapsed.asSeconds());
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    camera->SetPosition(
                            camera->GetPosition() -
                            Vector3(rotation[2], 0, -rotation[0]) * moving_speed * time_elapsed.asSeconds());

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    camera->SetPosition(
                            camera->GetPosition() - rotation * moving_speed * time_elapsed.asSeconds());

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    camera->SetPosition(
                            camera->GetPosition() +
                            Vector3(rotation[2], 0, -rotation[0]) * moving_speed * time_elapsed.asSeconds());

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    camera->SetPosition(
                            camera->GetPosition() + Vector3(0, moving_speed, 0) * time_elapsed.asSeconds());

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    camera->SetPosition(
                            camera->GetPosition() + Vector3(0, -moving_speed, 0) * time_elapsed.asSeconds());
            }
        }

        ImGui::SFML::Update(window, time_elapsed);

        engine.Draw();
//        if (menu_active)
        DrawMenu(&menu_data);

        window.clear(background_color);
        RenderDrawList(&window, engine.GetDrawList());
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}