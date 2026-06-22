#pragma once

// ============================================================
// Texture.h - Nạp và quản lý OpenGL Textures
// Sử dụng stb_image, tự động tạo Mipmaps
// ============================================================

#include "Common.h"

class Texture {
public:
    GLuint      id       = 0;       // Texture ID trên GPU
    std::string filePath;           // Đường dẫn file (để debug)
    int         width    = 0;
    int         height   = 0;
    int         channels = 0;       // Số kênh màu (3=RGB, 4=RGBA)

    // --------------------------------------------------------
    // Constructor: Nạp texture từ file
    // slot: texture unit để bind (GL_TEXTURE0, GL_TEXTURE1...)
    // --------------------------------------------------------
    explicit Texture(const std::string& imagePath,
                     GLenum wrapMode  = GL_REPEAT,
                     GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, // Dùng mipmap
                     GLenum filterMag = GL_LINEAR);

    // Tạo texture màu đơn giản (dùng khi chưa có file texture)
    static Texture createSolid(glm::vec4 color);

    // Destructor: Giải phóng texture trên GPU
    ~Texture();

    // Không copy
    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    // Bind texture vào texture unit chỉ định
    void bind(GLuint slot = 0) const;

    // Unbind
    void unbind() const;

    bool isValid() const { return id != 0; }

private:
    void loadFromFile(const std::string& path, GLenum wrap, GLenum min, GLenum mag);
};
