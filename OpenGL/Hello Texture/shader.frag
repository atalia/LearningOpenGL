#version 330 core

in vec3 color_in;
in vec2 Texcoord_in;

out vec4 color;
uniform sampler2D outTexture;

void main()
{
    color = texture(outTexture ,Texcoord_in);
}