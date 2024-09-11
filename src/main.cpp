#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Geometry>
#include "shader.h"
#include "resource/image.h"

#define MY_PI 3.14159265358979323846

static unsigned char g_vert_spv_data[] = {
    #include "triangle.vert.spv.h"
};

static unsigned char g_frag_spv_data[] = {
    #include "triangle.frag.spv.h"
};

using namespace std;
using namespace Eigen;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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

void initBuffers(
    const float * const vertices,
    unsigned int numVertices,
    const unsigned int * const indices,
    unsigned int numIndices,
    unsigned int &VBO,
    unsigned int &EBO,
    unsigned int &VAO
) {

    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices, vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
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

Matrix4f perspective(float fovY, float aspect, float near, float far) {
    Matrix4f m;
    m.setZero();
    float f = 1.0f / tan(fovY / 2.0f);
    m(0, 0) = f / aspect;
    m(1, 1) = f;
    m(2, 2) = (far + near) / (far - near);
    m(2, 3) = -1.0f;
    m(3, 2) = -(2.0f * far * near) / (far - near);
    return m;
}

int main(int argc, char** argv)
{
    Transform<float, 3, Affine> model_matrix = Translation3f(0,0,0) * AngleAxisf(0, Vector3f(0,1,0)) * Scaling(1.0f);
    Transform<float, 3, Affine> view_matrix = Translation3f(0,0,-3) * AngleAxisf(0, Vector3f(0,0,1)) * Scaling(1.0f);
    Matrix4f projection_matrix = perspective(0.5*MY_PI, 800.0f/600.0f, 0.1f, 100.0f);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifndef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.25f,  0.25f, 0.25f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 前右上
        0.25f, -0.25f, 0.25f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 前右下
        -0.25f, -0.25f, 0.25f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 前左下
        -0.25f,  0.25f, 0.25f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 前左上
        0.25f,  0.25f, -0.25f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 后右上
        0.25f, -0.25f, -0.25f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 后右下
        -0.25f, -0.25f, -0.25f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 后左下
        -0.25f,  0.25f, -0.25f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 后左上
    };

    unsigned int indices[] = {
        // 前
        0, 1, 3, // 第一个三角形
        1, 2, 3,  // 第二个三角形
        // 上
        4, 0, 7,
        0, 3, 7,
        // 下
        1, 5, 2,
        5, 6, 2,
        // 左
        3, 2, 7,
        2, 6, 7,
        // 右
        4, 5, 0,
        5, 1, 0,
        // 后
        7, 6, 4,
        6, 5, 4,
    };

    unsigned int VBO, EBO, VAO;
    initBuffers(vertices, sizeof(vertices), indices, sizeof(indices), VBO, EBO, VAO);

    if (VAO == 0 || VBO == 0 || EBO == 0) {
        return -1;
    }

#ifndef __APPLE__
    Shader shader(
        (const char*)g_vert_spv_data,
        sizeof(g_vert_spv_data),
        (const char*)g_frag_spv_data,
        sizeof(g_frag_spv_data)
    );
#else
    Shader shader("shaders/triangle.vs", "shaders/triangle.fs");
#endif

    if (shader.ID == -1) {
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
    // wrap repeat normal

    // if not use binding, use this
    // shader.use();
    // shader.setInt("texture1", 0);
    // shader.setInt("texture2", 1);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time_value = glfwGetTime();
        model_matrix = Translation3f(0,0,0) * AngleAxisf(-time_value*MY_PI, Vector3f(0,1,0)) * Scaling(1.0f);

        shader.use();
        // shader.setVec3("ourColor", 0.0f, 0.0f, 1.0f);
        GLint transformLoc = glGetUniformLocation(shader.ID, "model_matrix");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, model_matrix.data());
        transformLoc = glGetUniformLocation(shader.ID, "view_matrix");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, view_matrix.data());
        transformLoc = glGetUniformLocation(shader.ID, "projection_matrix");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, projection_matrix.data());

        shader.setFloat("mix_value", 0.2f);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
