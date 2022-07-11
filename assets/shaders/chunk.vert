#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTextureCoords;
layout (location = 2) in float aLighting;

out vec2 fTextureCoords;
out float fLighting;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    fTextureCoords = aTextureCoords;
    fLighting = aLighting;
    gl_Position = uProjection * uView * vec4(aPosition, 1.0);
}