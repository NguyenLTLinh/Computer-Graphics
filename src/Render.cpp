// ============================================================
// Renderer.cpp - Hệ thống render với Phong Lighting
// ============================================================

#include "Renderer.h"
#include "Mesh.h"
#include "Texture.h"

// ============================================================
// Khởi tạo OpenGL State
// ============================================================
Renderer::Renderer()
    : clearColor(OCEAN_FOG_COLOR.r, OCEAN_FOG_COLOR.g, OCEAN_FOG_COLOR.b, 1.0f)
{
    // Bật depth testing: vật thể gần che vật thể xa
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Bật back-face culling: không vẽ mặt phía sau polygon
    // Tiết kiệm ~50% fill rate
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // Counter-clockwise = mặt trước

    // Bật blending để hỗ trợ alpha transparency (nước, hiệu ứng)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Bật gamma correction (sRGB framebuffer)
    // glEnable(GL_FRAMEBUFFER_SRGB); // Uncomment nếu dùng HDR pipeline

    setClearColor(clearColor);
    std::cout << "[Renderer] OpenGL state initialized.\n";
}

Renderer::~Renderer() {}

// ============================================================
// Bắt đầu frame: xóa color và depth buffer
// ============================================================
void Renderer::beginFrame() {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
    // Có thể thêm post-processing pass ở đây sau này
}

// ============================================================
// Render toàn bộ scene với Phong Lighting Model
// ============================================================
void Renderer::renderScene(
    const std::vector<RenderObject>& objects,
    const Shader&                    shader,
    const Camera&                    camera,
    const std::vector<PointLight>&   pointLights,
    const DirectionalLight&          sunLight,
    float                            aspectRatio)
{
    shader.use();

    // ---- Truyền ma trận View và Projection ----
    glm::mat4 view       = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);

    shader.setMat4("view",       view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos",    camera.getPosition()); // Cần cho specular

    // ---- Truyền môi trường ánh sáng đại dương ----
    shader.setVec3 ("ambientColor",  OCEAN_AMBIENT_COLOR);
    shader.setVec3 ("fogColor",      OCEAN_FOG_COLOR);
    shader.setFloat("fogDensity",    OCEAN_FOG_DENSITY);
    shader.setFloat("time",          (float)glfwGetTime()); // Cho animation

    // ---- Truyền Directional Light (ánh sáng từ mặt nước) ----
    shader.setDirectionalLight("sunLight", sunLight);

    // ---- Truyền tất cả Point Lights (mái vòm phát sáng) ----
    int numLights = static_cast<int>(std::min(pointLights.size(), size_t(8)));
    shader.setInt("numPointLights", numLights);
    for (int i = 0; i < numLights; ++i) {
        shader.setPointLight("pointLights[" + std::to_string(i) + "]", pointLights[i]);
    }

    // ---- Vẽ từng object ----
    for (const auto& obj : objects) {
        renderObject(obj, shader);
    }
}

// ============================================================
// Render một object đơn lẻ
// ============================================================
void Renderer::renderObject(const RenderObject& obj, const Shader& shader) {
    if (!obj.mesh) return;

    // Truyền Model Matrix
    shader.setMat4("model", obj.modelMatrix);

    // Normal Matrix: transpose của inverse của model matrix
    // Cần thiết khi model có scale không đều để normal đúng hướng
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(obj.modelMatrix)));
    shader.setMat3("normalMatrix", normalMatrix);

    // Bind diffuse texture vào slot 0
    if (obj.diffuseTexture && obj.diffuseTexture->isValid()) {
        obj.diffuseTexture->bind(0);
        shader.setInt("diffuseTexture", 0);
        shader.setBool("hasDiffuseTexture", true);
    } else {
        shader.setBool("hasDiffuseTexture", false);
    }

    // Bind emissive texture (glow map) vào slot 1
    if (obj.emissiveTexture && obj.emissiveTexture->isValid()) {
        obj.emissiveTexture->bind(1);
        shader.setInt("emissiveTexture", 1);
        shader.setBool("hasEmissiveTexture", true);
    } else {
        shader.setBool("hasEmissiveTexture", false);
    }

    // Vẽ mesh bằng Indexed Drawing
    obj.mesh->draw(shader);
}

void Renderer::setViewport(int w, int h) {
    glViewport(0, 0, w, h);
}

void Renderer::setWireframe(bool enabled) {
    wireframeMode = enabled;
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
}

void Renderer::setClearColor(const glm::vec4& color) {
    clearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}
