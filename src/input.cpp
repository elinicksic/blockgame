#include "input.h"

namespace Input {
bool keyPressedData[GLFW_KEY_LAST] = {};
bool mouseButtonPressedData[GLFW_MOUSE_BUTTON_LAST] = {};
float mouseX = 0.0f;
float mouseY = 0.0f;
float mouseScrollX = 0.0f;
float mouseScrollY = 0.0f;
int activeJoystick = -1;
GLFWgamepadstate gamepadState;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key >= 0 && key < GLFW_KEY_LAST) {
    if (action == GLFW_PRESS)
      keyPressedData[key] = true;
    else if (action == GLFW_RELEASE)
      keyPressedData[key] = false;
  }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  mouseX = (float)xpos;
  mouseY = (float)ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
    mouseButtonPressedData[button] = action == GLFW_PRESS;
  }
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  mouseScrollX = (float)xoffset;
  mouseScrollY = (float)yoffset;
}

void joystick_callback(int jid, int event) {
  std::cout << "Hello boomer\n";

  std::cout << jid << " " << event << std::endl;

  if (event == GLFW_CONNECTED) {
    activeJoystick = jid;
    std::cout << "Connected gampad " << jid << ": " << glfwGetGamepadName(jid) << std::endl;
    updateJoysticks();
  }

  if (event == 262146) {
    std::cout << "Disconnect gampad " << jid << std::endl;

    activeJoystick = -1;
  }
}

bool isKeyDown(int key) {
  if (key >= 0 && key < GLFW_KEY_LAST) {
    return keyPressedData[key];
  }

  return false;
}

bool isMouseButtonDown(int mouseButton) {
  if (mouseButton >= 0 && mouseButton < GLFW_MOUSE_BUTTON_LAST) {
    return mouseButtonPressedData[mouseButton];
  }

  return false;
}

void updateJoysticks() {
  if (activeJoystick == -1) return;

  glfwGetGamepadState(activeJoystick, &gamepadState);
}

glm::vec2 getTranslationAxis() {
  glm::vec2 keyboard(0.0, 0.0);

  if (isKeyDown(GLFW_KEY_W)) {
    keyboard.y += 1.0f;
  }

  if (isKeyDown(GLFW_KEY_S)) {
    keyboard.y -= 1.0f;
  }

  if (isKeyDown(GLFW_KEY_A)) {
    keyboard.x -= 1.0;
  }

  if (isKeyDown(GLFW_KEY_D)) {
    keyboard.x += 1.0;
  }

  if (glm::length(keyboard) != 0)
    keyboard = glm::normalize(keyboard);

  if (activeJoystick == -1) return keyboard;

  glm::vec2 joystick = glm::vec2(gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X], -gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);

  if (glm::length(joystick) < 0.05)
    joystick *= 0.0;

  // return glm::max(keyboard, joystick);

  return glm::length(keyboard) > glm::length(joystick) ? keyboard : joystick;
}

bool isKeyOrButton(int key, int button) {
  return isKeyDown(key) || (activeJoystick != -1 && gamepadState.buttons[button]);
}
}  // namespace Input