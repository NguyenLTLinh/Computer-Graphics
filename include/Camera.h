#pragma once

#include <glm/glm.hpp>

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

class Camera {
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 2.0f, 8.0f));

    glm::mat4 viewMatrix() const;

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouse(float xOffset, float yOffset, bool constrainPitch = true);

    glm::vec3 position() const { return position_; }
    glm::vec3 front() const { return front_; }

    float zoom() const { return zoom_; }
    float yaw() const { return yaw_; }
    float pitch() const { return pitch_; }

private:
    glm::vec3 position_;
    glm::vec3 front_{0.0f, 0.0f, -1.0f};
    glm::vec3 up_{0.0f, 1.0f, 0.0f};
    glm::vec3 right_{1.0f, 0.0f, 0.0f};
    glm::vec3 worldUp_{0.0f, 1.0f, 0.0f};

    float yaw_{-90.0f};
    float pitch_{-8.0f};
    float movementSpeed_{4.2f};
    float mouseSensitivity_{0.08f};
    float zoom_{45.0f};

    void updateVectors();
};
