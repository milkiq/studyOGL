#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include "math.h"
#include "viewport.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

class Shader {
public:
    // 程序 ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const char* vertexSpvData, size_t vertexDataSize, const char* fragmentSpvData, size_t fragmentDataSize);
    ~Shader();

    bool is_valid() const;

    void set_uniform_callback(void (*callback)(const Shader *shader));
    void set_uniform_callback(std::function<void(const Shader *shader)> callback);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void set_bool(const std::string &name, bool value) const;  
    void set_int(const std::string &name, int value) const;   
    void set_float(const std::string &name, float value) const;
    void set_vec3(const std::string &name, float x, float y, float z) const;
    void set_vec3(const std::string &name, const glm::vec3 value) const;
    void set_mat3(const std::string &name, const glm::mat3 value) const;
    void set_mat4(const std::string &name, const glm::mat4 value) const;

    void bind_texture(unsigned int texture, GLint unit) const;

private:
    void (*uniform_callback)(const Shader *shader) = nullptr;
    std::function<void(const Shader *shader)> uniform_callback_func;

    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;

    unsigned int gen_text_shader(const char* shaderPath, unsigned int shaderType);
    unsigned int gen_spv_shader(const char* shaderPath, unsigned int shaderType);
    unsigned int gen_spv_shader(const char* spvData, size_t dataSize, unsigned int shaderType);

    bool link_shader_program(unsigned int vertexShader, unsigned int fragmentShader);
};

#endif // SHADER_H
