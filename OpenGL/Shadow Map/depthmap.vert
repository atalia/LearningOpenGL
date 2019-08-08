#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

out VS_OUT
{
	vec3 normal;
	vec2 texCoord;
}vs_out;


uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 model;

void main()
{
	gl_Position = lightProjection * lightView* model* vec4(position, 1.0f);
	vs_out.normal = normal;
	vs_out.texCoord = texCoord;
}