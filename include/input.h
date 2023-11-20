#pragma once

#include "core.h"

namespace Input {
    extern bool keyPressedData[GLFW_KEY_LAST];
    extern bool mouseButtonPressedData[GLFW_MOUSE_BUTTON_LAST];
    extern float mouseX;
    extern float mouseY;
    extern float mouseScrollX;
    extern float mouseScrollY;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void joystick_callback(int jid, int event);

    bool isKeyDown(int key);
    bool isMouseButtonDown(int mouseButton);

    void updateJoysticks();
    glm::vec2 getTranslationAxis();

    bool isKeyOrButton(int key, int button);
}