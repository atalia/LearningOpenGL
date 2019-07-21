#version 330 core

in vec3 textureDir;

out vec4 color;

uniform samplerCube cubeTexture;

void main()
{
    color = texture(cubeTexture, textureDir);
}
