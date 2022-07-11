#include "block.h"

#define TOP 0
#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
#define BOTTOM 5

BlockData::BlockData(std::string path, ResourceManager *resourceManager) {
  YAML::Node blockInfo = YAML::LoadFile(path);
  name = blockInfo["name"].as<std::string>();

  YAML::Node textureNode = blockInfo["texture"];

  texCoords[TOP] = resourceManager->getSprite(textureNode["top"].as<std::string>());
  texCoords[NORTH] = resourceManager->getSprite(textureNode["north"].as<std::string>());
  texCoords[EAST] = resourceManager->getSprite(textureNode["east"].as<std::string>());
  texCoords[SOUTH] = resourceManager->getSprite(textureNode["south"].as<std::string>());
  texCoords[WEST] = resourceManager->getSprite(textureNode["west"].as<std::string>());
  texCoords[BOTTOM] = resourceManager->getSprite(textureNode["bottom"].as<std::string>());
}

Vertex* BlockData::getVerticies(int x, int y, int z, int face, Vertex* vertexArray) {
  TextureCoords textureCoords = texCoords[face];

  float tx = textureCoords.x;
  float ty = textureCoords.y;
  float tw = textureCoords.width;
  float th = textureCoords.height;

  glm::vec2 coordsArray[4] = {
    glm::vec2(tx, ty - th),
    glm::vec2(tx + tw, ty - th),
    glm::vec2(tx, ty),
    glm::vec2(tx + tw, ty),
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