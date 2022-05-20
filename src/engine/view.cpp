#include "view.h"

void View::SetViewPort(const ViewPort &viewport) {
    viewport_ = viewport;
    viewport_aspect_ratio_ = viewport_.GetAspectRatio();
}

const ViewPort &View::GetViewPort() const {
    return viewport_;
}

void View::SetCamera(const std::shared_ptr<Camera> &camera) {
    camera_ = camera;
}

void View::RemoveCamera() {
    camera_ = nullptr;
}

const std::shared_ptr<Camera> &View::GetCamera() const {
    return camera_;
}

bool View::IsCameraAttached() const {
    return camera_ != nullptr;
}

void View::UpdateMatrices() {
    if (!IsCameraAttached())
        return;

    data_.view_matrix = camera_->ComputeViewMatrix();
    data_.projection_matrix = camera_->ComputeProjectionMatrix();
    data_.view_projection_matrix = data_.projection_matrix * data_.view_matrix;
}

const ViewData &View::GetViewData() const {
    return data_;
}