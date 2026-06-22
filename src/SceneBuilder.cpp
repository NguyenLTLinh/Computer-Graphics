// ============================================================
// SceneBuilder.cpp - Xây dựng kiến trúc "The Sunken Capital"
// ============================================================

#include "SceneBuilder.h"
#include <memory>

using namespace Primitives;

// ============================================================
// Điểm vào chính: Xây dựng toàn bộ thành phố
// ============================================================
void SceneBuilder::buildScene(
    std::vector<RenderObject>& objects,
    std::vector<PointLight>&   lights)
{
    // Nền đáy đại dương
    buildOceanFloor(objects);

    // Cung điện trung tâm - Trọng tâm của cảnh
    buildCentralPalace(objects, lights, glm::vec3(0.0f, 0.0f, 0.0f));

    // Hành lang bên trái và phải
    buildCorridor(objects, glm::vec3(-8.0f, 0.0f, -5.0f),
                           glm::vec3(-8.0f, 0.0f,  5.0f), 5);
    buildCorridor(objects, glm::vec3( 8.0f, 0.0f, -5.0f),
                           glm::vec3( 8.0f, 0.0f,  5.0f), 5);

    // Hành lang phía trước
    buildCorridor(objects, glm::vec3(-8.0f, 0.0f,  5.0f),
                           glm::vec3( 8.0f, 0.0f,  5.0f), 8);

    // 4 Tháp canh ở 4 góc với ánh sáng khác màu
    buildWatchTower(objects, lights, glm::vec3(-20.0f, 0.0f, -20.0f), 18.0f,
                    glm::vec3(1.0f, 0.8f, 0.3f));  // Vàng ấm
    buildWatchTower(objects, lights, glm::vec3( 20.0f, 0.0f, -20.0f), 18.0f,
                    glm::vec3(0.8f, 1.0f, 0.5f));  // Xanh lá nhạt
    buildWatchTower(objects, lights, glm::vec3(-20.0f, 0.0f,  20.0f), 15.0f,
                    glm::vec3(1.0f, 0.6f, 0.2f));  // Cam vàng
    buildWatchTower(objects, lights, glm::vec3( 20.0f, 0.0f,  20.0f), 15.0f,
                    glm::vec3(0.9f, 0.9f, 0.5f));  // Vàng trắng

    // Bậc thang phía trước cung điện
    buildStaircases(objects, glm::vec3(0.0f, 0.0f, 6.0f), 5);
}

// ============================================================
// Cung điện trung tâm - Tòa nhà chủ đạo của scene
// ============================================================
void SceneBuilder::buildCentralPalace(
    std::vector<RenderObject>& objects,
    std::vector<PointLight>&   lights,
    const glm::vec3&           origin)
{
    auto stoneMat = stoneWallMaterial();
    auto domeMat  = glowingDomeMaterial(glm::vec3(1.0f, 0.85f, 0.4f));

    // ---- Thân cung điện chính (hộp lớn) ----
    {
        auto mesh = std::make_shared<Mesh>(createBox(14.0f, 10.0f, 12.0f, stoneMat));
        glm::mat4 m = glm::translate(glm::mat4(1.0f), origin + glm::vec3(0, 5.0f, 0));
        objects.push_back(makeObject(mesh, m, "Palace_Main_Body"));
    }

    // ---- Mái vòm lớn (dome chính) ----
    {
        auto mesh = std::make_shared<Mesh>(createDome(5.0f, 16, 32, domeMat));
        glm::mat4 m = glm::translate(glm::mat4(1.0f), origin + glm::vec3(0, 11.0f, 0));
        objects.push_back(makeObject(mesh, m, "Palace_Main_Dome"));

        // Point light từ đỉnh vòm
        PointLight domeLight;
        domeLight.position  = origin + glm::vec3(0.0f, 13.0f, 0.0f);
        domeLight.color     = glm::vec3(1.0f, 0.85f, 0.4f); // Vàng ấm
        domeLight.intensity = 3.0f;
        domeLight.constant  = 1.0f;
        domeLight.linear    = 0.045f;
        domeLight.quadratic = 0.0075f;
        lights.push_back(domeLight);
    }

    // ---- Trụ cột hai bên cung điện (8 cột, 4 mỗi bên) ----
    float columnSpacing = 3.5f;
    for (int side : {-1, 1}) {
        for (int col = 0; col < 4; ++col) {
            float zOffset = (col - 1.5f) * columnSpacing;
            auto  mesh    = std::make_shared<Mesh>(createCylinder(0.5f, 9.0f, 16, stoneMat));
            glm::mat4 m   = glm::translate(glm::mat4(1.0f),
                origin + glm::vec3(side * 7.5f, 4.5f, zOffset));
            objects.push_back(makeObject(mesh, m, "Palace_Column_" + std::to_string(col)));
        }
    }

    // ---- Frieze trên (dầm ngang phía trên cột) ----
    {
        auto mesh = std::make_shared<Mesh>(createBox(18.0f, 1.2f, 1.0f, stoneMat));
        glm::mat4 m = glm::translate(glm::mat4(1.0f), origin + glm::vec3(0, 10.2f, 6.5f));
        objects.push_back(makeObject(mesh, m, "Palace_Frieze_Front"));

        m = glm::translate(glm::mat4(1.0f), origin + glm::vec3(0, 10.2f, -6.5f));
        objects.push_back(makeObject(mesh, m, "Palace_Frieze_Back"));
    }

    // ---- Tầng đế (Stylobate - bậc đế kiến trúc Hy Lạp) ----
    {
        auto mesh = std::make_shared<Mesh>(createBox(16.0f, 1.5f, 14.0f, stoneMat));
        glm::mat4 m = glm::translate(glm::mat4(1.0f), origin + glm::vec3(0, 0.75f, 0));
        objects.push_back(makeObject(mesh, m, "Palace_Stylobate"));
    }

    // ---- Hai tháp trang trí nhỏ hai bên ----
    for (int side : {-1, 1}) {
        auto bodyMesh = std::make_shared<Mesh>(createBox(3.5f, 7.0f, 3.5f, stoneMat));
        auto smDomeMesh = std::make_shared<Mesh>(createDome(2.0f, 10, 20, domeMat));

        glm::mat4 bodyM = glm::translate(glm::mat4(1.0f),
            origin + glm::vec3(side * 10.0f, 3.5f, 0.0f));
        glm::mat4 domeM = glm::translate(glm::mat4(1.0f),
            origin + glm::vec3(side * 10.0f, 8.0f, 0.0f));

        objects.push_back(makeObject(bodyMesh,  bodyM, "Palace_SideTower_Body"));
        objects.push_back(makeObject(smDomeMesh, domeM, "Palace_SideTower_Dome"));

        // Ánh sáng nhỏ từ tháp phụ
        PointLight sideLight;
        sideLight.position  = origin + glm::vec3(side * 10.0f, 9.5f, 0.0f);
        sideLight.color     = glm::vec3(1.0f, 0.75f, 0.3f);
        sideLight.intensity = 1.5f;
        sideLight.constant  = 1.0f;
        sideLight.linear    = 0.09f;
        sideLight.quadratic = 0.032f;
        lights.push_back(sideLight);
    }
}

// ============================================================
// Hành lang cột (Colonnade)
// ============================================================
void SceneBuilder::buildCorridor(
    std::vector<RenderObject>& objects,
    const glm::vec3&           start,
    const glm::vec3&           end,
    int                        numColumns)
{
    auto stoneMat = stoneWallMaterial();

    // Dầm trần hành lang
    glm::vec3 center = (start + end) * 0.5f;
    float     len    = glm::length(end - start);
    glm::vec3 dir    = glm::normalize(end - start);

    // Tính góc xoay của dầm để căn theo hướng hành lang
    float angle = atan2f(dir.x, dir.z);

    auto beamMesh = std::make_shared<Mesh>(createBox(len + 1.0f, 0.8f, 1.2f, stoneMat));
    glm::mat4 beamM = glm::translate(glm::mat4(1.0f), center + glm::vec3(0, 9.2f, 0));
    beamM = glm::rotate(beamM, angle, glm::vec3(0, 1, 0));
    objects.push_back(makeObject(beamMesh, beamM, "Corridor_Beam"));

    // Sinh các cột theo đường thẳng
    for (int i = 0; i < numColumns; ++i) {
        float t = (float)i / (numColumns - 1);
        glm::vec3 pos = glm::mix(start, end, t) + glm::vec3(0, 4.5f, 0);

        auto colMesh = std::make_shared<Mesh>(createCylinder(0.4f, 9.0f, 16, stoneMat));
        glm::mat4 m  = glm::translate(glm::mat4(1.0f), pos);
        objects.push_back(makeObject(colMesh, m, "Corridor_Column_" + std::to_string(i)));
    }
}

// ============================================================
// Tháp canh với mái vòm phát sáng
// ============================================================
void SceneBuilder::buildWatchTower(
    std::vector<RenderObject>& objects,
    std::vector<PointLight>&   lights,
    const glm::vec3&           position,
    float                      height,
    const glm::vec3&           glowColor)
{
    auto stoneMat = stoneWallMaterial();
    auto domeMat  = glowingDomeMaterial(glowColor);

    // Thân tháp
    auto bodyMesh = std::make_shared<Mesh>(createBox(4.0f, height, 4.0f, stoneMat));
    glm::mat4 bodyM = glm::translate(glm::mat4(1.0f), position + glm::vec3(0, height/2.0f, 0));
    objects.push_back(makeObject(bodyMesh, bodyM, "Tower_Body"));

    // Vành trang trí trên đỉnh tháp (cornice)
    auto corniceM = std::make_shared<Mesh>(createBox(5.0f, 1.0f, 5.0f, stoneMat));
    glm::mat4 corniceTransform = glm::translate(glm::mat4(1.0f),
        position + glm::vec3(0, height + 0.5f, 0));
    objects.push_back(makeObject(corniceM, corniceTransform, "Tower_Cornice"));

    // Mái vòm phát sáng
    auto domeMesh = std::make_shared<Mesh>(createDome(2.5f, 10, 20, domeMat));
    glm::mat4 domeM = glm::translate(glm::mat4(1.0f),
        position + glm::vec3(0, height + 1.0f, 0));
    objects.push_back(makeObject(domeMesh, domeM, "Tower_Dome"));

    // Point Light tại mái vòm
    PointLight light;
    light.position  = position + glm::vec3(0.0f, height + 3.0f, 0.0f);
    light.color     = glowColor;
    light.intensity = 2.5f;
    light.constant  = 1.0f;
    light.linear    = 0.027f;
    light.quadratic = 0.0028f;
    lights.push_back(light);
}

// ============================================================
// Nền đáy đại dương
// ============================================================
void SceneBuilder::buildOceanFloor(std::vector<RenderObject>& objects) {
    auto floorMat = oceanFloorMaterial();
    auto mesh     = std::make_shared<Mesh>(createPlane(200.0f, 200.0f, 40, floorMat));
    glm::mat4 m   = glm::mat4(1.0f); // Tại y=0
    objects.push_back(makeObject(mesh, m, "Ocean_Floor"));
}

// ============================================================
// Bậc thang phía trước cung điện
// ============================================================
void SceneBuilder::buildStaircases(
    std::vector<RenderObject>& objects,
    const glm::vec3&           base,
    int                        steps)
{
    auto stoneMat = stoneWallMaterial();
    float stepW = 12.0f, stepH = 0.4f, stepD = 0.8f;

    for (int i = 0; i < steps; ++i) {
        float w  = stepW - i * 0.5f;
        float zO = base.z + i * stepD;
        float yO = base.y + i * stepH;

        auto mesh = std::make_shared<Mesh>(createBox(w, stepH, stepD, stoneMat));
        glm::mat4 m = glm::translate(glm::mat4(1.0f),
            glm::vec3(base.x, yO + stepH/2, zO));
        objects.push_back(makeObject(mesh, m, "Stair_" + std::to_string(i)));
    }
}

// ============================================================
// Material Definitions
// ============================================================
MaterialProperties SceneBuilder::stoneWallMaterial() {
    MaterialProperties m;
    m.ambient   = glm::vec3(0.08f, 0.10f, 0.13f); // Phản chiếu ambient xanh biển
    m.diffuse   = glm::vec3(0.55f, 0.53f, 0.50f); // Xám đá cũ
    m.specular  = glm::vec3(0.20f, 0.22f, 0.25f); // Ánh sáng yếu (đá không bóng)
    m.shininess = 16.0f;
    m.emissiveStrength = 0.0f;
    return m;
}

MaterialProperties SceneBuilder::mossyRockMaterial() {
    MaterialProperties m;
    m.ambient   = glm::vec3(0.05f, 0.10f, 0.07f); // Xanh rêu
    m.diffuse   = glm::vec3(0.30f, 0.45f, 0.30f);
    m.specular  = glm::vec3(0.05f, 0.08f, 0.05f);
    m.shininess = 8.0f;
    return m;
}

MaterialProperties SceneBuilder::glowingDomeMaterial(const glm::vec3& color) {
    MaterialProperties m;
    m.ambient         = color * 0.3f;
    m.diffuse         = color * 0.7f;
    m.specular        = glm::vec3(1.0f, 0.95f, 0.8f);
    m.shininess       = 64.0f;
    m.emissiveStrength = 1.2f;
    m.emissiveColor   = color;
    return m;
}

MaterialProperties SceneBuilder::oceanFloorMaterial() {
    MaterialProperties m;
    m.ambient   = glm::vec3(0.03f, 0.06f, 0.08f);
    m.diffuse   = glm::vec3(0.20f, 0.25f, 0.28f);
    m.specular  = glm::vec3(0.05f, 0.08f, 0.10f);
    m.shininess = 4.0f;
    return m;
}

// ============================================================
// Helper: Tạo RenderObject từ mesh và transform
// ============================================================
RenderObject SceneBuilder::makeObject(
    std::shared_ptr<Mesh> mesh,
    const glm::mat4&      model,
    const std::string&    name)
{
    RenderObject obj;
    obj.mesh        = mesh;
    obj.modelMatrix = model;
    obj.name        = name;
    return obj;
}
