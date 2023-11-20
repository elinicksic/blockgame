#include "core.h"

namespace Window {
extern GLFWwindow* window;
void error_callback(int error_code, const char* description);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);
bool init(int width, int height, const char* title);
void close();
void swap();
bool shouldWindowClose();
};  // namespace Window