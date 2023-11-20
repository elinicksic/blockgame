#include "block.h"

#define TOP 0
#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
#define BOTTOM 5

BlockData::BlockData(std::string path, ResourceManager* resourceManager) {
  YAML::Node blockInfo = YAML::LoadFile(path);
  name = blockInfo["name"].as<std::string>();

  YAML::Node textureNode = blockInfo["texture"];

  texDepth[TOP] = resourceManager->getTextureDepth(textureNode["top"].as<std::string>());
  texDepth[NORTH] = resourceManager->getTextureDepth(textureNode["north"].as<std::string>());
  texDepth[EAST] = resourceManager->getTextureDepth(textureNode["east"].as<std::string>());
  texDepth[SOUTH] = resourceManager->getTextureDepth(textureNode["south"].as<std::string>());
  texDepth[WEST] = resourceManager->getTextureDepth(textureNode["west"].as<std::string>());
  texDepth[BOTTOM] = resourceManager->getTextureDepth(textureNode["bottom"].as<std::string>());
}

Vertex* BlockData::getVerticies(int x, int y, int z, int face, Vertex* vertexArray) {
  int textureDepth = texDepth[face];

  glm::vec3 coordsArray[4] = {
      glm::vec3(0.0f, 0.0f, textureDepth),
      glm::vec3(1.0f, 0.0f, textureDepth),
      glm::vec3(0.0f, 1.0f, textureDepth),
      glm::vec3(1.0f, 1.0f, textureDepth),
  };

  float lighting = 0.0f;

  if (face == SOUTH || face == WEST || face == BOTTOM) {
    lighting = 0.4f;
  }

  if (face == 0) {
    vertexArray[0] = {glm::vec3(0.0f, 1.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[0], lighting};
    vertexArray[1] = {glm::vec3(0.0f, 1.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[1], lighting};
    vertexArray[2] = {glm::vec3(1.0f, 1.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[2], lighting};
    vertexArray[4] = {glm::vec3(1.0f, 1.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[3], lighting};
    vertexArray[3] = vertexArray[1];
    vertexArray[5] = vertexArray[2];
  } else if (face == 1) {
    vertexArray[0] = {glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[0], lighting};
    vertexArray[1] = {glm::vec3(0.0f, 0.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[1], lighting};
    vertexArray[2] = {glm::vec3(0.0f, 1.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[2], lighting};
    vertexArray[4] = {glm::vec3(0.0f, 1.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[3], lighting};
    vertexArray[3] = vertexArray[1];
    vertexArray[5] = vertexArray[2];
  } else if (face == 2) {
    vertexArray[0] = {glm::vec3(0.0f, 0.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[0], lighting};
    vertexArray[1] = {glm::vec3(1.0f, 0.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[1], lighting};
    vertexArray[2] = {glm::vec3(0.0f, 1.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[2], lighting};
    vertexArray[4] = {glm::vec3(1.0f, 1.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[3], lighting};
    vertexArray[3] = vertexArray[1];
    vertexArray[5] = vertexArray[2];
  } else if (face == 3) {
    vertexArray[0] = {glm::vec3(1.0f, 1.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[2], lighting};
    vertexArray[1] = {glm::vec3(1.0f, 0.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[1], lighting};
    vertexArray[2] = {glm::vec3(1.0f, 0.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[0], lighting};
    vertexArray[3] = vertexArray[0];
    vertexArray[4] = {glm::vec3(1.0f, 1.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[3], lighting};
    vertexArray[5] = vertexArray[1];
  } else if (face == 4) {
    vertexArray[0] = {glm::vec3(0.0f, 1.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[2], lighting};
    vertexArray[1] = {glm::vec3(1.0f, 0.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[1], lighting};
    vertexArray[2] = {glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[0], lighting};
    vertexArray[3] = vertexArray[0];
    vertexArray[4] = {glm::vec3(1.0f, 1.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[3], lighting};
    vertexArray[5] = vertexArray[1];
  } else if (face == 5) {
    vertexArray[0] = {glm::vec3(1.0f, 0.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[2], lighting};
    vertexArray[1] = {glm::vec3(0.0f, 0.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[1], lighting};
    vertexArray[2] = {glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(x, y, z), coordsArray[0], lighting};
    vertexArray[3] = vertexArray[0];
    vertexArray[4] = {glm::vec3(1.0f, 0.0f, 1.0f) + glm::vec3(x, y, z), coordsArray[3], lighting};
    vertexArray[5] = vertexArray[1];
  }

  return vertexArray;
}
