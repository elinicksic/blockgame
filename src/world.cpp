#include "world.h"

World::World(ResourceManager *resourceManager) {
  this->resourceManager = resourceManager;
}

void World::loadChunk(int cx, int cy) {
  Chunk chunk(cx, cy, this, resourceManager);
  chunk.fill("nuke");
  chunks.insert({std::make_tuple(cx, cy), chunk});
}

void World::unloadChunk(int cx, int cy) {
  chunks.at({cx, cy}).unload();
  chunks.erase({cx, cy});
}

void World::autoLoadChunks(int cx, int cy, int renderDistance) {
  // Check for chunks needing to be unloaded.
  for (auto &i : chunks) {
    auto [x, y] = i.first;
    if (x < cx - renderDistance || x > cx + renderDistance || y < cy - renderDistance || y > cy + renderDistance) {
      unloadChunk(x, y);
    }
  }

  std::vector<std::tuple<int, int>> newChunks;

  for (int x = -renderDistance; x < renderDistance; x++) {
    for (int y = -renderDistance; y < renderDistance; y++) {
      if (chunks.find({x, y}) == chunks.end()) {
        newChunks.push_back({x, y});
        loadChunk(x, y);
      }
    }
  }

  for (auto &i : newChunks) {
    chunks.at(i).update();
  }
}

void World::draw() {
  for (auto &i : chunks) {
    i.second.draw();
  }
}

std::string World::getBlock(int x, int y, int z) {
  int cx = x / 16;
	int cy = z / 16;

  int localX = glm::abs(x % 16);
  int localZ = glm::abs(z % 16);

  if (x < 0) {
    cx--;
    localX = 15 - localX + 1;
    if (localX == 16) {
      localX = 0;
      cx++;
    }
  }
  if (z < 0) {
    cy--;
    localZ = 15 - localZ + 1;
    if (localZ == 16) {
      localZ = 0;
      cy++;
    }
  }
  
  if (chunks.find({cx, cy}) == chunks.end()) 
    return "";
  return chunks.at({cx, cy}).getBlock(localX, y, localZ);
}

void World::setBlock(int x, int y, int z, std::string block) {
  int cx = x / 16;
	int cy = z / 16;

  int localX = glm::abs(x % 16);
  int localZ = glm::abs(z % 16);

  if (x < 0) {
    cx--;
    localX = 15 - localX + 1;
    if (localX == 16) {
      localX = 0;
      cx++;
    }
  }
  if (z < 0) {
    cy--;
    localZ = 15 - localZ + 1;
    if (localZ == 16) {
      localZ = 0;
      cy++;
    }
  }

  chunks.at(std::make_tuple(cx, cy)).setBlock(localX, y, localZ, block);
  //std::cout << chunks.at(std::make_tuple(cx, cy)).getBlock(x % 16, y, z % 16) << std::endl;

  chunks.at({cx, cy}).update();
  if (localX == 0)
    chunks.at({cx - 1, cy}).update();
  if (localX == 15)
    chunks.at({cx + 1, cy}).update();
  if (localZ == 0)
    chunks.at({cx, cy - 1}).update();
  if (localZ == 15)
    chunks.at({cx, cy + 1}).update();
}

bool World::isChunkLoaded(int cx, int cy) {
  return isChunkLoaded({cx, cy});
}

bool World::isChunkLoaded(std::tuple<int, int> chunkCoords) {
  return chunks.find(chunkCoords) != chunks.end();
}

std::tuple<int, int> getChunkCoords(int x, int z) {
  int cx = x / 16;
	int cy = z / 16;

  int localX = glm::abs(x % 16);
  int localZ = glm::abs(z % 16);

  if (x < 0) {
    cx--;
    localX = 15 - localX + 1;
    if (localX == 16) {
      localX = 0;
      cx++;
    }
  }
  if (z < 0) {
    cy--;
    localZ = 15 - localZ + 1;
    if (localZ == 16) {
      localZ = 0;
      cy++;
    }
  }

  return {cx, cy};
}

int World::getUnmappedBlock(int x, int y, int z) {
  int cx = x / 16;
	int cy = z / 16;

  int localX = glm::abs(x % 16);
  int localZ = glm::abs(z % 16);

  if (x < 0) {
    cx--;
    localX = 15 - localX + 1;
    if (localX == 16) {
      localX = 0;
      cx++;
    }
  }
  if (z < 0) {
    cy--;
    localZ = 15 - localZ + 1;
    if (localZ == 16) {
      localZ = 0;
      cy++;
    }
  }

  try {
    return chunks.at({cx, cy}).getUnmappedBlock(localX, y, localZ);
  } catch (std::out_of_range) {
    return -1;
  }
}

Chunk* World::getChunk(int cx, int cy) {
  try {
    return &chunks.at({cx, cy});
  } catch (std::out_of_range) {
    return nullptr;
  }
}