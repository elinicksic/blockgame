#include "shader.h"

const char* vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;

out vec4 fColor;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    fColor = aColor;
    gl_Position = uProjection * uView * vec4(aPosition, 1.0);
})";

    const char* fragmentShaderSource = R"(
#version 460 core
out vec4 FragColor;
in vec4 fColor;
void main()
{
    FragColor = fColor;
})";

void Shader::compile() {
    // Compile Vertex Shader
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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