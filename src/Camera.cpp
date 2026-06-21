#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

Camera::Camera(glm::vec3 position) : position_(position) {
    updateVectors();
}

glm::mat4 Camera::viewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    const float velocity = movementSpeed_ * deltaTime;

    if (direction == CameraMovement::Forward) {
        position_ += front_ * velocity;
    }
    if (direction == CameraMovement::Backward) {
        position_ -= front_ * velocity;
    }
    if (direction == CameraMovement::Left) {
        position_ -= right_ * velocity;
    }
    if (direction == CameraMovement::Right) {
        position_ += right_ * velocity;
    }
    if (direction == CameraMovement::Up) {
        position_ += worldUp_ * velocity;
    }
    if (direction == CameraMovement::Down) {
        position_ -= worldUp_ * velocity;
    }
}

void Camera::processMouse(float xOffset, float yOffset, bool constrainPitch) {
    yaw_ += xOffset * mouseSensitivity_;
    pitch_ += yOffset * mouseSensitivity_;

    if (constrainPitch) {
        pitch_ = std::clamp(pitch_, -89.0f, 89.0f);
    }

    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw_)) * std::cos(glm::radians(pitch_));
    front.y = std::sin(glm::radians(pitch_));
    front.z = std::sin(glm::radians(yaw_)) * std::cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, worldUp_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
