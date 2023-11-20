#include "physics/collisions.h"

namespace Collisions {
  bool boxToAABB(AABB box1, AABB box2) {
    if (box1.max.x < box2.min.x || box2.max.x < box1.min.x) return false;
    if (box1.max.y < box2.min.y || box2.max.y < box1.min.y) return false;
    if (box1.max.z < box2.min.z || box2.max.z < box1.min.z) return false;

    return true;
  }

  bool rayToAABB(glm::vec3 pos, glm::vec3 dir, float magnitude, AABB box, glm::vec3 *point, glm::vec3 *normal)
  {
    glm::vec3 invD = 1.0f / dir;
    glm::vec3 t0s = (box.min - pos) * invD;
    glm::vec3 t1s = (box.max - pos) * invD;

    glm::vec3 tsmaller = glm::min(t0s, t1s);
    glm::vec3 tbigger = glm::max(t0s, t1s);
    
    float tmin = glm::max(0.0f, glm::max(tsmaller[0], glm::max(tsmaller[1], tsmaller[2])));
    float tmax = glm::min(magnitude, glm::min(tbigger[0], glm::min(tbigger[1], tbigger[2])));

    if (tmin >= tmax) return false;

    // Set collision point and normal vectors

    *point = pos + dir * tmin;

    glm::vec3 normals[] = {
        glm::vec3(-1, 0, 0), // Left face
        glm::vec3(1, 0, 0),  // Right face
        glm::vec3(0, -1, 0), // Bottom face
        glm::vec3(0, 1, 0),  // Top face
        glm::vec3(0, 0, -1), // Back face
        glm::vec3(0, 0, 1)   // Front face
    };

    // Determine which face was intersected
    glm::vec3 intersectedFaceNormal;

    if (tsmaller[0] > tsmaller[1] && tsmaller[0] > tsmaller[2])
    {
      intersectedFaceNormal = normals[dir.x < 0 ? 1 : 0]; // Left or Right face
    }
    else if (tsmaller[1] > tsmaller[2])
    {
      intersectedFaceNormal = normals[dir.y < 0 ? 3 : 2]; // Bottom or Top face
    }
    else
    {
      intersectedFaceNormal = normals[dir.z < 0 ? 5 : 4]; // Back or Front face
    }

    *normal = intersectedFaceNormal;

    return true;
  }
}