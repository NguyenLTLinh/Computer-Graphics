// ============================================================
// Texture.cpp - Nạp và quản lý OpenGL Textures với stb_image
// ============================================================

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

// ============================================================
// Constructor: Nạp texture từ file
// ============================================================
Texture::Texture(const std::string& imagePath,
                 GLenum wrapMode,
                 GLenum filterMin,
                 GLenum filterMag)
    : filePath(imagePath)
{
    loadFromFile(imagePath, wrapMode, filterMin, filterMag);
}

// ============================================================
// Tạo texture màu đơn sắc (fallback khi không có file)
// ============================================================
Texture Texture::createSolid(glm::vec4 color) {
    Texture tex;
    tex.width = tex.height = 1;
    tex.channels = 4;
    tex.filePath = "solid_color";

    unsigned char pixel[4] = {
        static_cast<unsigned char>(color.r * 255),
        static_cast<unsigned char>(color.g * 255),
        static_cast<unsigned char>(color.b * 255),
        static_cast<unsigned char>(color.a * 255)
    };

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

Texture::~Texture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}

// ============================================================
// Bind texture vào một texture unit
// Slot 0 = GL_TEXTURE0 (diffuse), Slot 1 = GL_TEXTURE1 (emissive)
// ============================================================
void Texture::bind(GLuint slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ============================================================
// Nạp ảnh từ file và upload lên GPU
// ============================================================
void Texture::loadFromFile(const std::string& path, GLenum wrap,
                           GLenum filterMin, GLenum filterMag) {
    // stb_image: flip theo trục Y để khớp với UV convention của OpenGL
    // OpenGL: UV(0,0) = góc dưới-trái. stb: (0,0) = góc trên-trái
    stbi_set_flip_vertically_on_load(true);

    // Nạp ảnh từ đĩa
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "[Texture] WARN: Failed to load '" << path
                  << "'. Using fallback white texture.\n";
        // Tạo texture trắng fallback thay vì crash
        *this = Texture::createSolid(glm::vec4(1.0f));
        return;
    }

    // Xác định format dựa trên số channels
    GLenum internalFmt, pixelFmt;
    if (channels == 1) {
        internalFmt = pixelFmt = GL_RED;
    } else if (channels == 3) {
        internalFmt = GL_RGB;
        pixelFmt    = GL_RGB;
    } else { // channels == 4
        internalFmt = GL_RGBA;
        pixelFmt    = GL_RGBA;
    }

    // Upload lên GPU
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, width, height, 0,
                 pixelFmt, GL_UNSIGNED_BYTE, data);

    // Tạo mipmaps tự động: giảm aliasing khi texture ở xa camera
    // Các mức mipmap: 1x1, 2x2, 4x4, 8x8, ... đến kích thước gốc
    glGenerateMipmap(GL_TEXTURE_2D);

    // Thiết lập chế độ wrap (GL_REPEAT cho tile texture đá cổ)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    // Filter khi thu nhỏ: dùng mipmap (GL_LINEAR_MIPMAP_LINEAR = trilinear)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);

    // Filter khi phóng to: GL_LINEAR cho smooth
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

    // Anisotropic filtering nếu extension có sẵn (giảm mờ ở góc nghiêng)
    float maxAniso = 1.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
    if (maxAniso > 1.0f) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                        std::min(maxAniso, 8.0f)); // Tối đa 8x
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data); // Giải phóng bộ nhớ CPU

    std::cout << "[Texture] Loaded: " << path
              << " (" << width << "x" << height << ", " << channels << "ch)\n";
}
