#include "window.h"
#include "input.h"

namespace Window {
	GLFWwindow* window = NULL;

	bool init(int width, int height, const char* title) {
		glfwSetErrorCallback(error_callback);

		if(!glfwInit()) {
			printf("Failed to initialize GLFW\n");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window) {
			std::cout << "Failed to create GLFW window\n";
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			printf("Coult not init OpenGL\n");
			return false;
		}

		glfwSetKeyCallback(window, Input::keyCallback);
		glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
		glfwSetCursorPosCallback(window, Input::mouseCallback);
		glfwSetScrollCallback(window, Input::mouseScrollCallback);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwSetJoystickCallback(Input::joystick_callback);
		glfwSetFramebufferSizeCallback(window, windowSizeCallback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDebugMessageCallback(GLDebugMessageCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glCullFace(GL_BACK);

		return true;
	}

	void error_callback(int error_code, const char* description) {
		//printf("GLFW Error: %s\n", description);
		std::cout << "GLFW Error: " << description << std::endl;
	}

	void windowSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data) {
		std::cout << "OpenGL Message: " << msg << std::endl;
	}

	void close() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void swap() {
		glfwSwapBuffers(window);
	}

	bool shouldWindowClose() {
		return glfwWindowShouldClose(window);
	}
}