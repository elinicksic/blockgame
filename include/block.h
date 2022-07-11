#pragma once

#include "core.h"
#include "texture.h"
#include "resource_manager.h"
#include <yaml-cpp/yaml.h>

struct Vertex {
  glm::vec3 position;
  glm::vec2 textureCoords;
  float lighting;
};

class ResourceManager;

class BlockData {
  public:
    BlockData(std::string path, ResourceManager *texture);
    
    std::string name;
    TextureCoords texCoords[6];

    Vertex* getVerticies(int x, int y, int z, int face, Vertex* vertexArray);
};