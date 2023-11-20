#include "rendering/arraytexture.h"

ArrayTexture::ArrayTexture(std::vector<std::string> paths) {
  glGenTextures(1, &id);
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D_ARRAY, id);

  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, 16, 16, paths.size());

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  stbi_set_flip_vertically_on_load(1);

  for (int i = 0; i < paths.size(); i++) {
    std::string path = paths[i];

    int width, height, nrChannels;

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id);

    if (data) {
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
      std::cout << "Failed to load texture: " << path << std::endl;
    }

    stbi_image_free(data);
  }

  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void ArrayTexture::bind(GLuint shaderId) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, id);
  glUniform1i(glGetUniformLocation(shaderId, "texture0"), 0);
}