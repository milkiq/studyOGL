#ifndef CAMERA_H
#define CAMERA_H

#include "node3d.h"
#include "../core/math.h"

class Camera : public Node3D {
private:
    glm::vec3 up = glm::vec3(0,1,0);

    float fovY = MY_PI_4;
    float near_clip = 0.1f;
    float far_clip = 100.0f;

public:
    Camera();
    ~Camera();

    void set_fovY(float fovY);
    void set_clip(float near_clip, float far_clip);
    void look_at(glm::vec3 target, glm::vec3 up);
    float get_fovY();
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix(float aspect);
};

#endif
