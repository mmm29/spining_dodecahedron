#include <imgui.h>
#include <imgui-SFML.h>

#include "engine/engine.h"

#include "menu.h"

void Menu::Initialize() {
    ImGui::SFML::Init(window_);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
}

void Menu::Shutdown() {
    ImGui::SFML::Shutdown();
}

namespace ImGui {
    // Color editor for SFML color
    static bool ColorEdit3(const char *label, sf::Color *color) {
        float color_float_tmp[3] = {
                static_cast<float>(color->r) / 255.f,
                static_cast<float>(color->g) / 255.f,
                static_cast<float>(color->b) / 255.f
        };

        bool result = ImGui::ColorEdit3(label, color_float_tmp);

        if (result) {
            color->r = static_cast<sf::Uint8>(color_float_tmp[0] * 255.f);
            color->g = static_cast<sf::Uint8>(color_float_tmp[1] * 255.f);
            color->b = static_cast<sf::Uint8>(color_float_tmp[2] * 255.f);
        }

        return result;
    }

    // Color editor for engine color
    static bool ColorEdit4(const char *label, Color *color) {
        float color_float_tmp[4] = {
                static_cast<float>(color->r) / 255.f,
                static_cast<float>(color->g) / 255.f,
                static_cast<float>(color->b) / 255.f,
                static_cast<float>(color->a) / 255.f
        };

        bool result = ImGui::ColorEdit4(label, color_float_tmp);

        if (result) {
            color->r = static_cast<sf::Uint8>(color_float_tmp[0] * 255.f);
            color->g = static_cast<sf::Uint8>(color_float_tmp[1] * 255.f);
            color->b = static_cast<sf::Uint8>(color_float_tmp[2] * 255.f);
            color->a = static_cast<sf::Uint8>(color_float_tmp[3] * 255.f);
        }

        return result;
    }
}

void Menu::Draw(DrawData *data) {
    if (!menu_active_)
        return;

    static bool show_demo = false;

    if (show_demo)
        ImGui::ShowDemoWindow(&show_demo);

    ImGui::Begin("Menu", &menu_active_);

    ImGui::Checkbox("Show demo", &show_demo);

    ImGui::ColorEdit3("Background color", data->window_background_color);

    if (ImGui::CollapsingHeader("Cameras")) {
        // Create camera
        static char camera_name[64] = {};
        if (ImGui::Button("Create camera")) {
            camera_name[0] = '\0';
            ImGui::OpenPopup("create_camera_popup");
        }

        if (ImGui::BeginPopupContextItem("create_camera_popup")) {
            ImGui::InputText("Name", camera_name, sizeof(camera_name));
            if (ImGui::Button("Create")) {
                auto camera = data->engine->CreateCamera();
                data->cameras[camera_name] = CameraInfo{.camera = camera};
            }
            ImGui::EndPopup();
        }

        auto active_camera = data->engine->GetActiveCamera();

        for (auto it = data->cameras.begin(); it != data->cameras.end();) {
            CameraInfo &camera_info = it->second;
            auto camera = camera_info.camera;

            const bool is_this_camera_active = camera == active_camera;
            bool delete_camera = false;

            if (ImGui::TreeNode(it->first.c_str())) {
                ImGui::Text("Name: %s", it->first.c_str());

                // Active
                ImGui::Text("Is active: %s", is_this_camera_active ? "Yes" : "No");
                if (!is_this_camera_active && ImGui::Button("Set active"))
                    data->engine->SetActiveCamera(camera);

                // Delete
                if (data->cameras.size() > 1) {
                    if (ImGui::Button("Delete"))
                        delete_camera = true;
                }

                // Viewing frustum
                ImGui::Checkbox("Show viewing frustum", &camera_info.show_viewing_frustum);
                if (camera_info.show_viewing_frustum)
                    ImGui::ColorEdit4("Viewing frustum color", &camera_info.viewing_frustum_color);

                // FOV
                float fov = Degree(camera->GetFieldOfView());
                if (ImGui::DragFloat("FOV", &fov, 0.5, 1, 180))
                    camera->SetFieldOfView(Radians(fov));

                // Near Z
                float near_z = camera->GetNearZ();
                if (ImGui::DragFloat("Near Z", &near_z, 0.05, 0.001))
                    camera->SetNearZ(near_z);

                // Far Z
                float far_z = camera->GetFarZ();
                if (ImGui::DragFloat("Far Z", &far_z, 0.05, 0.001))
                    camera->SetFarZ(far_z);

                // Position
                Vector3 camera_position = camera->GetPosition();
                if (ImGui::DragFloat3("Position", &camera_position[0], 0.01))
                    camera->SetPosition(camera_position);

                // Rotation
                Vector2 rotation_angles = camera->GetRotationAngles() * Degree(1);
                if (ImGui::DragFloat2("Rotation", &rotation_angles[0], 1, -360, 360))
                    camera->SetRotationAngles(rotation_angles * Radians(1));

                // Direction forward
                Vector3 direction = camera->GetDirectionForward();
                ImGui::InputFloat3("Direction forward", &direction[0], "%.8f", ImGuiInputTextFlags_ReadOnly);

                ImGui::TreePop();
            }

            if (delete_camera) {
                it = data->cameras.erase(it);
                if (is_this_camera_active)
                    data->engine->SetActiveCamera(it->second.camera);
            } else
                ++it;
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