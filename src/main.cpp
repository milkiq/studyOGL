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

int main(int argc, char** argv)
{
    Transform<float, 3, Affine> t = Translation3f(0,0,0) * AngleAxisf(0.5*MY_PI, Vector3f(0,0,1)) * Scaling(0.5f);

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
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
        // 0, 1, 2
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

        // float timeValue = glfwGetTime();
        // float colorValue = sin(timeValue) / 2.0f + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // glUniform4f(vertexColorLocation, colorValue, 0.0f, 0.0f, 1.0f);

        shader.use();
        // shader.setVec3("ourColor", 0.0f, 0.0f, 1.0f);
        GLint transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, t.data());
        shader.setFloat("mix_value", 0.2f);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
