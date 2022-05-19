#include <imgui.h>
#include <imgui-SFML.h>

#include "engine/engine.h"
#include "engine/math/matrix_transform.h"

#include "menu.h"

void Menu::Initialize() {
    ImGui::SFML::Init(window_);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
}

void Menu::Shutdown() {
    ImGui::SFML::Shutdown();
}

void Menu::Draw(DrawData *data) {
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

void Menu::Render() {
    ImGui::SFML::Render(window_);
}

void Menu::Update(sf::Time time_elapsed) {
    ImGui::SFML::Update(window_, time_elapsed);
}

void Menu::ProcessEvent(sf::Event &event) {
    ImGui::SFML::ProcessEvent(event);
}

void Menu::Show() {
    menu_active_ = true;
}

void Menu::Hide() {
    menu_active_ = false;
}

bool Menu::IsActive() const {
    return menu_active_;
}

void Menu::Toggle() {
    if (menu_active_)
        Hide();
    else
        Show();
}