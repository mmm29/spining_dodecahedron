#include <cassert>

#include <imgui.h>
#include <imgui-SFML.h>

#include "engine/engine.h"

#include "menu.h"

void Menu::Initialize() {
    assert(ImGui::SFML::Init(window_));
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

static void DrawObjectProperties(Object *object) {
    // Position
    Vector3 world_position = object->GetWorldPosition();
    if (ImGui::DragFloat3("World position", &world_position[0], 0.01))
        object->SetWorldPosition(world_position);

    // Position
    Vector3 relative_position = object->GetRelativePosition();
    if (ImGui::DragFloat3("Relative position", &relative_position[0], 0.01))
        object->SetRelativePosition(relative_position);

    // Rotation
    Vector2 rotation_angles = object->GetRotationAngles() * Degree(1);
    if (ImGui::DragFloat2("Rotation", &rotation_angles[0], 1, -360, 360))
        object->SetRotationAngles(rotation_angles * Radians(1));

    // Direction forward
    Vector3 direction = object->GetDirectionForward();
    ImGui::InputFloat3("Direction forward", &direction[0], "%.8f", ImGuiInputTextFlags_ReadOnly);
}

void Menu::Draw(DrawData *data) {
    if (!menu_active_)
        return;

    static bool show_demo = false;

    if (show_demo)
        ImGui::ShowDemoWindow(&show_demo);

    ImGui::Begin("Menu", &menu_active_);

    ImGui::Text("FPS: %f (%f ms/frame)", ImGui::GetIO().Framerate, 1000.f / ImGui::GetIO().Framerate);

    ImGui::Checkbox("Show demo", &show_demo);

    ImGui::ColorEdit3("Background color", data->window_background_color);

    if (ImGui::CollapsingHeader("Settings")) {
        const auto show_triangles_settings = [&](DebugSettings::TriangleSettings &triangle_settings) {
            ImGui::Checkbox("Show outlines", &triangle_settings.outlines.show);
            if (triangle_settings.outlines.show)
                ImGui::ColorEdit4("Outlines color", &triangle_settings.outlines.color);


            ImGui::Checkbox("Show normals", &triangle_settings.normals.show);
            if (triangle_settings.normals.show) {
                ImGui::ColorEdit4("Normals color", &triangle_settings.normals.color);
                ImGui::DragFloat("Normals lengths", &triangle_settings.normals.length, 0.1, 0, 100);
            }
        };

        Settings *settings = data->engine->AccessSettings();

        // Temporarily doesn't work
//        if (ImGui::TreeNode("Triangles")) {
//            show_triangles_settings(settings->debug.triangle);
//            ImGui::TreePop();
//        }

        if (ImGui::TreeNode("Clipped triangles")) {
            show_triangles_settings(settings->debug.clipped_triangle);
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader("Objects")) {
        const std::list<std::shared_ptr<RigidBody>> &bodies = data->engine->GetWorld()->ListObjects();

        for (const std::shared_ptr<RigidBody> &body : bodies) {
            if (ImGui::TreeNode(body.get(), "body 0x%p", body.get())) {
                bool visible = body->IsVisible();
                if (ImGui::Checkbox("Visible", &visible))
                    body->SetVisible(visible);

                Color color = body->GetColor();
                if (ImGui::ColorEdit4("Color", &color))
                    body->SetColor(color);

                Vector2 rotation_velocity = body->GetRotationVelocity() * (180 / M_PI);
                if (ImGui::DragFloat2("Rotation velocity", &rotation_velocity[0], 1, -180, 180))
                    body->SetRotationVelocity(rotation_velocity * (M_PI / 180));

                DrawObjectProperties(body.get());
                ImGui::TreePop();
            }
        }
    }

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
                std::shared_ptr<Camera> camera = data->engine->CreateCamera();
                data->cameras[camera_name] = CameraInfo{.camera = camera};
            }
            ImGui::EndPopup();
        }

        std::shared_ptr<Camera> active_camera = data->engine->GetActiveCamera();

        for (auto it = data->cameras.begin(); it != data->cameras.end();) {
            CameraInfo &camera_info = it->second;
            const std::shared_ptr<Camera> &camera = camera_info.camera;

            const bool is_this_camera_active = camera == active_camera;
            bool delete_camera = false;

            if (ImGui::TreeNode(it->first.c_str())) {
                ImGui::Text("Name: %s", it->first.c_str());

                // Active
                ImGui::Text("Is active: %s", is_this_camera_active ? "Yes" : "No");
                if (!is_this_camera_active && ImGui::Button("Set active"))
                    data->engine->SetActiveCamera(camera);

                // Attach/detach
                ImGui::Text("Is attached: %s", camera->IsAttached() ? "Yes" : "No");
                if (camera->IsAttached()) {
                    if (ImGui::Button("Detach"))
                        camera->Detach();
                    else {
                        float attach_distance = camera->GetAttachDistance();
                        if (ImGui::DragFloat("Attach distance", &attach_distance, 0.1))
                            camera->SetAttachDistance(attach_distance);
                    }
                }

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

                DrawObjectProperties(camera.get());

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