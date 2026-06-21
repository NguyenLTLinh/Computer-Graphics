#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    const std::string vertexSource = readFile(vertexPath);
    const std::string fragmentSource = readFile(fragmentPath);

    const GLuint vertexShader = compile(GL_VERTEX_SHADER, vertexSource, vertexPath);
    const GLuint fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource, fragmentPath);

    programId_ = glCreateProgram();
    glAttachShader(programId_, vertexShader);
    glAttachShader(programId_, fragmentShader);
    glLinkProgram(programId_);

    GLint success = GL_FALSE;
    glGetProgramiv(programId_, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(programId_, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(static_cast<size_t>(logLength));
        glGetProgramInfoLog(programId_, logLength, nullptr, log.data());
        throw std::runtime_error("Shader link failed: " + std::string(log.begin(), log.end()));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (programId_ != 0) {
        glDeleteProgram(programId_);
    }
}

Shader::Shader(Shader&& other) noexcept : programId_(std::exchange(other.programId_, 0)) {}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (programId_ != 0) {
            glDeleteProgram(programId_);
        }
        programId_ = std::exchange(other.programId_, 0);
    }
    return *this;
}

void Shader::use() const {
    glUseProgram(programId_);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(uniformLocation(name), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(uniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(uniformLocation(name), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(uniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path, std::ios::in);
    if (!file) {
        throw std::runtime_error("Cannot open shader file: " + path);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compile(GLenum type, const std::string& source, const std::string& debugName) {
    const GLuint shader = glCreateShader(type);
    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(static_cast<size_t>(logLength));
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        throw std::runtime_error("Shader compile failed [" + debugName + "]: " + std::string(log.begin(), log.end()));
    }

    return shader;
}

GLint Shader::uniformLocation(const std::string& name) const {
    return glGetUniformLocation(programId_, name.c_str());
}
