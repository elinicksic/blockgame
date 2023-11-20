#pragma once

#include "core.h"

struct Shader {
  uint32_t programId;

  void bind();
  void compile(std::string fragSource, std::string vertSource);
};