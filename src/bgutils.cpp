#include "bgutils.h"

namespace Utils {
BlockFace raycast(glm::vec3 pos, glm::vec3 direction, World* world, float dist) {
  glm::vec3 cur = glm::floor(pos);
  glm::vec3 xs = pos;
  int face = 0;

  while (true) {
    if (glm::distance(xs, pos) > dist) break;

    if (world->getBlock((int)cur.x, (int)cur.y, (int)cur.z) != "air") {
      // world->setBlock((int) cur.x, (int) cur.y, (int) cur.z, "air");
      return BlockFace{
          (int)cur.x,
          (int)cur.y,
          (int)cur.z,
          face};
    }

    float minM = 1000.0f;
    int minFace;
    glm::vec3 minNormal;

    for (int i = 0; i < 6; i++) {
      glm::vec3 normal;
      int normalSign;

      if (i == 0) {
        normal = glm::vec3(0.0f, 1.0f, 0.0f);
        normalSign = 1;
      } else if (i == 1) {
        normal = glm::vec3(1.0f, 0.0f, 0.0f);
        normalSign = 1;
      } else if (i == 2) {
        normal = glm::vec3(0.0f, 0.0f, 1.0f);
        normalSign = 1;
      } else if (i == 3) {
        normal = glm::vec3(-1.0f, 0.0f, 0.0f);
        normalSign = -1;
      } else if (i == 4) {
        normal = glm::vec3(0.0f, 0.0f, -1.0f);
        normalSign = -1;
      } else if (i == 5) {
        normal = glm::vec3(0.0f, -1.0f, 0.0f);
        normalSign = -1;
      }

      float proj_dv = glm::dot(glm::normalize(direction), normal);

      if (proj_dv <= 0) continue;

      float currPos;

      if (normal.x != 0.0f) {
        currPos = xs.x;
      } else if (normal.y != 0.0f) {
        currPos = xs.y;
      } else if (normal.z != 0.0f) {
        currPos = xs.z;
      }

      float nextPos = normalSign > 0 ? glm::floor(currPos + (float)normalSign) : glm::ceil(currPos + (float)normalSign);

      float deltaPos = glm::abs(nextPos - currPos);

      float m = deltaPos / proj_dv;

      if (i == 0 || m < minM) {
        minM = m;
        minFace = i;
        minNormal = normal;
      }

      face = minFace;
    }

    cur += minNormal;
    xs += glm::normalize(direction) * minM;
  }

  return {
      (int)cur.x,
      (int)cur.y,
      (int)cur.z,
      face,
      world->getUnmappedBlock(cur.x, cur.y, cur.z) == 0};
}
}  // namespace Utils