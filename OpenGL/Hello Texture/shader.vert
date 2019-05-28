#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

out vec3 color;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
	color = color;
	texCoord = texCoord;
}