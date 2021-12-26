#include "core.h"

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float pitch;
    float yaw;

    float fov = 90.0f;

    void update(uint32_t shaderId);
};