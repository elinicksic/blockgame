#pragma once

#include <yaml-cpp/yaml.h>

#include "core.h"
#include "resource_manager.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 textureCoords;
  float lighting;
};

class ResourceManager;

class BlockData {
 public:
  BlockData(std::string path, ResourceManager* texture);

  std::string name;
  int texDepth[6];

  Vertex* getVerticies(int x, int y, int z, int face, Vertex* vertexArray);
};