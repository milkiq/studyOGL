#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/math.h"
#include "core/shader.h"
#include "core/mesh.h"
#include "core/viewport.h"
#include "node/camera.h"
#include "node/mesh3d.h"
#include "node/light3d.h"
#include "resource/image.h"
#include "resource/box_model.h"

static unsigned char box_vert_spv_data[] = {
    #include "triangle.vert.spv.h"
};

static unsigned char box_frag_spv_data[] = {
    #include "triangle.frag.spv.h"
};

static unsigned char light_mesh_vert_spv_data[] = {
    #include "light_mesh.vert.spv.h"
};

static unsigned char light_mesh_frag_spv_data[] = {
    #include "light_mesh.frag.spv.h"
};

using namespace std;

float vp_width = 800.0f;
float vp_height = 600.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    vp_width = width;
    vp_height = height;
    Viewport::get_main_viewport()->set_size(width, height);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

unsigned int gen_tex(const char* filename) {
    Image image(filename);
    if (image.get_data() == nullptr) {
        return -1;
    }

    GLint tex_format = image.get_channels() == 4 ? GL_RGBA : GL_RGB;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, tex_format, image.get_width(), image.get_height(), 0, tex_format, GL_UNSIGNED_BYTE, image.get_data());

    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

GLFWwindow* create_window(const char* title, int width, int height, GLFWframebuffersizefun size_callback) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifndef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return nullptr;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, size_callback);

    return window;
}

int main(int argc, char** argv)
{

    GLFWwindow* window = create_window("LearnOpenGL", vp_width, vp_height, framebuffer_size_callback);

    Viewport vp(vp_width, vp_height);

    if (window == nullptr) {
        return -1;
    }

    Camera main_camera;
    main_camera.position = glm::vec3(1.0f, 1.0f, 2.0f);
    main_camera.set_clip(0.1f, 100.0f);
    main_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    vp.set_main_camera(&main_camera);

    BoxModel box_model;
    Mesh box_mesh(*box_model.get_vertices(), *box_model.get_indices());
    if (box_mesh.is_valid() == false) {
        return -1;
    }

// box_mesh start

#ifndef __APPLE__
    Shader box_shader(
        (const char*)box_vert_spv_data,
        sizeof(box_vert_spv_data),
        (const char*)box_frag_spv_data,
        sizeof(box_frag_spv_data)
    );
#else
    Shader box_shader("shaders/triangle.vert", "shaders/triangle.frag");
#endif

    if (box_shader.is_valid() == false) {
        return -1;
    }

    unsigned int texture = gen_tex("resources/textures/container.jpg");
    if (texture == -1) {
        std::cout << "Failed to load container texture" << std::endl;
        return -1;
    }

    unsigned int texture2 = gen_tex("resources/textures/awesomeface.png");
    if (texture2 == -1) {
        std::cout << "Failed to load awesomeface texture" << std::endl;
        return -1;
    }

    box_shader.set_uniform_callback([texture, texture2](const Shader *shader) {
        // wrap repeat normal

        // if not use binding, use this
        // shader.use();
        // shader.set_int("texture1", 0);
        // shader.set_int("texture2", 1);
        shader->bind_texture(texture, GL_TEXTURE0);
        shader->bind_texture(texture2, GL_TEXTURE1);
        shader->set_vec3("ourColor", 0.0f, 0.0f, 1.0f);
        shader->set_float("mix_value", 0.2f);
    });

    Mesh3D box_instance(&box_mesh, &box_shader);
    box_instance.position = glm::vec3(0.0f, 0.0f, 0.0f);
    box_instance.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    box_instance.scale = glm::vec3(1.0f, 1.0f, 1.0f);

// box_mesh end

// light_mesh start

#ifndef __APPLE__
    Shader light_shader(
        (const char*)light_mesh_vert_spv_data,
        sizeof(light_mesh_vert_spv_data),
        (const char*)light_mesh_frag_spv_data,
        sizeof(light_mesh_frag_spv_data)
    );
#else
    Shader light_shader("shaders/light_mesh.vert", "shaders/light_mesh.frag");
#endif

    if (light_shader.is_valid() == false) {
        return -1;
    }

    Light3DMesh light_mesh(&box_mesh, &light_shader);
    light_mesh.color = glm::vec3(1.0f, 1.0f, 1.0f);
    light_mesh.position = glm::vec3(0.5f, 0.5f, 0.1f);
    light_mesh.rotation = glm::vec3(2.0f, 1.0f, 0.0f);
    light_mesh.scale = glm::vec3(0.2f, 0.2f, 0.2f);

// light_mesh end

    float move_radius = 0.0f;

    float last_frame_time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        float time_value = glfwGetTime();
        float delta = time_value - last_frame_time;
        last_frame_time = time_value;

        move_radius += MY_PI / 6. * delta;
        // main_camera.position = glm::vec3(2.0f * sin(move_radius), 1.0f, 2.0f * cos(move_radius));
        // main_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        box_instance.rotation = glm::vec3(0.0f, move_radius, 0.0f);

        box_instance.draw();
        light_mesh.draw();

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
