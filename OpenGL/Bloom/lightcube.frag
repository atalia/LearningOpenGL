#version 330 core

out vec4 color;

uniform vec3 cubecolor;

void main()
{
	color = vec4(cubecolor, 1.0f);
}