// ============================================================
// Mesh.cpp - Triển khai Mesh với VAO/VBO/IBO (Indexed Drawing)
// ============================================================

#include "Mesh.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
// Constructor
// ============================================================
Mesh::Mesh(const std::vector<Vertex>& verts,
           const std::vector<unsigned int>& idxs,
           const MaterialProperties& mat)
    : vertices(verts), indices(idxs), material(mat)
{
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices))
    , indices(std::move(other.indices))
    , material(other.material)
    , vao(other.vao), vbo(other.vbo), ibo(other.ibo)
{
    other.vao = other.vbo = other.ibo = 0;
}

// ============================================================
// Upload dữ liệu lên GPU và thiết lập Vertex Attribute Pointers
// ============================================================
void Mesh::setupMesh() {
    // Tạo và bind VAO trước
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ---- Vertex Buffer Object ----
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 vertices.data(),
                 GL_STATIC_DRAW);

    // ---- Index Buffer Object ----
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    // ---- Thiết lập Vertex Attributes ----
    // location=0: position (vec3, offset=0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, position));

    // location=1: normal (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));

    // location=2: texCoord (vec2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texCoord));

    // Unbind VAO (nhưng KHÔNG unbind IBO trước khi unbind VAO)
    glBindVertexArray(0);
}

// ============================================================
// Vẽ mesh: Truyền material rồi gọi glDrawElements
// ============================================================
void Mesh::draw(const Shader& shader) const {
    // Truyền thuộc tính vật liệu Phong vào shader
    shader.setVec3 ("material.ambient",          material.ambient);
    shader.setVec3 ("material.diffuse",          material.diffuse);
    shader.setVec3 ("material.specular",         material.specular);
    shader.setFloat("material.shininess",        material.shininess);
    shader.setFloat("material.emissiveStrength", material.emissiveStrength);
    shader.setVec3 ("material.emissiveColor",    material.emissiveColor);

    // Bind VAO và vẽ bằng indices (Indexed Drawing)
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}

void Mesh::updateVertices(const std::vector<Vertex>& newVertices) {
    vertices = newVertices;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    vertices.size() * sizeof(Vertex),
                    vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ============================================================
//  PRIMITIVES - Các hàm tạo hình học cơ bản
// ============================================================

namespace Primitives {

// ============================================================
// Hộp chữ nhật (Box/Cuboid) - Dùng cho tường, nền, trụ
// ============================================================
Mesh createBox(float w, float h, float d, const MaterialProperties& mat) {
    float hw = w * 0.5f, hh = h * 0.5f, hd = d * 0.5f;

    // 24 đỉnh (4 đỉnh mỗi mặt x 6 mặt) để normal mỗi mặt độc lập
    std::vector<Vertex> verts = {
        // +X (phải)
        {{hw,-hh,-hd},{1,0,0},{0,0}}, {{hw,-hh, hd},{1,0,0},{1,0}},
        {{hw, hh, hd},{1,0,0},{1,1}}, {{hw, hh,-hd},{1,0,0},{0,1}},
        // -X (trái)
        {{-hw,-hh, hd},{-1,0,0},{0,0}}, {{-hw,-hh,-hd},{-1,0,0},{1,0}},
        {{-hw, hh,-hd},{-1,0,0},{1,1}}, {{-hw, hh, hd},{-1,0,0},{0,1}},
        // +Y (trên)
        {{-hw,hh,-hd},{0,1,0},{0,0}}, {{ hw,hh,-hd},{0,1,0},{1,0}},
        {{ hw,hh, hd},{0,1,0},{1,1}}, {{-hw,hh, hd},{0,1,0},{0,1}},
        // -Y (dưới)
        {{-hw,-hh, hd},{0,-1,0},{0,0}}, {{ hw,-hh, hd},{0,-1,0},{1,0}},
        {{ hw,-hh,-hd},{0,-1,0},{1,1}}, {{-hw,-hh,-hd},{0,-1,0},{0,1}},
        // +Z (trước)
        {{-hw,-hh,hd},{0,0,1},{0,0}}, {{ hw,-hh,hd},{0,0,1},{1,0}},
        {{ hw, hh,hd},{0,0,1},{1,1}}, {{-hw, hh,hd},{0,0,1},{0,1}},
        // -Z (sau)
        {{ hw,-hh,-hd},{0,0,-1},{0,0}}, {{-hw,-hh,-hd},{0,0,-1},{1,0}},
        {{-hw, hh,-hd},{0,0,-1},{1,1}}, {{ hw, hh,-hd},{0,0,-1},{0,1}},
    };

    // 6 mặt x 2 tam giác x 3 đỉnh = 36 indices
    std::vector<unsigned int> idx;
    for (unsigned int f = 0; f < 6; ++f) {
        unsigned int b = f * 4;
        idx.insert(idx.end(), {b,b+1,b+2, b,b+2,b+3});
    }

    return Mesh(verts, idx, mat);
}

// ============================================================
// Hình trụ (Cylinder) - Dùng cho cột kiến trúc
// ============================================================
Mesh createCylinder(float radius, float height, int segs, const MaterialProperties& mat) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> idx;
    float hh = height * 0.5f;

    // Sinh các vòng đỉnh (top và bottom)
    for (int ring = 0; ring <= 1; ++ring) {
        float y   = (ring == 0) ? -hh : hh;
        float ny  = (ring == 0) ? 0.0f : 0.0f; // Normal cho thân trụ
        float v   = (float)ring;

        for (int i = 0; i <= segs; ++i) {
            float angle = 2.0f * (float)M_PI * i / segs;
            float cx = cosf(angle), cz = sinf(angle);
            Vertex vert;
            vert.position = {radius * cx, y, radius * cz};
            vert.normal   = {cx, 0.0f, cz}; // Normal hướng ra ngoài
            vert.texCoord = {(float)i / segs, v};
            verts.push_back(vert);
        }
    }

    // Indices cho thân trụ
    int stride = segs + 1;
    for (int i = 0; i < segs; ++i) {
        unsigned int b = i, t = stride + i;
        idx.insert(idx.end(), {b,t,b+1, t,t+1,b+1});
    }

    // Cap trên và dưới
    for (int cap = 0; cap < 2; ++cap) {
        float y      = (cap == 0) ? -hh : hh;
        float ny     = (cap == 0) ? -1.0f : 1.0f;
        unsigned int centerIdx = (unsigned int)verts.size();

        Vertex center;
        center.position = {0.0f, y, 0.0f};
        center.normal   = {0.0f, ny, 0.0f};
        center.texCoord = {0.5f, 0.5f};
        verts.push_back(center);

        unsigned int firstRim = (unsigned int)verts.size();
        for (int i = 0; i <= segs; ++i) {
            float angle = 2.0f * (float)M_PI * i / segs;
            Vertex v2;
            v2.position = {radius * cosf(angle), y, radius * sinf(angle)};
            v2.normal   = {0.0f, ny, 0.0f};
            v2.texCoord = {0.5f + 0.5f * cosf(angle), 0.5f + 0.5f * sinf(angle)};
            verts.push_back(v2);
        }

        for (int i = 0; i < segs; ++i) {
            if (cap == 0) { // Bottom cap - winding đảo ngược
                idx.insert(idx.end(), {centerIdx, firstRim + i + 1, firstRim + i});
            } else {
                idx.insert(idx.end(), {centerIdx, firstRim + i,     firstRim + i + 1});
            }
        }
    }

    return Mesh(verts, idx, mat);
}

// ============================================================
// Mái vòm (Dome) - Cho đỉnh tháp canh và cung điện
// ============================================================
Mesh createDome(float radius, int rings, int slices, const MaterialProperties& mat) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> idx;

    // Chỉ dựng nửa trên của hình cầu (dome)
    for (int r = 0; r <= rings; ++r) {
        // Phi: từ 0 (đỉnh) đến PI/2 (xích đạo)
        float phi = (float)M_PI * 0.5f * r / rings;
        float y   = radius * cosf(phi);
        float xzR = radius * sinf(phi);

        for (int s = 0; s <= slices; ++s) {
            float theta = 2.0f * (float)M_PI * s / slices;
            Vertex v;
            v.position = {xzR * cosf(theta), y, xzR * sinf(theta)};
            v.normal   = glm::normalize(v.position); // Normal = position/radius cho hình cầu
            v.texCoord = {(float)s / slices, (float)r / rings};
            verts.push_back(v);
        }
    }

    int stride = slices + 1;
    for (int r = 0; r < rings; ++r) {
        for (int s = 0; s < slices; ++s) {
            int cur  = r * stride + s;
            int next = cur + stride;
            idx.insert(idx.end(), {
                (unsigned int)cur,       (unsigned int)(cur+1),
                (unsigned int)(next+1),  (unsigned int)cur,
                (unsigned int)(next+1),  (unsigned int)next
            });
        }
    }

    return Mesh(verts, idx, mat);
}

// ============================================================
// Mặt phẳng (Ocean Floor) - Có subdivision để đổ bóng mịn
// ============================================================
Mesh createPlane(float width, float depth, int subdiv, const MaterialProperties& mat) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> idx;
    int n = subdiv + 1; // Số đỉnh mỗi chiều

    for (int z = 0; z < n; ++z) {
        for (int x = 0; x < n; ++x) {
            float u = (float)x / subdiv;
            float v = (float)z / subdiv;
            Vertex vert;
            vert.position = {(u - 0.5f) * width, 0.0f, (v - 0.5f) * depth};
            vert.normal   = {0.0f, 1.0f, 0.0f}; // Hướng lên
            vert.texCoord = {u * 8.0f, v * 8.0f}; // Tile texture 8x8
            verts.push_back(vert);
        }
    }

    for (int z = 0; z < subdiv; ++z) {
        for (int x = 0; x < subdiv; ++x) {
            unsigned int tl = z * n + x;
            unsigned int tr = tl + 1;
            unsigned int bl = (z+1) * n + x;
            unsigned int br = bl + 1;
            idx.insert(idx.end(), {tl, bl, tr, tr, bl, br});
        }
    }

    return Mesh(verts, idx, mat);
}

// ============================================================
// Hình chóp cụt (Frustum) - Mái kiến trúc Hy Lạp/La Mã
// ============================================================
Mesh createFrustum(float topR, float botR, float height, int segs, const MaterialProperties& mat) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> idx;
    float hh = height * 0.5f;
    float slope = (botR - topR) / height; // Độ dốc mặt bên

    for (int ring = 0; ring <= 1; ++ring) {
        float y = (ring == 0) ? -hh : hh;
        float r = (ring == 0) ? botR : topR;
        float v = (float)ring;

        for (int i = 0; i <= segs; ++i) {
            float angle = 2.0f * (float)M_PI * i / segs;
            float cx = cosf(angle), cz = sinf(angle);

            // Normal của mặt bên nghiêng
            glm::vec3 normal = glm::normalize(glm::vec3(cx, slope, cz));

            Vertex vert;
            vert.position = {r * cx, y, r * cz};
            vert.normal   = normal;
            vert.texCoord = {(float)i / segs, v};
            verts.push_back(vert);
        }
    }

    int stride = segs + 1;
    for (int i = 0; i < segs; ++i) {
        unsigned int b = i, t = stride + i;
        idx.insert(idx.end(), {b, b+1, t, b+1, t+1, t});
    }

    return Mesh(verts, idx, mat);
}

} // namespace Primitives
