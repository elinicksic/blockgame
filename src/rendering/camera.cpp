#include "rendering/camera.h"
void Camera::update(uint32_t shaderId) {
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glm::mat4 projection = glm::perspective(glm::radians(fov), (GLfloat)viewport[2] / (GLfloat)viewport[3], 0.1f, 100000.0f);

  glUniformMatrix4fv(
      glGetUniformLocation(shaderId, "uProjection"),
      1,
      GL_FALSE,
      glm::value_ptr(projection));

  float pitchLimit = 89.99f;

  if (pitch > pitchLimit)
    pitch = pitchLimit;
  if (pitch < -pitchLimit)
    pitch = -pitchLimit;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw));
  direction.y = tan(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw));

  front = glm::normalize(direction);

  glm::mat4 view = glm::lookAt(position, position + front, up);
  glUniformMatrix4fv(
      glGetUniformLocation(shaderId, "uView"),
      1,
      GL_FALSE,
      glm::value_ptr(view));
}