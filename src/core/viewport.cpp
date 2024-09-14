#include "viewport.h"

Viewport* Viewport::main_viewport = nullptr;

Viewport* Viewport::get_main_viewport() {
    return Viewport::main_viewport;
}

Viewport::Viewport(float width, float height) {
    if (Viewport::main_viewport == nullptr) {
        Viewport::main_viewport = this;
    }
    this->width = width;
    this->height = height;
}

Viewport::~Viewport() {}

void Viewport::set_main_camera(Camera* camera) {
    this->main_camera = camera;
}

void Viewport::set_size(float width, float height) {
    this->width = width;
    this->height = height;
}

float Viewport::get_aspect_ratio() const {
    return this->width / this->height;
}

glm::mat4 Viewport::get_view_matrix() {
    if (this->main_camera == nullptr) {
        return glm::mat4(1.0f);
    }
    return this->main_camera->get_view_matrix();
}

glm::mat4 Viewport::get_projection_matrix() {
    if (this->main_camera == nullptr) {
        return glm::mat4(1.0f);
    }
    return this->main_camera->get_projection_matrix(this->get_aspect_ratio());
}
