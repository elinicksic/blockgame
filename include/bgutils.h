#pragma once

#include <tuple>

#include "core.h"
#include "world/world.h"

class World;

namespace Utils {
struct BlockFace {
  int x;
  int y;
  int z;
  glm::vec3 normal;
  bool isAir;
};

BlockFace raycast(glm::vec3 pos, glm::vec3 direction, World* world, float dist);
}  // namespace Utils