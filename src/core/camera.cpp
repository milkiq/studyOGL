#include "camera.h"

Camera::Camera() {}

Camera::~Camera() {}

void Camera::set_fovY(float fovY) {
    this->fovY = fovY;
}

void Camera::set_pos(glm::vec3 global_position) {
    this->global_position = global_position;
}

void Camera::set_clip(float near_clip, float far_clip) {
    this->near_clip = near_clip;
    this->far_clip = far_clip;
}

void Camera::look_at(glm::vec3 target, glm::vec3 up) {
    this->up = up;
    global_rotation = glm::quatLookAt(glm::normalize(target - global_position), up);
}

float Camera::get_fovY() {
    return fovY;
}

glm::mat4 Camera::get_view_matrix() {
    glm::mat4 inv_view_matrix = glm::translate(glm::mat4(1.0f), global_position) * glm::mat4_cast(global_rotation);
    return glm::inverse(inv_view_matrix);
}

glm::mat4 Camera::get_projection_matrix(float aspect) {
    return glm::perspective(fovY, aspect, near_clip, far_clip);
}
