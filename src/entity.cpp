#include "entity.h"

Entity::Entity(EntityType *entityType, World *world, float x, float y, float z) {
  type_ = entityType;
  this->world = world;
  pos_ = glm::vec3(x, y, z);
  velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
  force_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Entity::update() {
  const float gravity = 0.08;
  const float friction = 0.546;
  const float drag = 0.04;

  force_ += glm::vec3(0.0f, -gravity, 0.0f);

  velocity_ += force_;

  velocity_.y = (velocity_.y * (1 - drag)) - (gravity) * (1 - drag);
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

void Entity::teleport(glm::vec3 location) { pos_ = location; }

void Entity::addForce(glm::vec3 force) { force_ += force; }

void Entity::handleCollision(glm::vec3 collisionPoint, glm::vec3 collisionNormal) {
  if (collisionNormal.x != 0) {
    pos_.x = collisionPoint.x + collisionNormal.x * 0.0001f;
    velocity_.x = 0;
  } else if (collisionNormal.y != 0) {
    pos_.y = collisionPoint.y - type_->height / 2.0f + collisionNormal.y * 0.0001;
    velocity_.y = 0;
  } else {
    pos_.z = collisionPoint.z + collisionNormal.z * 0.0001f;
    velocity_.z = 0;
  }

  if (collisionNormal.y > 0) {
    isOnGround_ = true;
  }
}

AABB Entity::getAABB() {
  return AABB(pos_ + glm::vec3(0.0f, type_->height / 2.0f, 0.0f), glm::vec3(type_->width, type_->height, type_->length));
}

void Entity::applyCollisions(glm::vec3 prevPos) {
  isOnGround_ = false;

  if (pos.y < 0 || pos.y > 255) return;

  for (int i = 0; i < 3; i++) {
    if (glm::length(velocity_) <= 0.00001) break;

    glm::vec3 currentPos = prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f);

    while (glm::distance(prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f), currentPos) <= glm::length(velocity_)) {
      AABB box = AABB(currentPos, glm::vec3(type_->width, type_->height, type_->length));

      std::vector<std::tuple<int *, glm::vec3, glm::vec3>> collisions;

      for (int x = std::floor(box.min.x - 1); x <= std::ceil(box.max.x + 1); x++) {
        for (int y = std::floor(box.min.y - 1); y <= std::ceil(box.max.y + 1); y++) {
          for (int z = std::floor(box.min.z - 1); z <= std::ceil(box.max.z + 1); z++) {
            if (!world->isBlockAir(x, y, z)) {
              glm::vec3 blockCenter(x + 0.5f, y + 0.5f, z + 0.5f);
              glm::vec3 blockSize(1.0f + type_->width, 1.0f + type_->height, 1.0f + type_->length);

              AABB blockAABB(blockCenter, blockSize);

              glm::vec3 collisionPoint, collisionNormal;

              glm::vec3 startPosition = prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f);

              if (Collisions::rayToAABB(startPosition, velocity_, blockAABB, &collisionPoint, &collisionNormal)) {
                int blockPos[] = {x, y, z};
                collisions.push_back(
                    std::make_tuple(blockPos, collisionPoint, collisionNormal));
              }
            }
          }
        }
      }

      std::tuple<int *, glm::vec3, glm::vec3> nearest;
      float nearestDistance = std::numeric_limits<float>::infinity();

      // Find nearest collision
      for (std::tuple<int *, glm::vec3, glm::vec3> collision : collisions) {
        glm::vec3 startPose =
            prevPos + glm::vec3(0.0f, type_->height / 2.0f, 0.0f);
        glm::vec3 collisionPoint = std::get<1>(collision);

        float distance = glm::distance(startPose, collisionPoint);
        if (distance < nearestDistance) {
          nearestDistance = distance;
          nearest = collision;
        }
      }

      if (nearestDistance != INFINITY) {
        int *blockPos = std::get<0>(nearest);
        glm::vec3 collisionPoint = std::get<1>(nearest);
        glm::vec3 collisionNormal = std::get<2>(nearest);

        float dot = glm::dot(velocity_, -collisionNormal);

        if (dot != 0.0f) {
          handleCollision(collisionPoint, collisionNormal);
        }

        break;
      }
      currentPos = currentPos + glm::normalize(velocity_) * 1.0f;
    }
  }
}