#version 330 core

out vec4 color;
uniform vec3 deltaColor;

void main()
{
    color = vec4(deltaColor ,1.0f);
}