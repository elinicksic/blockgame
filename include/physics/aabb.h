#pragma once
#include "core.h"

struct AABB {
  glm::vec3 min;
  glm::vec3 max;
  AABB(glm::vec3 position, glm::vec3 size);
};
