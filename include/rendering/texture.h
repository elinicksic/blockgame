#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <map>

#include "core.h"
#include "stb_image.h"

struct TextureCoords {
  float x;
  float y;
  float width;
  float height;
};

class Texture {
 private:
  GLuint textureId;
  int width, height;

 public:
  Texture(std::string path);
  Texture();
  void load(std::string path);
  void bind(GLuint shaderId);
  int getWidth();
  int getHeight();
};