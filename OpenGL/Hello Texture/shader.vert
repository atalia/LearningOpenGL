#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 color_in;
layout(location = 3) in vec2 texCoord_in;

out vec3 color;
out vec2 tecCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
	vec3 color = color_in;
	vec2 texcoord = texCoord_in;
}