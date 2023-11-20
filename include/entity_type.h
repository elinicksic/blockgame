#pragma once

#include "core.h"
#include <yaml-cpp/yaml.h>

class EntityType {
  private:
    std::string name_;
    float length_;
    float width_;
    float height_;
  public:
    void load(std::string path);
    const float& length = length_;
    const float& width = width_;
    const float& height = height_;
    const std::string& name = name_;
};