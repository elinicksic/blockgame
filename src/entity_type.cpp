#include "entity_type.h"

void EntityType::load(std::string path) {
  YAML::Node yamlNode = YAML::LoadFile(path);
  length_ = yamlNode["bounding_box"]["length"].as<float>();
  width_ = yamlNode["bounding_box"]["width"].as<float>();
  height_ = yamlNode["bounding_box"]["height"].as<float>();
  name_ = yamlNode["name"].as<std::string>();
}