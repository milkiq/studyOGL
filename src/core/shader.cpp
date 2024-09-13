#include "shader.h"

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath): ID(-1) {
    unsigned int vertex, fragment;

    std::string vertexFilePath(vertexPath);
    std::string fragmentFilePath(fragmentPath);

    std::string vertexFileExtension = vertexFilePath.substr(vertexFilePath.find_last_of(".") + 1);
    std::string fragmentFileExtension = fragmentFilePath.substr(fragmentFilePath.find_last_of(".") + 1);

    if (vertexFileExtension == ".spv") {
        vertex = genSpvShader(vertexPath, GL_VERTEX_SHADER);
    } else {
        vertex = genTextShader(vertexPath, GL_VERTEX_SHADER);
    }

    if (fragmentFileExtension == ".spv") {
        fragment = genSpvShader(fragmentPath, GL_FRAGMENT_SHADER);
    } else {
        fragment = genTextShader(fragmentPath, GL_FRAGMENT_SHADER);
    }

    if (vertex == -1 || fragment == -1) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        ID = -1;
        return;
    }
    
    if (!linkShaderProgram(vertex, fragment)) {
        ID = -1;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char* vertexSpvData, size_t vertexDataSize, const char* fragmentSpvData, size_t fragmentDataSize): ID(-1) {
    unsigned int vertex, fragment;

    vertex = genSpvShader(vertexSpvData, vertexDataSize, GL_VERTEX_SHADER);
    fragment = genSpvShader(fragmentSpvData, fragmentDataSize, GL_FRAGMENT_SHADER);

    if (vertex == -1 || fragment == -1) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        ID = -1;
        return;
    }

    if (!linkShaderProgram(vertex, fragment)) {
        ID = -1;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

bool Shader::linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Shader::isValid() const {
    return ID != -1;
}

unsigned int Shader::genTextShader(const char* shaderPath, unsigned int shaderType) {
    string code;
    ifstream shaderFile;

    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        shaderFile = ifstream(shaderPath);

        stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();

        code = shaderStream.str();
    } catch (ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        return -1;
    }

    const char* shaderCode = code.c_str();

    unsigned int sid;

    sid = glCreateShader(shaderType);
    glShaderSource(sid, 1, &shaderCode, NULL);
    glCompileShader(sid);

    int success;
    char infoLog[512];
    glGetShaderiv(sid, GL_COMPILE_STATUS, &success);

    if (!success) {
        glDeleteShader(sid);
        glGetShaderInfoLog(sid, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    return sid;
}

unsigned int Shader::genSpvShader(const char* shaderPath, unsigned int shaderType) {
    ifstream shaderFile;
    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    size_t size = 0;
    char* buffer = nullptr;
    try {
        shaderFile = ifstream(shaderPath, ios::binary);

        shaderFile.seekg(0, ios::end);
        size = shaderFile.tellg();
        shaderFile.seekg(0, ios::beg);

        buffer = new char[size];
        shaderFile.read(buffer, size);

        shaderFile.close();
    } catch (ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        return -1;
    }

    unsigned int sid;
    sid = glCreateShader(shaderType);
    glShaderBinary(1, &sid, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer, size);
    glSpecializeShader(sid, "main", 0, nullptr, nullptr);

    if (buffer != nullptr)
        delete[] buffer;

    int success;
    char infoLog[512];
    glGetShaderiv(sid, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(sid, 512, NULL, infoLog);
        glDeleteShader(sid);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    return sid;
}

unsigned int Shader::genSpvShader(const char* spvData, size_t dataSize, unsigned int shaderType) {
    unsigned int sid;
    sid = glCreateShader(shaderType);
    glShaderBinary(1, &sid, GL_SHADER_BINARY_FORMAT_SPIR_V, spvData, dataSize);
    glSpecializeShader(sid, "main", 0, nullptr, nullptr);

    int success;
    char infoLog[512];
    glGetShaderiv(sid, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(sid, 512, NULL, infoLog);
        glDeleteShader(sid);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    return sid;
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
