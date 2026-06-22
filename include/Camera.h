#pragma once

// ============================================================
// Camera.h - Camera góc nhìn tự do với Euler Angles
// Điều hướng WASD + chuột, sử dụng glm::lookAt
// ============================================================

#include "Common.h"

// Enum các hướng di chuyển (tách biệt khỏi phím cụ thể)
enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,    // Q - bay lên (hữu ích khi quan sát dưới nước)
    DOWN   // E - lặn xuống
};

class Camera {
public:
    // --------------------------------------------------------
    // Thuộc tính vị trí và định hướng
    // --------------------------------------------------------
    glm::vec3 position;
    glm::vec3 front;     // Vector hướng nhìn (tính từ Yaw/Pitch)
    glm::vec3 up;        // Vector "lên" của camera (world up mặc định)
    glm::vec3 right;     // Vector "phải" của camera

    // --------------------------------------------------------
    // Góc Euler
    // --------------------------------------------------------
    float yaw;    // Xoay ngang (độ) - mặc định -90 (nhìn về -Z)
    float pitch;  // Xoay dọc (độ) - giới hạn [-89, 89] tránh gimbal lock

    // --------------------------------------------------------
    // Cài đặt di chuyển
    // --------------------------------------------------------
    float moveSpeed;
    float mouseSensitivity;
    float fov; // Field of View (degrees)

    // --------------------------------------------------------
    // Constructor: Vị trí ban đầu nhìn vào trung tâm thành phố
    // --------------------------------------------------------
    explicit Camera(
        glm::vec3 startPosition = glm::vec3(0.0f, 8.0f, 30.0f),
        float     startYaw      = -90.0f,
        float     startPitch    = -10.0f
    );

    // --------------------------------------------------------
    // Lấy ma trận View (dùng glm::lookAt)
    // Gọi mỗi frame để cập nhật góc nhìn
    // --------------------------------------------------------
    glm::mat4 getViewMatrix() const;

    // --------------------------------------------------------
    // Lấy ma trận Projection
    // --------------------------------------------------------
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    // --------------------------------------------------------
    // Xử lý input bàn phím (gọi trong game loop với deltaTime)
    // deltaTime đảm bảo tốc độ di chuyển nhất quán trên mọi máy
    // --------------------------------------------------------
    void processKeyboard(CameraMovement direction, float deltaTime);

    // --------------------------------------------------------
    // Xử lý input chuột (xOffest, yOffset là pixels đã di chuyển)
    // --------------------------------------------------------
    void processMouseMovement(float xOffset, float yOffset,
                              bool constrainPitch = true);

    // Zoom bằng scroll wheel
    void processMouseScroll(float yOffset);

    // Getter vị trí hiện tại
    const glm::vec3& getPosition() const { return position; }

private:
    // Cập nhật lại vector front, right, up từ Yaw và Pitch
    // Gọi sau mỗi lần thay đổi góc Euler
    void updateCameraVectors();
};
