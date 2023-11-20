#include "entity.h"

Entity::Entity(EntityType* entityType, World* world, float x, float y, float z) {
  type_ = entityType;
  this->world = world;
  pos_ = glm::vec3(x, y, z);
  velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
  force_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Entity::update() {
  const float gravity = 0.08;
  const float friction = 0.546;
  force_ += glm::vec3(0.0f, -gravity, 0.0f);

  // force_ += -velocity_ / glm::vec3(2.0f, 1.0f, 2.0);

  velocity_ += force_;

  const float drag = 0.04;

  velocity_.y = (velocity_.y * (1-drag))-(gravity)*(1-drag);
  velocity_.x = velocity_.x * friction;
  velocity_.z = velocity_.z * friction;

  glm::vec3 prevPos = pos_;

  pos_ += velocity_;

  applyCollisions(prevPos);

  rendered_pos_ = pos_;

  force_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Entity::render(int delta) {
  // rendered_pos += velocity_ * ((float) delta / 50.0f);
}

void Entity::teleport(glm::vec3 location) {
  pos_ = location;
}

void Entity::addForce(glm::vec3 force) {
  force_ += force;
}

void Entity::handleCollision(int x, int y, int z) {
  if (!world->isChunkLoaded(world->getChunkCoords(x, z)) || world->getUnmappedBlock(x, y, z) == 0)
    return;

  glm::vec3 minA(pos_.x - type_->width / 2.0f, pos.y, pos.z - type_->length / 2.0f);
  glm::vec3 maxA(pos_.x + type_->width / 2.0f, pos.y + type_->height, pos.z + type_->length / 2.0f);

  glm::vec3 minB(x, y, z);
  glm::vec3 maxB(x + 1, y + 1, z + 1);

  float overlapX = 0.0;
  bool collidesX = false;

  float overlapY = 0.0;
  bool collidesY = false;

  float overlapZ = 0.0;
  bool collidesZ = false;

  if (minA.x < maxB.x && maxA.x > minB.x)
  {
    overlapX = glm::min(maxA.x, maxB.x) - glm::max(minA.x, minB.x);
    collidesX = true;
    std::cout << "Collides x!\n";
  }

  if (minA.y < maxB.y && maxA.y > minB.y)
  {
    overlapY = glm::min(maxA.y, maxB.y) - glm::max(minA.y, minB.y);
    collidesY = true;
    std::cout << "Collides y!\n";
  }

  if (minA.z < maxB.z && maxA.z > minB.z)
  {
    overlapZ = glm::min(maxA.z, maxB.z) - glm::max(minA.z, minB.z);
    collidesZ = true;
    std::cout << "Collides z!\n";
  }

  if (!collidesX || !collidesY || !collidesZ)
    return;
  // Collides with block

  // Adjusting collision resolution for x and z axes (side collisions)
  if (overlapX < overlapY && overlapX < overlapZ)
  {
    // Resolve in x direction
    if (minA.x < minB.x)
    {
      // Resolve in positive direction
      pos_.x -= overlapX;
      minA.x = maxA.x - (maxA.x - minA.x);
      minA = glm::vec3(pos_.x - type_->width / 2.0f, pos.y, pos.z - type_->length / 2.0f);
      maxA = glm::vec3(pos_.x + type_->width / 2.0f, pos.y + type_->height, pos.z + type_->length / 2.0f);
    }
    else
    {
      pos_.x += overlapX;
      maxA.x = minA.x + (maxA.x - minA.x);
      minA = glm::vec3(pos_.x - type_->width / 2.0f, pos.y, pos.z - type_->length / 2.0f);
      maxA = glm::vec3(pos_.x + type_->width / 2.0f, pos.y + type_->height, pos.z + type_->length / 2.0f);
    }
    velocity_.x = 0;
  }
  else if (overlapZ < overlapY)
  {
    // Resolve in z direction
    if (minA.z < minB.b)
    {
      // Resolve in positive direction
      pos_.z -= overlapZ;
      minA.z = maxA.z - (maxA.z - minA.z);
      minA = glm::vec3(pos_.x - type_->width / 2.0f, pos.y, pos.z - type_->length / 2.0f);
      maxA = glm::vec3(pos_.x + type_->width / 2.0f, pos.y + type_->height, pos.z + type_->length / 2.0f);
    }
    else
    {
      pos_.z += overlapZ;
      maxA.z = minA.z + (maxA.z - minA.z);
      minA = glm::vec3(pos_.x - type_->width / 2.0f, pos.y, pos.z - type_->length / 2.0f);
      maxA = glm::vec3(pos_.x + type_->width / 2.0f, pos.y + type_->height, pos.z + type_->length / 2.0f);
    }
    velocity_.z = 0;
  }
  else
  {
    // Resolve in y direction
    if (minA.y < minB.y)
    {
      // Resolve in positive direction
      pos_.y -= overlapY;
      minA.y = maxA.y - (maxA.y - minA.y);
      minA = glm::vec3(pos_.x - type_->width / 2.0f, pos.y, pos.z - type_->length / 2.0f);
      maxA = glm::vec3(pos_.x + type_->width / 2.0f, pos.y + type_->height, pos.z + type_->length / 2.0f);
    }
    else
    {
      pos_.y += overlapY;
      maxA.y = minA.y + (maxA.y - minA.y);
      minA = glm::vec3(pos_.x - type_->width / 2.0f, pos.y, pos.z - type_->length / 2.0f);
      maxA = glm::vec3(pos_.x + type_->width / 2.0f, pos.y + type_->height, pos.z + type_->length / 2.0f);
      isOnGround_ = true;
    }
    velocity_.y = 0;
  }
}

AABB Entity::getAABB() {
  return AABB(pos_ + glm::vec3(0.0f, type_->height / 2.0f, 0.0f), glm::vec3(type_->width, type_->height, type_->length));
}

void Entity::applyCollisions(glm::vec3 prevPos) {
  isOnGround_ = false;

  if (pos.y < 0 || pos.y > 255) return; 

  for (int i = 0; i < 3; i++) {
    if (glm::length(velocity_) <= 0.00001) {
      std::cout << "Skipping collision checks because velocity it too low\n";
      break;
    }

    glm::vec3 currentPos = prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f);

    while (glm::distance(prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f), currentPos) <= glm::length(velocity_))
    {
      AABB box = AABB(currentPos, glm::vec3(type_->width, type_->height, type_->length));

      std::vector<std::tuple<int*, glm::vec3, glm::vec3>> collisions;

      for (int x = std::floor(box.min.x - 1); x <= std::ceil(box.max.x + 1); x++)
      {
        for (int y = std::floor(box.min.y - 1); y <= std::ceil(box.max.y + 1); y++)
        {
          for (int z = std::floor(box.min.z - 1); z <= std::ceil(box.max.z + 1); z++)
          {
            if (!world->isBlockAir(x, y, z))
            {

              // Perform ray-AABB collision check between the voxel and the AABB box
              glm::vec3 voxelCenter(x + 0.5f, y + 0.5f, z + 0.5f);
              glm::vec3 voxelSize(1.0f + type_->width, 1.0f + type_->height, 1.0f + type_->length);

              AABB voxelAABB(voxelCenter, voxelSize);

              glm::vec3 collisionPoint, collisionNormal;

              if (Collisions::rayToAABB(prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f), glm::normalize(velocity_), glm::length(velocity_), voxelAABB, &collisionPoint, &collisionNormal))
              {
                int blockPos[] = {x, y, z};
                collisions.push_back(std::make_tuple(blockPos, collisionPoint, collisionNormal));
              }
            }
          }
        }
      }

      std::tuple<int *, glm::vec3, glm::vec3> nearest;
      float nearestDistance = std::numeric_limits<float>::infinity();

      for (std::tuple<int *, glm::vec3, glm::vec3> collision : collisions)
      {
        glm::vec3 startPose = prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f);
        glm::vec3 collisionPoint = std::get<1>(collision);

        float distance = glm::distance(startPose, collisionPoint);
        if (distance < nearestDistance) {
          nearestDistance = distance;
          nearest = collision;
        }
      }

      if (nearestDistance != INFINITY) {
        int* blockPos = std::get<0>(nearest);
        glm::vec3 collisionPoint = std::get<1>(nearest);
        glm::vec3 collisionNormal = std::get<2>(nearest);

        float dot = glm::dot(velocity_, -collisionNormal);

        if (dot == 0.0f) break;

        // pos_ = (collisionPoint - glm::vec3(0.0f, type_->height / 2, 0.0f));

        if (collisionNormal.x != 0) {
          pos_.x = collisionPoint.x + collisionNormal.x * 0.0001f;
          velocity_.x = 0;
          std::cout << "Collide X with " << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << " normal " << collisionNormal.x << std::endl;
        } else if (collisionNormal.y != 0) {
          pos_.y = collisionPoint.y - type_->height / 2.0f + collisionNormal.y * 0.0001;;
          velocity_.y = 0;
          std::cout << "Collide Y with " << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << " normal " << collisionNormal.y << std::endl;
        } else {
          pos_.z = collisionPoint.z + collisionNormal.z * 0.0001f;
          velocity_.z = 0;
          std::cout << "Collide Z with " << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << " normal " << collisionNormal.z << std::endl;
        }

        if (collisionNormal.y > 0) {
          isOnGround_ = true;
        }
        break;
      }

      currentPos = currentPos + glm::normalize(velocity_) * 1.0f;
    }
  }

  std::cout << pos_.x << " " << pos_.y << " " << pos_.z << std::endl;
}