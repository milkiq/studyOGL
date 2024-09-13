#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // 程序 ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const char* vertexSpvData, size_t vertexDataSize, const char* fragmentSpvData, size_t fragmentDataSize);
    ~Shader();

    bool isValid() const;

    // 使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

private:
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;

    unsigned int genTextShader(const char* shaderPath, unsigned int shaderType);
    unsigned int genSpvShader(const char* shaderPath, unsigned int shaderType);
    unsigned int genSpvShader(const char* spvData, size_t dataSize, unsigned int shaderType);

    bool linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
};

#endif // SHADER_H
