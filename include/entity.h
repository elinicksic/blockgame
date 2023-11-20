#pragma once

#include "core.h"
#include "entity_type.h"
#include "world/world.h"
#include "bgutils.h"
#include "physics/aabb.h"
#include "physics/collisions.h"

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
    void handleCollision(int x, int y, int z);
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