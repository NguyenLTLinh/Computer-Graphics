#pragma once

// ============================================================
// SceneBuilder.h - Xây dựng kiến trúc "The Sunken Capital"
// Tạo cung điện trung tâm, hành lang, tháp canh và nền biển
// ============================================================

#include "Common.h"
#include "Mesh.h"
#include "Texture.h"
#include "Renderer.h"

class SceneBuilder {
public:
    // --------------------------------------------------------
    // Xây dựng toàn bộ scene, trả về danh sách RenderObject
    // và danh sách Point Light từ các mái vòm
    // --------------------------------------------------------
    static void buildScene(
        std::vector<RenderObject>& outObjects,
        std::vector<PointLight>&   outLights
    );

private:
    // -------- Các khối kiến trúc riêng lẻ --------

    // Cung điện trung tâm (tòa nhà chính với mái vòm lớn)
    static void buildCentralPalace(
        std::vector<RenderObject>& objects,
        std::vector<PointLight>&   lights,
        const glm::vec3&           origin
    );

    // Hành lang nối hai bên (colonnaded corridors)
    static void buildCorridor(
        std::vector<RenderObject>& objects,
        const glm::vec3&           start,
        const glm::vec3&           end,
        int                        numColumns
    );

    // Tháp canh với mái vòm phát sáng
    static void buildWatchTower(
        std::vector<RenderObject>& objects,
        std::vector<PointLight>&   lights,
        const glm::vec3&           position,
        float                      height,
        const glm::vec3&           glowColor
    );

    // Nền đại dương (ocean floor) với độ chi tiết cao
    static void buildOceanFloor(
        std::vector<RenderObject>& objects
    );

    // Thêm chi tiết trang trí (bậc thang, lan can...)
    static void buildStaircases(
        std::vector<RenderObject>& objects,
        const glm::vec3&           base,
        int                        steps
    );

    // -------- Helpers tạo Material --------
    static MaterialProperties stoneWallMaterial();
    static MaterialProperties mossyRockMaterial();
    static MaterialProperties glowingDomeMaterial(const glm::vec3& color);
    static MaterialProperties oceanFloorMaterial();

    // -------- Helper tạo RenderObject --------
    static RenderObject makeObject(
        std::shared_ptr<Mesh> mesh,
        const glm::mat4&      model,
        const std::string&    name
    );
};
