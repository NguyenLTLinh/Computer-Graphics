#include "Camera.h"
#include "Mesh.h"
#include "SceneFactory.h"
#include "Shader.h"
#include "Texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

Camera camera({0.0f, 2.2f, 10.0f});
float lastX = static_cast<float>(kWindowWidth) * 0.5f;
float lastY = static_cast<float>(kWindowHeight) * 0.5f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct RenderItem {
    Mesh* mesh;
    glm::mat4 model;
};

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow*, double xPos, double yPos) {
    const float x = static_cast<float>(xPos);
    const float y = static_cast<float>(yPos);

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    const float xOffset = x - lastX;
    const float yOffset = lastY - y;
    lastX = x;
    lastY = y;

    camera.processMouse(xOffset, yOffset);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::Forward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::Backward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::Left, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::Right, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::Up, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::Down, deltaTime);
    }
}

GLFWwindow* createWindow() {
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight, "The Sunken Capital", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    const GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        throw std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(glewStatus)));
    }
    glGetError();

    return window;
}

void setSharedLighting(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", camera.position());
    shader.setVec3("pointLight.position", glm::vec3(0.0f, 5.5f, 2.5f));
    shader.setVec3("pointLight.ambient", glm::vec3(0.02f, 0.15f, 0.13f));
    shader.setVec3("pointLight.diffuse", glm::vec3(0.23f, 0.62f, 0.58f));
    shader.setVec3("pointLight.specular", glm::vec3(0.55f, 0.90f, 0.82f));
    shader.setFloat("pointLight.constant", 1.0f);
    shader.setFloat("pointLight.linear", 0.10f);
    shader.setFloat("pointLight.quadratic", 0.045f);
    shader.setVec3("waterColor", glm::vec3(0.02f, 0.20f, 0.23f));
    shader.setFloat("fogDensity", 0.045f);
}
}

int main() {
    try {
        GLFWwindow* window = createWindow();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_FRAMEBUFFER_SRGB);

        Shader ruinShader("assets/shaders/ruin.vert", "assets/shaders/ruin.frag");
        Shader seaweedShader("assets/shaders/seaweed.vert", "assets/shaders/seaweed.frag");
        Shader bubbleShader("assets/shaders/bubble.vert", "assets/shaders/bubble.frag");

        Texture stoneTexture("assets/textures/ancient_stone_moss.png");
        Texture mossTexture("assets/textures/coral_moss_surface.png");

        Mesh block = createAncientBlock(3.0f, 2.4f, 2.1f);
        Mesh slab = createAncientBlock(4.2f, 0.35f, 2.9f);
        Mesh column = createColumn(0.25f, 3.0f, 32);
        Mesh seaweed = createSeaweedBlade(1.8f, 0.38f, 14);
        Mesh bubble = createBubbleQuad(0.16f);

        std::vector<RenderItem> ruins;
        ruins.push_back({&slab, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f))});
        ruins.push_back({&block, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.05f, 0.0f))});
        ruins.push_back({&slab, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.45f, 0.0f))});

        const std::array<glm::vec3, 4> columnPositions = {
            glm::vec3(-1.95f, 1.2f, -1.15f), glm::vec3(1.95f, 1.2f, -1.15f),
            glm::vec3(-1.95f, 1.2f,  1.15f), glm::vec3(1.95f, 1.2f,  1.15f)
        };
        for (const glm::vec3& p : columnPositions) {
            ruins.push_back({&column, glm::translate(glm::mat4(1.0f), p)});
        }

        const std::vector<glm::vec3> seaweedPositions = {
            {-3.2f, -0.38f, 1.7f}, {-2.7f, -0.38f, 2.2f}, {2.8f, -0.38f, 1.6f},
            {3.4f, -0.38f, 2.3f}, {-1.6f, -0.38f, -2.4f}, {1.5f, -0.38f, -2.6f}
        };

        const std::vector<glm::vec3> bubblePositions = {
            {-1.4f, 0.4f, 1.8f}, {-1.0f, 1.3f, 1.7f}, {1.3f, 0.8f, 1.5f}, {1.8f, 1.8f, 1.6f}
        };

        while (!glfwWindowShouldClose(window)) {
            const float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            const float aspect = static_cast<float>(width) / static_cast<float>(height > 0 ? height : 1);
            const glm::mat4 view = camera.viewMatrix();
            const glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()), aspect, 0.1f, 100.0f);

            glClearColor(0.01f, 0.09f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            setSharedLighting(ruinShader, view, projection);
            ruinShader.setVec3("material.diffuseColor", glm::vec3(0.42f, 0.48f, 0.41f));
            ruinShader.setVec3("material.specularColor", glm::vec3(0.22f, 0.34f, 0.31f));
            ruinShader.setFloat("material.shininess", 28.0f);
            ruinShader.setInt("diffuseMap", 0);
            stoneTexture.bind(0);
            for (const RenderItem& item : ruins) {
                ruinShader.setMat4("model", item.model);
                item.mesh->draw();
            }

            setSharedLighting(seaweedShader, view, projection);
            seaweedShader.setFloat("time", currentFrame);
            seaweedShader.setInt("diffuseMap", 0);
            mossTexture.bind(0);
            for (size_t i = 0; i < seaweedPositions.size(); ++i) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), seaweedPositions[i]);
                model = glm::rotate(model, 0.4f * static_cast<float>(i), glm::vec3(0.0f, 1.0f, 0.0f));
                seaweedShader.setFloat("phase", static_cast<float>(i) * 0.73f);
                seaweedShader.setMat4("model", model);
                seaweed.draw();
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_CULL_FACE);
            bubbleShader.use();
            bubbleShader.setMat4("view", view);
            bubbleShader.setMat4("projection", projection);
            bubbleShader.setFloat("time", currentFrame);
            bubbleShader.setVec3("waterColor", glm::vec3(0.02f, 0.20f, 0.23f));
            for (size_t i = 0; i < bubblePositions.size(); ++i) {
                glm::vec3 p = bubblePositions[i];
                p.y += std::fmod(currentFrame * (0.25f + 0.08f * static_cast<float>(i)), 2.6f);
                p.x += std::sin(currentFrame * 1.2f + static_cast<float>(i)) * 0.08f;
                bubbleShader.setMat4("model", glm::translate(glm::mat4(1.0f), p));
                bubbleShader.setFloat("phase", static_cast<float>(i) * 1.37f);
                bubble.draw();
            }
            glEnable(GL_CULL_FACE);
            glDisable(GL_BLEND);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }
}
