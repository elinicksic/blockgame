#include "resource_manager.h"

BlockData ResourceManager::getBlock(std::string name) {
  return blocks.at(name);
}

int ResourceManager::getTextureDepth(std::string name) {
  return textures.at(name);
}

ResourceManager::ResourceManager(std::string assetsPath) {
  this -> assetsPath = assetsPath;

  loadShaders();
  loadTextures();
  loadBlocks();
  loadEntities();
}

void ResourceManager::loadBlocks() {
  for (const auto & entry : std::filesystem::directory_iterator(assetsPath / "data" / "blocks")) {
    BlockData block(entry.path(), this);
    blocks.insert({block.name, block});
  }
}

void ResourceManager::loadTextures() {
  std::vector<std::string> texturePaths;

  for (const auto &entry : std::filesystem::directory_iterator(assetsPath / "texture" / "blocks")) {
    if (entry.path().extension() != ".png") continue;

    textures.insert({entry.path().stem(), texturePaths.size()});
    texturePaths.push_back(entry.path());
  }

  arrayTexture = new ArrayTexture(texturePaths);
}

void ResourceManager::loadShaders() {
  chunkShader = loadShader("chunk");
  hudShader = loadShader("hud");
}

Shader ResourceManager::loadShader(std::string name) {
  std::ifstream fragStream;
  fragStream.open(assetsPath / "shaders" / (name + ".frag"));
  std::string fragSrc;

  if (fragStream.is_open()) {
    std::stringstream buffer;
    buffer << fragStream.rdbuf();

    fragSrc = buffer.str();
    fragStream.close();
  }
  
  std::ifstream vertStream;
  vertStream.open(assetsPath / "shaders" / name.append(".vert"));
  std::string vertSrc;

  if (vertStream.is_open()) {
    std::stringstream buffer;
    buffer << vertStream.rdbuf();

    vertSrc = buffer.str();

    vertStream.close();
  }

  Shader shader;
  shader.compile(fragSrc, vertSrc);

  return shader;
}

Shader* ResourceManager::getChunkShader() {
  return &chunkShader;
}

Shader* ResourceManager::getHudShader() {
  return &hudShader;
}

void ResourceManager::bindTexture() {
  arrayTexture->bind(chunkShader.programId);
}

void ResourceManager::loadEntities() {
  for (const auto &entry : std::filesystem::directory_iterator(assetsPath / "data" / "entities")) {
    EntityType* entity = new EntityType();

    entity->load(entry.path());

    entities.insert({entity->name, entity});
  }
}

EntityType* ResourceManager::getEntityType(std::string name) {
  return entities.at(name);
}