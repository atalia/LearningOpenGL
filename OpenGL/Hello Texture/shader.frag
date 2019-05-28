#version 330 core

in vec3 color;
in vec2 Texcoord;

out vec4 color;
uniform sampler2D outTexture;

void main()
{
    color = texture(outTexture ,Texcoord);
}