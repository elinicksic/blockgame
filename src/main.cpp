#include "core.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "block.h"
#include "chunk.h"
#include "world.h"
#include "resource_manager.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Minecraft Clone";
GLFWwindow* window;
ResourceManager* resourceManager;
World* world;

static Camera camera;

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
	std::cout << "Minecraft\n";

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

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	resourceManager = new ResourceManager("./assets");
	world = new World(resourceManager);

	uint32_t myVaoId;
	glCreateVertexArrays(1, &myVaoId);
	glBindVertexArray(myVaoId);

	float lastX = 400, lastY = 300;
	bool isMouseDown = false;

	camera.position = glm::vec3(0.0f, 256.0f, 0.0f);

	std::tuple<int, int> prevChunk = {-1, 0};
	
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		resourceManager->getChunkShader()->bind();
		world->update();
		world->draw();

		float speed;

		if (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
			speed = 0.4f;
		} else {
			speed = 0.1f;
		}

		glm::vec3 forward = camera.front;
		forward.y = 0;

		if (Input::isKeyDown(GLFW_KEY_ESCAPE))
			break;
		if (Input::isKeyDown(GLFW_KEY_W))
			camera.position += speed * glm::normalize(forward);
		if (Input::isKeyDown(GLFW_KEY_S)) 
			camera.position -= speed * glm::normalize(forward);
		if (Input::isKeyDown(GLFW_KEY_A)) 
			camera.position -= glm::normalize(glm::cross(forward, camera.up)) * speed;
		if (Input::isKeyDown(GLFW_KEY_D))
			camera.position += glm::normalize(glm::cross(forward, camera.up)) * speed;
		if (Input::isKeyDown(GLFW_KEY_SPACE))
			camera.position += speed * camera.up;
		if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
			camera.position -= speed * camera.up;
		//camera.position += glm::vec3(0.0f, 0.1f, 0.0f);
		if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			if (!isMouseDown) {
				glm::vec3 cur = glm::floor(camera.position);
				glm::vec3 xs = camera.position;

				while (true) {
					if (glm::distance(xs, camera.position) > 5) break;

					if (world->getBlock((int) cur.x, (int) cur.y, (int) cur.z) != "air") {
						world->setBlock((int) cur.x, (int) cur.y, (int) cur.z, "air");
						isMouseDown = true;
						break;
					}

					float minM = 1000.0f;
					int minFace;
					glm::vec3 minNormal;

					for (int i = 0; i < 6; i++) {
						glm::vec3 normal;
						int normalSign;

						if (i == 0) {
							normal = glm::vec3(0.0f, 1.0f, 0.0f);
							normalSign = 1;
						} else if (i == 1) {
							normal = glm::vec3(1.0f, 0.0f, 0.0f);
							normalSign = 1;
						} else if (i == 2) {
							normal = glm::vec3(0.0f, 0.0f, 1.0f);
							normalSign = 1;
						} else if (i == 3) {
							normal = glm::vec3(-1.0f, 0.0f, 0.0f);
							normalSign = -1;
						} else if (i == 4) {
							normal = glm::vec3(0.0f, 0.0f, -1.0f);
							normalSign = -1;
						} else if (i == 5) {
							normal = glm::vec3(0.0f, -1.0f, 0.0f);
							normalSign = -1;
						}

						float proj_dv = glm::dot(glm::normalize(camera.front), normal);

						if (proj_dv <= 0) continue;

						float currPos;

						if (normal.x != 0.0f) {
							currPos = xs.x;
						} else if (normal.y != 0.0f) {
							currPos = xs.y;
						} else if (normal.z != 0.0f) {
							currPos = xs.z;
						}
						
						float nextPos = normalSign > 0 ? glm::floor(currPos + (float) normalSign) : glm::ceil(currPos + (float) normalSign);

						float deltaPos = glm::abs(nextPos - currPos);

						float m = deltaPos / proj_dv;

						if (i == 0 || m < minM) {
							minM = m;
							minFace = i;
							minNormal = normal;
						}
					}

					cur += minNormal;
					xs += glm::normalize(camera.front) * minM;
				}
			}
		} else {
			isMouseDown = false;
		}

		auto newChunk = world->getChunkCoords(camera.position.x, camera.position.z);

		if (prevChunk != newChunk) {
			prevChunk = newChunk;
			world->autoLoadChunks(std::get<0>(newChunk), std::get<1>(newChunk), 16);
		}
		
		float xoffset = Input::mouseX - lastX;
		float yoffset = lastY - Input::mouseY; // reversed since y-coordinates range from bottom to top
		lastX = Input::mouseX;
		lastY = Input::mouseY;

		const float sensitivity = 0.2f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.yaw += xoffset;
		camera.pitch += yoffset;

		camera.update(resourceManager->getChunkShader()->programId);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}