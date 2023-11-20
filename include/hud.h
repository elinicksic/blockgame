#include "core.h"
#include "resource_manager.h"

struct HUDVertex {
  glm::vec2 pos;
  glm::vec4 color;
};

class HUD {
 private:
  ResourceManager* resourceManager;
  GLuint vbo;
  GLuint vao;
  int bufferSize;

 public:
  HUD(ResourceManager* resourceManager);
  void render();
};