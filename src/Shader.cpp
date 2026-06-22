// ============================================================
// Shader.cpp - Triển khai class Shader
// ============================================================

#include "Shader.h"

// ============================================================
// Constructor: Đọc file, biên dịch và liên kết shader program
// ============================================================
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // 1. Đọc source code từ file
    std::string vertSrc = readFile(vertexPath);
    std::string fragSrc = readFile(fragmentPath);

    // 2. Biên dịch từng shader
    GLuint vertShader = compileShader(vertSrc, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);

    // 3. Liên kết thành shader program
    programID = glCreateProgram();
    glAttachShader(programID, vertShader);
    glAttachShader(programID, fragShader);
    glLinkProgram(programID);

    // Kiểm tra lỗi liên kết
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetProgramInfoLog(programID, 1024, nullptr, log);
        throw std::runtime_error("[Shader] Link error in '" + vertexPath + "':\n" + log);
    }

    // 4. Xóa shader đơn lẻ (đã liên kết vào program, không cần nữa)
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    std::cout << "[Shader] Program compiled successfully: "
              << vertexPath << " + " << fragmentPath << "\n";
}

Shader::~Shader() {
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }
}

void Shader::use() const {
    glUseProgram(programID);
}

// ============================================================
// Đọc nội dung file shader từ đĩa
// ============================================================
std::string Shader::readFile(const std::string& path) const {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("[Shader] Cannot open file: " + path);
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// ============================================================
// Biên dịch một shader (vertex hoặc fragment)
// ============================================================
GLuint Shader::compileShader(const std::string& source, GLenum type) const {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Kiểm tra lỗi biên dịch
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        const char* typeName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        throw std::runtime_error(
            std::string("[Shader] Compile error in ") + typeName + " shader:\n" + log
        );
    }
    return shader;
}

// ============================================================
// Lấy vị trí uniform (với cảnh báo nếu không tìm thấy)
// ============================================================
GLint Shader::getUniformLocation(const std::string& name) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
#ifdef _DEBUG
    if (loc == -1) {
        std::cerr << "[Shader] WARNING: Uniform '" << name << "' not found or unused.\n";
    }
#endif
    return loc;
}

// ============================================================
// Các hàm set Uniform
// ============================================================
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(getUniformLocation(name), (int)value);
}
void Shader::setVec2(const std::string& name, const glm::vec2& v) const {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(v));
}
void Shader::setVec3(const std::string& name, const glm::vec3& v) const {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
}
void Shader::setVec4(const std::string& name, const glm::vec4& v) const {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v));
}
void Shader::setMat3(const std::string& name, const glm::mat3& m) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}
void Shader::setMat4(const std::string& name, const glm::mat4& m) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}

// ============================================================
// Truyền Point Light vào shader theo prefix (vd: "lights[0]")
// ============================================================
void Shader::setPointLight(const std::string& prefix, const PointLight& light) const {
    setVec3 (prefix + ".position",  light.position);
    setVec3 (prefix + ".color",     light.color);
    setFloat(prefix + ".intensity", light.intensity);
    setFloat(prefix + ".constant",  light.constant);
    setFloat(prefix + ".linear",    light.linear);
    setFloat(prefix + ".quadratic", light.quadratic);
}

// ============================================================
// Truyền Directional Light (ánh sáng từ mặt nước)
// ============================================================
void Shader::setDirectionalLight(const std::string& prefix, const DirectionalLight& light) const {
    setVec3 (prefix + ".direction", light.direction);
    setVec3 (prefix + ".color",     light.color);
    setFloat(prefix + ".intensity", light.intensity);
}
