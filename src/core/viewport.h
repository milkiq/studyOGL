#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "math.h"
#include "../node/camera.h"
#include "../node/light3d.h"

class Viewport {
private:
    static Viewport* main_viewport;

    float width = 0.0f;
    float height = 0.0f;
    Camera* main_camera = nullptr;

    Viewport(const Viewport& viewport) = delete;
    Viewport& operator=(const Viewport& viewport) = delete;

public:
    Light3D* light;

    static Viewport* get_main_viewport();

    Viewport(float width, float height);
    ~Viewport();

    void set_main_camera(Camera* camera);
    void set_size(float width, float height);
    float get_aspect_ratio() const;
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();
};

#endif // VIEWPORT_H
