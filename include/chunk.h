#pragma once

#include "core.h"
#include <map>
#include "block.h"
#include "texture.h"
#include "resource_manager.h"
#include "world.h"

class World;

class Chunk {
  private:
    std::map<int, std::string> blockMap;
    short blockData[16][16][256];
    int cx;
    int cy;
    GLuint bufferId;
    int vboSize;
    ResourceManager *resourceManager;
    World *world;
  public:
    Chunk(int cx, int cy, World *world, ResourceManager *resourceManager);
    std::string getBlock(int x, int y, int z);
    std::vector<Vertex> genVertexArray();
    void fill(std::string block);
    void draw();
    void update();
    void setBlock(int x, int y, int z, std::string id);
    void unload();
    int getUnmappedBlock(int x, int y, int z);
};