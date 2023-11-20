#pragma once

#include <fstream>
#include <map>

#include "block.h"
#include "core.h"
#include "entity_type.h"
#include "rendering/arraytexture.h"
#include "rendering/shader.h"

class BlockData;

class ResourceManager {
 private:
  std::filesystem::path assetsPath;

  std::map<std::string, BlockData> blocks;
  std::map<std::string, int> textures;
  std::map<std::string, EntityType*> entities;

  // Texture atlas;

  ArrayTexture* arrayTexture;

  Shader chunkShader;
  Shader hudShader;

  void loadBlocks();
  void loadSprites();
  void loadShaders();
  void loadTextures();
  void loadEntities();

  Shader loadShader(std::string name);

 public:
  ResourceManager(std::string assetsPath);
  BlockData getBlock(std::string name);
  int getTextureDepth(std::string name);
  void bindTexture();
  EntityType* getEntityType(std::string name);

  Shader* getChunkShader();
  Shader* getHudShader();
};