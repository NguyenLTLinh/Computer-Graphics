#pragma once

// ============================================================
// Renderer.h - Hệ thống render trung tâm
// Quản lý state OpenGL và submit draw calls
// ============================================================

#include "Common.h"
#include "Shader.h"
#include "Camera.h"

// Struct đại diện cho một đối tượng có thể vẽ trong scene
struct RenderObject {
    std::shared_ptr<class Mesh>    mesh;
    std::shared_ptr<Texture>       diffuseTexture;
    std::shared_ptr<Texture>       emissiveTexture; // Map phát sáng cho mái vòm
    glm::mat4                      modelMatrix;     // Transform trong world space
    std::string                    name;            // Tên để debug
};

class Renderer {
public:
    // --------------------------------------------------------
    // Constructor: Khởi tạo OpenGL state
    // --------------------------------------------------------
    explicit Renderer();
    ~Renderer();

    // --------------------------------------------------------
    // Bắt đầu frame mới: xóa buffer, thiết lập clear color
    // --------------------------------------------------------
    void beginFrame();

    // --------------------------------------------------------
    // Kết thúc frame: không cần gọi glfwSwapBuffers ở đây
    // (việc đó thuộc về main loop để linh hoạt)
    // --------------------------------------------------------
    void endFrame();

    // --------------------------------------------------------
    // Render toàn bộ danh sách object với một shader
    // Sử dụng Phong Lighting Model
    // --------------------------------------------------------
    void renderScene(
        const std::vector<RenderObject>& objects,
        const Shader&                    shader,
        const Camera&                    camera,
        const std::vector<PointLight>&   pointLights,
        const DirectionalLight&          sunLight,
        float                            aspectRatio
    );

    // --------------------------------------------------------
    // Render một object đơn lẻ
    // --------------------------------------------------------
    void renderObject(
        const RenderObject& obj,
        const Shader&       shader
    );

    // Thiết lập viewport khi cửa sổ thay đổi kích thước
    void setViewport(int width, int height);

    // Bật/tắt wireframe (dùng để debug geometry)
    void setWireframe(bool enabled);

    // Màu nền (ocean color)
    void setClearColor(const glm::vec4& color);

private:
    glm::vec4 clearColor;
    bool      wireframeMode = false;
};
