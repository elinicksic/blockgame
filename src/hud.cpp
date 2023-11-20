#include "hud.h"

HUD::HUD(ResourceManager* resourceManager) {
  this->resourceManager = resourceManager;

  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, pos));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, color));
  glEnableVertexAttribArray(1);

  HUDVertex verticies[3] = {
      {glm::vec2(-1.0f, -1.0f),
       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
      {glm::vec2(1.0f, -1.0f),
       glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
      {glm::vec2(0.0f, 1.0f),
       glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)}};

  bufferSize = sizeof(verticies);

  glBufferData(GL_ARRAY_BUFFER, bufferSize, verticies, GL_STATIC_DRAW);

  glm::mat4 ortho = glm::ortho(-(800.0f / 2.0f), 800.0f / 2.0f, 600.0f / 2.0f, -(600.0f / 2.0f), -1000.0f, 1000.0f);

  glUniformMatrix4fv(
      glGetUniformLocation(resourceManager->getHudShader()->programId, "uProjection"),
      1,
      GL_FALSE,
      glm::value_ptr(ortho));
}

void HUD::render() {
  resourceManager->getHudShader()->bind();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBindVertexArray(vao);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, pos));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, color));
  glEnableVertexAttribArray(1);

  glDisable(GL_DEPTH_TEST);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}