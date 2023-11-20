#include "rendering/shader.h"

void Shader::compile(std::string fragSource, std::string vertSource) {
  // Compile Vertex Shader
  uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLchar const* glVertSource = vertSource.c_str();
  glShaderSource(vertexShader, 1, &glVertSource, NULL);
  glCompileShader(vertexShader);

  // Check for success compiling Vertex Shader
  int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("Failed to compile Vertex Shader: %s\n", infoLog);
    }

  // Compile Fragment Shader
  uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  GLchar const* glFragSource = fragSource.c_str();
  glShaderSource(fragmentShader, 1, &glFragSource, NULL);
  glCompileShader(fragmentShader);

  // Check for success compiling Fragment Shader
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("Failed to compile Fragment Shader: %s\n", infoLog);
  }

  // Link Program
  programId = glCreateProgram();
  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragmentShader);
  glLinkProgram(programId);

  // Check for errors linking
  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programId, 512, NULL, infoLog);
    printf("Failed linking shader program: ", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::bind() {
  glUseProgram(programId);
}