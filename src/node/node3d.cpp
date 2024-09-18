#include "node3d.h"
#include "../core/viewport.h"

Node3D::Node3D() {}

Node3D::~Node3D() {}

glm::mat4 Node3D::get_model_matrix() {
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rotation_matrix = glm::toMat4(rotation);
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;
    return model_matrix;
}

Node3D::Matrices Node3D::get_matrices() {
    Matrices matrices = {
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat3(1.0f)
    };

    Viewport *vp = Viewport::get_main_viewport();
    if (vp != nullptr) {
        matrices.view_matrix = vp->get_view_matrix();
        matrices.projection_matrix = vp->get_projection_matrix();
    }

    glm::mat4 model_matrix = this->get_model_matrix();
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
    
    matrices.model_matrix = model_matrix;
    matrices.normal_matrix = normal_matrix;
    return matrices;
}
