#version 460 core
out vec4 FragColor;

in vec3 fTextureCoords;
in float fLighting;
uniform sampler2DArray texture0;

void main() {
    FragColor = mix(texture(texture0, fTextureCoords), vec4(0.0f, 0.0f, 0.0f, 1.0f), fLighting);
}