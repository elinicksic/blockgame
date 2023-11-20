#pragma once
#include "core.h"
#include "physics/aabb.h"

namespace Collisions {
  bool boxToAABB(AABB box1, AABB box2);
  bool rayToAABB(glm::vec3 pos, glm::vec3 dir, float magnitude, AABB box, glm::vec3* point, glm::vec3* normal);
}