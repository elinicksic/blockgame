#include "core.h"
#include "shader.h"
#include "camera.h"
#include "input.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Minecraft Clone";
GLFWwindow* window;

static Shader shader;
static Camera camera;

struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
};

void error_callback(int error_code, const char* description) {
	printf("GLFW Error: %s\n", description);
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void create_window(int width, int height, const char* title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		printf("Failed to create GLFW window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
}

int main() {
	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		return -1;
	}

	create_window(WIDTH, HEIGHT, TITLE);

	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::mouseCallback);
	glfwSetScrollCallback(window, Input::mouseScrollCallback);
	glfwSetFramebufferSizeCallback(window, windowSizeCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Coult not init OpenGL\n");
        return -1;
    }

	shader.compile();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	shader.bind();

	Vertex myVertexArray[8] = {
		{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, // 0 Front Bottom Right
		{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)}, // 1 Front Top Right
		{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, // 2 Front Bottom Left
		{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)}, // 3 Front Top Left
		{glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, // 4 Back Bottom Right
		{glm::vec3(0.0f, 1.0f, 2.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)}, // 5 Back Top Right
		{glm::vec3(1.0f, 0.0f, 2.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, // 6 Back Bottom Left
		{glm::vec3(1.0f, 1.0f, 2.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)}, // 7 Back Top Left
	};
	// 3    1   7    5
	// 
	// 2    0   6    4

	//   7    5
	// 3    1 
	//   6    4 
	// 2    0  

	static uint32_t myElementArray[36] = {
		// Front
		0, 1, 2,
		1, 3, 2,
		// Right
		0, 4, 5,
		0, 1, 5,
		// Top
		1, 3, 7,
		1, 5, 7,
		// Left
		2, 3, 6,
		3, 7, 6,
		// Bottom
		0, 4, 6,
		0, 2, 6,
		// Back
		6, 7, 5,
		6, 4, 5
	};

	uint32_t myVaoId;
	glCreateVertexArrays(1, &myVaoId);
	glBindVertexArray(myVaoId);

	uint32_t myVboId;
	glGenBuffers(1, &myVboId);
	glBindBuffer(GL_ARRAY_BUFFER, myVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myVertexArray), myVertexArray, GL_STATIC_DRAW);

	// Position Vertex Attribute
	glVertexAttribPointer (
		0, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex), 
		(void*)offsetof(Vertex, position)
	);
	glEnableVertexAttribArray(0);
	
	// Color Vertex Attribute
	glVertexAttribPointer (
		1, 
		4, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex), 
		(void*)offsetof(Vertex, color)
	);
	glEnableVertexAttribArray(1);

	uint32_t myEboId;
	glGenBuffers(1, &myEboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(myElementArray), myElementArray, GL_STATIC_DRAW);

	float lastX = 400, lastY = 300;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		float speed = 0.05f;

		if (Input::isKeyDown(GLFW_KEY_W)) 
			camera.position += speed * camera.front;
		if (Input::isKeyDown(GLFW_KEY_S)) 
			camera.position -= speed * camera.front;
		if (Input::isKeyDown(GLFW_KEY_A)) 
			camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * speed;
		if (Input::isKeyDown(GLFW_KEY_D))
			camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * speed;
		
		float xoffset = Input::mouseX - lastX;
		float yoffset = lastY - Input::mouseY; // reversed since y-coordinates range from bottom to top
		lastX = Input::mouseX;
		lastY = Input::mouseY;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.yaw += xoffset;
		camera.pitch += yoffset;

		camera.update(shader.programId);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}