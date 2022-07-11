#include "resource_manager.h"

BlockData ResourceManager::getBlock(std::string name) {
  return blocks.at(name);
}

TextureCoords ResourceManager::getSprite(std::string name) {
  return sprites.at(name);
}

ResourceManager::ResourceManager(std::string assetsPath) {
  this -> assetsPath = assetsPath;

  loadShaders();
  loadSprites();
  loadBlocks();
}

void ResourceManager::loadBlocks() {
  for (const auto & entry : std::filesystem::directory_iterator(assetsPath / "data" / "blocks")) {
    std::cout << entry.path() << std::endl;
    BlockData block(entry.path(), this);
    blocks.insert({block.name, block});
  }
}

void ResourceManager::loadSprites() {
  YAML::Node spriteYaml = YAML::LoadFile(assetsPath / "texture" / "sprites.yaml");

  std::string atlasPath = spriteYaml["path"].as<std::string>();

  atlas.load(assetsPath / "texture" / atlasPath);

  YAML::Node texturesNode = spriteYaml["textures"];

  for(unsigned int i = 0; i < texturesNode.size(); i++) {
    YAML::Node node = texturesNode[i];
    std::string name = node["name"].as<std::string>();

    TextureCoords coords = {
      (float) node["x"].as<int>() / atlas.getWidth(),
      (float) (atlas.getHeight() - node["y"].as<int>()) / atlas.getHeight(),
      (float) node["width"].as<int>() / atlas.getWidth(),
      (float) node["height"].as<int>() / atlas.getHeight()
    };

    sprites.insert({name, coords});
  }
}

void ResourceManager::loadShaders() {
  chunkShader = loadShader("chunk");
  //hudShader = loadShader("hud");
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