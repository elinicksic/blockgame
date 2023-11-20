#include "world/world.h"

World::World(ResourceManager* resourceManager) {
  this->resourceManager = resourceManager;

  const siv::PerlinNoise::seed_type seed = 12345u;
  this->perlin = siv::PerlinNoise{seed};
}

void World::loadChunk(int cx, int cy) {
  Chunk chunk(cx, cy, this, resourceManager);
  chunks.insert({std::make_tuple(cx, cy), chunk});
  chunkGenerationQueue.push_back({cx, cy});
}

void World::generateChunk(int cx, int cy) {
  try {
    chunks.at({cx, cy}).generate(&perlin);
    try {
      if (chunks.at({cx - 1, cy}).getHasUpdatedOnce())
        chunkUpdateQueue.push_back({cx - 1, cy});
    } catch (std::out_of_range) {
    }
    try {
      if (chunks.at({cx, cy - 1}).getHasUpdatedOnce())
        chunkUpdateQueue.push_back({cx, cy - 1});
    } catch (std::out_of_range) {
    }
    try {
      if (chunks.at({cx + 1, cy}).getHasUpdatedOnce())
        chunkUpdateQueue.push_back({cx + 1, cy});
    } catch (std::out_of_range) {
    }
    try {
      if (chunks.at({cx, cy + 1}).getHasUpdatedOnce())
        chunkUpdateQueue.push_back({cx, cy + 1});
    } catch (std::out_of_range) {
    }
    chunkUpdateQueue.push_back({cx, cy});
  } catch (std::out_of_range) {
  }
}

void World::unloadChunk(std::tuple<int, int> chunk) {
  chunks.at(chunk).unload();
  chunks.erase(chunk);
}

void World::autoLoadChunks(int cx, int cy, int renderDistance) {
  // Check for chunks needing to be unloaded.

  std::vector<std::tuple<int, int>> chunksToRemove;
  for (auto& i : chunks) {
    auto [x, y] = i.first;
    if (x < cx - renderDistance || x > cx + renderDistance || y < cy - renderDistance || y > cy + renderDistance) {
      chunksToRemove.push_back({x, y});
    }
  }

  for (auto& i : chunksToRemove) {
    unloadChunk(i);
  }

  std::vector<std::tuple<int, int>> newChunks;

  for (int x = -renderDistance + cx; x <= renderDistance + cx; x++) {
    for (int y = -renderDistance + cy; y <= renderDistance + cy; y++) {
      if (chunks.find({x, y}) == chunks.end()) {
        loadChunk(x, y);
      }
    }
  }

  std::sort(chunkGenerationQueue.begin(), chunkGenerationQueue.end(), [&cx, &cy](auto coords1, auto coords2) {
    auto [x1, y1] = coords1;
    auto [x2, y2] = coords2;
    float distance1 = glm::distance(glm::vec2(cx, cy), glm::vec2(x1, y1));
    float distance2 = glm::distance(glm::vec2(cx, cy), glm::vec2(x2, y2));
    return distance1 > distance2;
  });

  std::sort(chunkUpdateQueue.begin(), chunkUpdateQueue.end(), [&cx, &cy](auto coords1, auto coords2) {
    auto [x1, y1] = coords1;
    auto [x2, y2] = coords2;
    float distance1 = glm::distance(glm::vec2(cx, cy), glm::vec2(x1, y1));
    float distance2 = glm::distance(glm::vec2(cx, cy), glm::vec2(x2, y2));
    return distance1 < distance2;
  });
}

void World::draw() {
  resourceManager->getChunkShader()->bind();

  for (auto& i : chunks) {
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
  // std::cout << chunks.at(std::make_tuple(cx, cy)).getBlock(x % 16, y, z % 16) << std::endl;

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

std::tuple<int, int> World::getChunkCoords(int x, int z) {
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

bool World::isBlockAir(int x, int y, int z) {
  return y > 255 || y < 0 || getUnmappedBlock(x, y, z) <= 0;
}

Chunk* World::getChunk(int cx, int cy) {
  try {
    return &chunks.at({cx, cy});
  } catch (std::out_of_range) {
    return nullptr;
  }
}

void World::update() {
  if (chunkUpdateQueue.size() > 0) {
    try {
      chunks.at(chunkUpdateQueue.front()).update();
    } catch (std::out_of_range) {
    }
    chunkUpdateQueue.pop_front();
  }

  if (chunkGenerationQueue.size() > 0) {
    auto chunkToLoad = chunkGenerationQueue.back();
    generateChunk(std::get<0>(chunkToLoad), std::get<1>(chunkToLoad));
    chunkGenerationQueue.pop_back();
  }

  for (auto& entity : entities) {
    entity->update();
  }
}

Entity* World::spawnEntity(std::string name, float x, float y, float z) {
  EntityType* type = resourceManager->getEntityType(name);
  std::cout << type->name << std::endl;
  Entity* entity = new Entity(type, this, x, y, z);

  entities.push_back(entity);

  return entity;
}

void World::spawnEntity(Entity* entity) {
  entities.push_back(entity);
}