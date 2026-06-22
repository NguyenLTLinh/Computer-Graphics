#pragma once

// ============================================================
// Shader.h - Quản lý biên dịch và liên kết GLSL Shader
// ============================================================

#include "Common.h"

class Shader {
public:
    // ID của shader program trên GPU
    GLuint programID = 0;

    // --------------------------------------------------------
    // Constructor: Nạp từ đường dẫn file .vert và .frag
    // --------------------------------------------------------
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // Destructor: Giải phóng tài nguyên GPU
    ~Shader();

    // Không cho phép copy (RAII pattern)
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    // Kích hoạt shader program
    void use() const;

    // --------------------------------------------------------
    // Các hàm truyền Uniform Variable
    // --------------------------------------------------------
    void setInt   (const std::string& name, int value)          const;
    void setFloat (const std::string& name, float value)        const;
    void setBool  (const std::string& name, bool value)         const;
    void setVec2  (const std::string& name, const glm::vec2& v) const;
    void setVec3  (const std::string& name, const glm::vec3& v) const;
    void setVec4  (const std::string& name, const glm::vec4& v) const;
    void setMat3  (const std::string& name, const glm::mat3& m) const;
    void setMat4  (const std::string& name, const glm::mat4& m) const;

    // Truyền thông tin Point Light (tối đa 8 đèn)
    void setPointLight(const std::string& prefix, const struct PointLight& light) const;

    // Truyền thông tin Directional Light
    void setDirectionalLight(const std::string& prefix, const struct DirectionalLight& light) const;

private:
    // Đọc nội dung file shader từ đĩa
    std::string readFile(const std::string& path) const;

    // Biên dịch một shader đơn lẻ (vertex hoặc fragment)
    GLuint compileShader(const std::string& source, GLenum type) const;

    // Lấy vị trí uniform và cảnh báo nếu không tìm thấy
    GLint getUniformLocation(const std::string& name) const;
};
