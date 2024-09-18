#include "shader.h"

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath): ID(-1) {
    unsigned int vertex, fragment;

    std::string vertexFilePath(vertexPath);
    std::string fragmentFilePath(fragmentPath);

    std::string vertexFileExtension = vertexFilePath.substr(vertexFilePath.find_last_of(".") + 1);
    std::string fragmentFileExtension = fragmentFilePath.substr(fragmentFilePath.find_last_of(".") + 1);

    if (vertexFileExtension == ".spv") {
        vertex = gen_spv_shader(vertexPath, GL_VERTEX_SHADER);
    } else {
        vertex = gen_text_shader(vertexPath, GL_VERTEX_SHADER);
    }

    if (fragmentFileExtension == ".spv") {
        fragment = gen_spv_shader(fragmentPath, GL_FRAGMENT_SHADER);
    } else {
        fragment = gen_text_shader(fragmentPath, GL_FRAGMENT_SHADER);
    }

    if (vertex == -1 || fragment == -1) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        ID = -1;
        return;
    }
    
    if (!link_shader_program(vertex, fragment)) {
        ID = -1;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char* vertexSpvData, size_t vertexDataSize, const char* fragmentSpvData, size_t fragmentDataSize): ID(-1) {
    unsigned int vertex, fragment;

    vertex = gen_spv_shader(vertexSpvData, vertexDataSize, GL_VERTEX_SHADER);
    fragment = gen_spv_shader(fragmentSpvData, fragmentDataSize, GL_FRAGMENT_SHADER);

    if (vertex == -1 || fragment == -1) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        ID = -1;
        return;
    }

    if (!link_shader_program(vertex, fragment)) {
        ID = -1;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

bool Shader::link_shader_program(unsigned int vertexShader, unsigned int fragmentShader) {
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

bool Shader::is_valid() const {
    return ID != -1;
}

unsigned int Shader::gen_text_shader(const char* shaderPath, unsigned int shaderType) {
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

unsigned int Shader::gen_spv_shader(const char* shaderPath, unsigned int shaderType) {
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

unsigned int Shader::gen_spv_shader(const char* spvData, size_t dataSize, unsigned int shaderType) {
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

    if (uniform_callback != nullptr) {
        uniform_callback(this);
    }

    if (uniform_callback_func != nullptr) {
        uniform_callback_func(this);
    }
}

void Shader::set_uniform_callback(void (*callback)(const Shader *shader)) {
    uniform_callback = callback;
}

void Shader::set_uniform_callback(std::function<void(const Shader *shader)> callback) {
    uniform_callback_func = callback;
}

void Shader::set_bool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_vec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::set_mat3(const std::string &name, const glm::mat3 value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_mat4(const std::string &name, const glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::bind_texture(unsigned int texture, GLint unit) const {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::bind_block(const GLuint ubo, const GLuint binding_index, const char *block_name) const {
    glUniformBlockBinding(ID, glGetUniformBlockIndex(ID, block_name), binding_index);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding_index, ubo);
}
