#include "SceneFactory.h"

#include <glm/gtc/constants.hpp>

#include <cmath>
#include <utility>
#include <vector>

Mesh createAncientBlock(float width, float height, float depth) {
    const float x = width * 0.5f;
    const float y = height * 0.5f;
    const float z = depth * 0.5f;

    std::vector<Vertex> vertices = {
        {{-x, -y,  z}, { 0,  0,  1}, {0, 0}}, {{ x, -y,  z}, { 0,  0,  1}, {1, 0}}, {{ x,  y,  z}, { 0,  0,  1}, {1, 1}}, {{-x,  y,  z}, { 0,  0,  1}, {0, 1}},
        {{ x, -y, -z}, { 0,  0, -1}, {0, 0}}, {{-x, -y, -z}, { 0,  0, -1}, {1, 0}}, {{-x,  y, -z}, { 0,  0, -1}, {1, 1}}, {{ x,  y, -z}, { 0,  0, -1}, {0, 1}},
        {{-x, -y, -z}, {-1,  0,  0}, {0, 0}}, {{-x, -y,  z}, {-1,  0,  0}, {1, 0}}, {{-x,  y,  z}, {-1,  0,  0}, {1, 1}}, {{-x,  y, -z}, {-1,  0,  0}, {0, 1}},
        {{ x, -y,  z}, { 1,  0,  0}, {0, 0}}, {{ x, -y, -z}, { 1,  0,  0}, {1, 0}}, {{ x,  y, -z}, { 1,  0,  0}, {1, 1}}, {{ x,  y,  z}, { 1,  0,  0}, {0, 1}},
        {{-x,  y,  z}, { 0,  1,  0}, {0, 0}}, {{ x,  y,  z}, { 0,  1,  0}, {1, 0}}, {{ x,  y, -z}, { 0,  1,  0}, {1, 1}}, {{-x,  y, -z}, { 0,  1,  0}, {0, 1}},
        {{-x, -y, -z}, { 0, -1,  0}, {0, 0}}, {{ x, -y, -z}, { 0, -1,  0}, {1, 0}}, {{ x, -y,  z}, { 0, -1,  0}, {1, 1}}, {{-x, -y,  z}, { 0, -1,  0}, {0, 1}},
    };

    std::vector<unsigned int> indices = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    return Mesh(std::move(vertices), std::move(indices));
}

Mesh createColumn(float radius, float height, int segments) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    vertices.reserve(static_cast<size_t>((segments + 1) * 2));
    indices.reserve(static_cast<size_t>(segments * 6));

    for (int i = 0; i <= segments; ++i) {
        const float angle = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
        const float cx = std::cos(angle);
        const float cz = std::sin(angle);
        const glm::vec3 normal = glm::normalize(glm::vec3(cx, 0.0f, cz));
        const float u = static_cast<float>(i) / static_cast<float>(segments);

        vertices.push_back({{radius * cx, -height * 0.5f, radius * cz}, normal, {u, 0.0f}});
        vertices.push_back({{radius * cx,  height * 0.5f, radius * cz}, normal, {u, 1.0f}});
    }

    for (int i = 0; i < segments; ++i) {
        const unsigned int base = static_cast<unsigned int>(i * 2);
        indices.insert(indices.end(), {base, base + 1, base + 2, base + 2, base + 1, base + 3});
    }

    return Mesh(std::move(vertices), std::move(indices));
}

Mesh createSeaweedBlade(float height, float width, int joints) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    vertices.reserve(static_cast<size_t>((joints + 1) * 2));

    for (int i = 0; i <= joints; ++i) {
        const float t = static_cast<float>(i) / static_cast<float>(joints);
        const float taper = 1.0f - t * 0.72f;
        const float halfWidth = width * taper * 0.5f;
        const float y = height * t;
        vertices.push_back({{-halfWidth, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, t}});
        vertices.push_back({{ halfWidth, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, t}});
    }

    for (int i = 0; i < joints; ++i) {
        const unsigned int base = static_cast<unsigned int>(i * 2);
        indices.insert(indices.end(), {base, base + 1, base + 2, base + 2, base + 1, base + 3});
    }

    return Mesh(std::move(vertices), std::move(indices));
}

Mesh createBubbleQuad(float size) {
    const float s = size * 0.5f;
    std::vector<Vertex> vertices = {
        {{-s, -s, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ s, -s, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{ s,  s, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-s,  s, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    };
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
    return Mesh(std::move(vertices), std::move(indices));
}
