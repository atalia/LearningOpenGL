#version 330 core

in vec3 textureDir;

out vec4 color;

uniform sampler3D cubeTexture;

void main()
{
    color = texture(cubeTexture, textureDir);
}
