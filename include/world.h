#pragma once
#include "core.h"
#include <map>
#include "chunk.h"
#include "resource_manager.h"

class Chunk;

class World {
  private:
    std::map<std::tuple<int, int>, Chunk> chunks;
    ResourceManager *resourceManager;
  public:
    World(ResourceManager *resourceManager);
    void loadChunk(int cx, int cy);
    void unloadChunk(int cx, int cy);
    void autoLoadChunks(int cx, int cy, int renderDistance);
    void draw();
    std::string getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, std::string block);
    bool isChunkLoaded(int cx, int cy);
    bool isChunkLoaded(std::tuple<int, int>);
    std::tuple<int, int> getChunkCoords(int x, int z);
    int getUnmappedBlock(int x, int y, int z);
    Chunk* getChunk(int cx, int cy);
};