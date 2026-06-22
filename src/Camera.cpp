// ============================================================
// Camera.cpp - Triển khai Camera góc nhìn tự do
// ============================================================

#include "Camera.h"

Camera::Camera(glm::vec3 startPos, float startYaw, float startPitch)
    : position(startPos)
    , front(0.0f, 0.0f, -1.0f)
    , up(0.0f, 1.0f, 0.0f)
    , right(1.0f, 0.0f, 0.0f)
    , yaw(startYaw)
    , pitch(startPitch)
    , moveSpeed(CAMERA_MOVE_SPEED)
    , mouseSensitivity(CAMERA_MOUSE_SENS)
    , fov(CAMERA_FOV)
{
    updateCameraVectors(); // Tính toán front/right/up ban đầu
}

// ============================================================
// Ma trận View: glm::lookAt(eye, target, up)
// ============================================================
glm::mat4 Camera::getViewMatrix() const {
    // lookAt(position, position+front, up)
    // front đã được chuẩn hóa trong updateCameraVectors()
    return glm::lookAt(position, position + front, up);
}

// ============================================================
// Ma trận Projection: Perspective với aspect ratio
// ============================================================
glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(
        glm::radians(fov),     // Góc nhìn (rad)
        aspectRatio,           // Tỉ lệ màn hình
        CAMERA_NEAR_PLANE,     // Near clip plane
        CAMERA_FAR_PLANE       // Far clip plane
    );
}

// ============================================================
// Xử lý phím WASD + QE với Delta Time
// Delta time = thời gian frame trước để tốc độ đồng nhất
// ============================================================
void Camera::processKeyboard(CameraMovement dir, float deltaTime) {
    float velocity = moveSpeed * deltaTime;

    switch (dir) {
        case CameraMovement::FORWARD:
            position += front * velocity;
            break;
        case CameraMovement::BACKWARD:
            position -= front * velocity;
            break;
        case CameraMovement::LEFT:
            position -= right * velocity;
            break;
        case CameraMovement::RIGHT:
            position += right * velocity;
            break;
        case CameraMovement::UP:
            // Bay lên theo world up (không theo camera up)
            position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
            break;
        case CameraMovement::DOWN:
            position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
            break;
    }
}

// ============================================================
// Xử lý di chuyển chuột: cập nhật Yaw và Pitch
// ============================================================
void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw   += xOffset;
    pitch += yOffset;

    // Giới hạn pitch để tránh gimbal lock và đảo ngược màn hình
    if (constrainPitch) {
        if (pitch >  89.0f) pitch =  89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    // Cập nhật vectors sau khi thay đổi góc
    updateCameraVectors();
}

// ============================================================
// Zoom bằng scroll wheel
// ============================================================
void Camera::processMouseScroll(float yOffset) {
    fov -= yOffset * 2.0f;
    if (fov < 10.0f)  fov = 10.0f;
    if (fov > 90.0f)  fov = 90.0f;
}

// ============================================================
// Tính lại front, right, up từ Yaw và Pitch (Euler Angles)
// Đây là trái tim của hệ thống camera
// ============================================================
void Camera::updateCameraVectors() {
    // Tính vector front từ góc Euler (spherical coordinates -> cartesian)
    glm::vec3 newFront;
    newFront.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    newFront.y = sinf(glm::radians(pitch));
    newFront.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

    // Chuẩn hóa để luôn là unit vector
    front = glm::normalize(newFront);

    // right = cross(front, worldUp) - vector vuông góc với front trong mặt phẳng ngang
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    // up thực của camera = cross(right, front)
    up    = glm::normalize(glm::cross(right, front));
}
