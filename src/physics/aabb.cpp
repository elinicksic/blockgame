#include "physics/aabb.h"

AABB::AABB(glm::vec3 position, glm::vec3 size) {
  min = position - size / 2.0f;
  max = position + size / 2.0f;
}
