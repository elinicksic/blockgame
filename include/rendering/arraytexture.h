#include "core.h"
#include "stb_image.h"

class ArrayTexture {
 private:
  GLuint id;

 public:
  ArrayTexture(std::vector<std::string> paths);
  void bind(GLuint shaderId);
};