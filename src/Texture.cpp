#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <stdexcept>
#include <utility>

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

Texture::Texture(const std::string& path, bool srgb) {
    createFromFile(path, srgb);
}

Texture::Texture(const glm::vec3& color, bool srgb) {
    createSolid(color, srgb);
}

Texture::~Texture() {
    release();
}

Texture::Texture(Texture&& other) noexcept : id_(std::exchange(other.id_, 0)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        release();
        id_ = std::exchange(other.id_, 0);
    }
    return *this;
}

void Texture::bind(GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::createFromFile(const std::string& path, bool srgb) {
    stbi_set_flip_vertically_on_load(true);

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Cannot load texture: " + path);
    }

    GLenum dataFormat = GL_RGB;
    GLenum internalFormat = srgb ? GL_SRGB8 : GL_RGB8;
    if (channels == 4) {
        dataFormat = GL_RGBA;
        internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
    } else if (channels == 1) {
        dataFormat = GL_RED;
        internalFormat = GL_R8;
    }

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    configureSampler();

    stbi_image_free(data);
}

void Texture::createSolid(const glm::vec3& color, bool srgb) {
    const auto channel = [](float value) {
        return static_cast<unsigned char>(std::clamp(value, 0.0f, 1.0f) * 255.0f);
    };

    const unsigned char pixel[] = {
        channel(color.r),
        channel(color.g),
        channel(color.b),
        255
    };

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glGenerateMipmap(GL_TEXTURE_2D);
    configureSampler();
}

void Texture::configureSampler() const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef GLEW_EXT_texture_filter_anisotropic
    if (GLEW_EXT_texture_filter_anisotropic) {
        GLfloat maxAnisotropy = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(maxAnisotropy, 8.0f));
    }
#endif
}

void Texture::release() {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
}
