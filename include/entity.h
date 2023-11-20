#pragma once

#include "bgutils.h"
#include "core.h"
#include "entity_type.h"
#include "physics/aabb.h"
#include "physics/collisions.h"
#include "world/world.h"

class World;

class Entity {
 private:
  EntityType* type_;
  World* world;
  glm::vec3 pos_;
  glm::vec3 velocity_;
  glm::vec3 force_;
  glm::vec3 rendered_pos_;
  bool isOnGround_ = false;
  void applyCollisions(glm::vec3 prevPos);
  void handleCollision(glm::vec3 collisionPoint, glm::vec3 collisionNormal);

 public:
  Entity(EntityType* type, World* world, float x, float y, float z);
  void teleport(glm::vec3 location);
  void addForce(glm::vec3 velocity);
  void update();
  void render(int delta);
  AABB getAABB();

  const glm::vec3& pos = pos_;
  const glm::vec3& velocity = velocity_;
  const glm::vec3& force = force_;
  const glm::vec3& rendered_pos = rendered_pos;
  const bool& isOnGround = isOnGround_;
};