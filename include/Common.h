#pragma once

// ============================================================
// Common.h - Các định nghĩa dùng chung toàn dự án
// The Sunken Capital - OpenGL 3.3 Core Profile
// ============================================================

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

// ============================================================
// Kích thước cửa sổ mặc định
// ============================================================
constexpr int   WINDOW_WIDTH  = 1280;
constexpr int   WINDOW_HEIGHT = 720;
constexpr char  WINDOW_TITLE[] = "The Sunken Capital";

// ============================================================
// Cài đặt Camera
// ============================================================
constexpr float CAMERA_MOVE_SPEED   = 5.0f;   // Đơn vị/giây
constexpr float CAMERA_MOUSE_SENS   = 0.08f;  // Độ nhạy chuột
constexpr float CAMERA_FOV          = 60.0f;  // Góc nhìn (độ)
constexpr float CAMERA_NEAR_PLANE   = 0.1f;
constexpr float CAMERA_FAR_PLANE    = 500.0f;

// ============================================================
// Màu sắc môi trường đại dương (RGB normalized)
// ============================================================
constexpr glm::vec3 OCEAN_AMBIENT_COLOR  = glm::vec3(0.02f, 0.05f, 0.12f); // Xanh thẳm
constexpr glm::vec3 OCEAN_FOG_COLOR      = glm::vec3(0.04f, 0.09f, 0.18f); // Màu sương mù nước
constexpr float     OCEAN_FOG_DENSITY    = 0.018f;

// ============================================================
// Struct chứa thông tin một Point Light
// Dùng cho các điểm sáng vàng tại mái vòm tòa nhà
// ============================================================
struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float     intensity;

    // Hệ số suy giảm ánh sáng (Attenuation)
    // Công thức: 1.0 / (constant + linear*d + quadratic*d^2)
    float constant;   // Thường = 1.0
    float linear;     // Kiểm soát suy giảm gần
    float quadratic;  // Kiểm soát suy giảm xa
};

// ============================================================
// Struct thông tin ánh sáng hướng (Directional - "god rays")
// Ánh sáng từ mặt nước chiếu xuống
// ============================================================
struct DirectionalLight {
    glm::vec3 direction;  // Hướng ánh sáng (đi xuống)
    glm::vec3 color;
    float     intensity;
};

// ============================================================
// Macro kiểm tra lỗi OpenGL (dùng trong debug)
// ============================================================
#ifdef _DEBUG
  #define GL_CHECK(call)                                          \
      do {                                                        \
          call;                                                   \
          GLenum err = glGetError();                              \
          if (err != GL_NO_ERROR) {                               \
              std::cerr << "[GL ERROR] " << err                   \
                        << " at " << __FILE__                     \
                        << ":" << __LINE__ << std::endl;          \
          }                                                       \
      } while(0)
#else
  #define GL_CHECK(call) call
#endif
