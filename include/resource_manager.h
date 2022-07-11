#pragma once

#include "core.h"
#include "block.h"
#include "shader.h"
#include <map>
#include <fstream>

class BlockData;

class ResourceManager {
  private:
    std::filesystem::path assetsPath;

    std::map<std::string, BlockData> blocks;
    std::map<std::string, TextureCoords> sprites;

    Texture atlas;

    Shader chunkShader;
    Shader hudShader;

    void loadBlocks();
    void loadSprites();
    void loadShaders();
    Shader loadShader(std::string name);
  public:
    ResourceManager(std::string assetsPath);
    BlockData getBlock(std::string name);
    TextureCoords getSprite(std::string name);

    Shader* getChunkShader();
    Shader* getHudShader();
};