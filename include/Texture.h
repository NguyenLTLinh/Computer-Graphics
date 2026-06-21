#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Texture {
public:
    Texture() = default;
    explicit Texture(const std::string& path, bool srgb = true);
    Texture(const glm::vec3& color, bool srgb = true);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void bind(GLuint unit = 0) const;
    GLuint id() const { return id_; }

private:
    GLuint id_{0};

    void createFromFile(const std::string& path, bool srgb);
    void createSolid(const glm::vec3& color, bool srgb);
    void configureSampler() const;
    void release();
};
