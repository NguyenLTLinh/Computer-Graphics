#pragma once

// ============================================================
// Mesh.h - Quản lý VAO, VBO, IBO và vẽ Indexed Geometry
// Sử dụng glDrawElements để tránh trùng lặp dữ liệu đỉnh
// ============================================================

#include "Common.h"
#include "Shader.h"

// ============================================================
// Struct Vertex - Dữ liệu cho mỗi đỉnh trong mesh
// Layout phải khớp với attribute location trong vertex shader
// ============================================================
struct Vertex {
    glm::vec3 position;  // location = 0
    glm::vec3 normal;    // location = 1 (Normal vector cho Phong lighting)
    glm::vec2 texCoord;  // location = 2 (Texture UV coordinates)
};

// ============================================================
// Struct MaterialProperties - Thuộc tính vật liệu Phong
// ============================================================
struct MaterialProperties {
    glm::vec3 ambient;          // Màu ambient (phản chiếu ánh sáng môi trường)
    glm::vec3 diffuse;          // Màu diffuse (phản chiếu ánh sáng khuếch tán)
    glm::vec3 specular;         // Màu specular (phản chiếu gương)
    float     shininess;        // Độ bóng (cao = bóng tập trung, thấp = mờ)
    float     emissiveStrength; // Độ phát sáng tự thân (cho các mái vòm sáng)
    glm::vec3 emissiveColor;    // Màu phát sáng

    // Constructor mặc định: đá cổ không phát sáng
    MaterialProperties()
        : ambient(0.1f, 0.12f, 0.15f)
        , diffuse(0.5f, 0.55f, 0.6f)
        , specular(0.3f, 0.3f, 0.35f)
        , shininess(32.0f)
        , emissiveStrength(0.0f)
        , emissiveColor(0.0f)
    {}
};

class Mesh {
public:
    // Dữ liệu CPU-side (giữ lại để có thể upload lại nếu cần)
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    MaterialProperties        material;

    // --------------------------------------------------------
    // Constructor: Tạo mesh từ dữ liệu đỉnh và chỉ mục
    // --------------------------------------------------------
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const MaterialProperties& material = MaterialProperties());

    // Destructor: Giải phóng VAO, VBO, IBO trên GPU
    ~Mesh();

    // Không cho phép copy
    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Cho phép move
    Mesh(Mesh&& other) noexcept;

    // --------------------------------------------------------
    // Vẽ mesh: Truyền material vào shader rồi gọi glDrawElements
    // --------------------------------------------------------
    void draw(const Shader& shader) const;

    // Cập nhật dữ liệu đỉnh (nếu geometry thay đổi)
    void updateVertices(const std::vector<Vertex>& newVertices);

    // Getters
    GLuint getVAO() const { return vao; }
    size_t getIndexCount() const { return indices.size(); }

private:
    GLuint vao = 0;  // Vertex Array Object
    GLuint vbo = 0;  // Vertex Buffer Object
    GLuint ibo = 0;  // Index Buffer Object (Element Buffer)

    // Upload dữ liệu lên GPU và thiết lập attribute pointers
    void setupMesh();
};

// ============================================================
// Namespace chứa các hàm tạo primitive geometry
// Dùng để build kiến trúc thành phố theo khối cơ bản
// ============================================================
namespace Primitives {
    // Tạo khối hộp chữ nhật (tường, nền, trụ cột)
    Mesh createBox(float width, float height, float depth,
                   const MaterialProperties& mat = MaterialProperties());

    // Tạo hình trụ (cột trụ La Mã)
    Mesh createCylinder(float radius, float height, int segments,
                        const MaterialProperties& mat = MaterialProperties());

    // Tạo mái vòm bán cầu (dome)
    Mesh createDome(float radius, int rings, int slices,
                    const MaterialProperties& mat = MaterialProperties());

    // Tạo mặt phẳng (nền đại dương)
    Mesh createPlane(float width, float depth, int subdivisions,
                     const MaterialProperties& mat = MaterialProperties());

    // Tạo hình chóp cụt (cho mái kiến trúc Hy Lạp)
    Mesh createFrustum(float topRadius, float botRadius, float height, int segments,
                       const MaterialProperties& mat = MaterialProperties());
}
