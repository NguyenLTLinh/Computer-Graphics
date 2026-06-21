#pragma once

#include "Vertex.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void draw() const;
    GLsizei indexCount() const { return indexCount_; }

private:
    GLuint vao_{0};
    GLuint vbo_{0};
    GLuint ibo_{0};
    GLsizei indexCount_{0};

    void upload(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void release();
};
