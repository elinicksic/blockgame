#include "bgutils.h"
#include "block.h"
#include "core.h"
#include "entity.h"
#include "hud.h"
#include "input.h"
#include "rendering/camera.h"
#include "rendering/shader.h"
#include "resource_manager.h"
#include "window.h"
#include "world/chunk.h"
#include "world/world.h"

#define MS_PER_TICK 50

ResourceManager* resourceManager;
World* world;
Entity* player;
HUD* hud;

static Camera camera;

void update();
void render();
void handleInput();

int main() {
  const char* title = "Block Game";

  if (!Window::init(800, 600, title)) {
    std::cout << "Failed to open window\n";
    return -1;
  }

  resourceManager = new ResourceManager("./assets");
  world = new World(resourceManager);

  uint32_t myVaoId;
  glCreateVertexArrays(1, &myVaoId);
  glBindVertexArray(myVaoId);

  player = world->spawnEntity("player", 0.0f, 155.0f, 0.0f);

  auto previous = std::chrono::system_clock::now();

  int lag = 0;

  int frames = 0;
  auto lastFpsCount = std::chrono::system_clock::now();

  resourceManager->getChunkShader()->bind();
  resourceManager->bindTexture();

  hud = new HUD(resourceManager);

  while (!Window::shouldWindowClose()) {
    auto current = std::chrono::system_clock::now();
    auto elapsed = current - previous;
    previous = current;
    lag += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

    handleInput();

    while (lag >= MS_PER_TICK) {
      update();
      lag -= MS_PER_TICK;
    }

    render();

    frames++;
  }

  Window::close();

  return 0;
}

float lastX = 400, lastY = 300;
bool isMouseDown = false;
int jumpCooldown = 0;

std::tuple<int, int> prevChunk = {-1, 0};

void handleInput() {
  glfwPollEvents();
  Input::updateJoysticks();

  float xoffset = Input::mouseX - lastX;
  float yoffset = lastY - Input::mouseY;  // reversed since y-coordinates range from bottom to top
  lastX = Input::mouseX;
  lastY = Input::mouseY;

  const float sensitivity = 0.2f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  camera.yaw += xoffset;
  camera.pitch += yoffset;

  resourceManager->getChunkShader()->bind();
  camera.update(resourceManager->getChunkShader()->programId);
}

void update() {
  world->update();

  float speed = 0.2;

  if (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
    speed *= 1.5f;
  }

  if (jumpCooldown > 0) jumpCooldown--;

  glm::vec2 axes = Input::getTranslationAxis();

  glm::vec3 forward(cos(glm::radians(camera.yaw)), 0.0, sin(glm::radians(camera.yaw)));
  glm::vec3 movementVector = glm::normalize(forward) * axes.y + glm::normalize(glm::cross(forward, camera.up)) * axes.x;

  player->addForce(movementVector * speed);

  if (Input::isKeyDown(GLFW_KEY_SPACE) && player->isOnGround && jumpCooldown == 0) {
    jumpCooldown = 8;
    player->addForce(glm::vec3(0.0f, 0.8, 0.0f));
  }
  if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
    player->addForce(glm::vec3(0.0f, -1.0f, 0.0f));
  // camera.position += glm::vec3(0.0f, 0.1f, 0.0f);

  camera.position = player->pos + glm::vec3(0.0f, 1.6f, 0.0f);

  if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
    if (!isMouseDown) {
      Utils::BlockFace blockface = Utils::raycast(camera.position, camera.front, world, 5.0f);

      if (!blockface.isAir) {
        world->setBlock(blockface.x, blockface.y, blockface.z, "air");
      }

      isMouseDown = true;
    }
  } else {
    isMouseDown = false;
  }

  auto newChunk = world->getChunkCoords(camera.position.x, camera.position.z);

  if (prevChunk != newChunk) {
    prevChunk = newChunk;
    world->autoLoadChunks(std::get<0>(newChunk), std::get<1>(newChunk), 24);
  }
}

void render() {
  glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  world->draw();
  // hud->render();

  Window::swap();
}