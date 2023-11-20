#include "rendering/texture.h"

Texture::Texture(std::string path) {
  load(path);
}

Texture::Texture() {
  
}

void Texture::load(std::string path) {
  stbi_set_flip_vertically_on_load(1);

  int width, height, nrChannels;

  unsigned char *data = stbi_load(&*path.begin(), &width, &height, &nrChannels, 3); 

  this -> width = width;
  this -> height = height;

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  } else {
    std::cout << "Failed to load atlas texture: " << path << std::endl;
  }
}

void Texture::bind(GLuint shaderId) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glUniform1i(glGetUniformLocation(shaderId, "texture0"), 0);
}

int Texture::getWidth() {
  return width;
}

int Texture::getHeight() {
  return height;
}