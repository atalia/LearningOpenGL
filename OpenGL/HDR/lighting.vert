#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

out VS_OUT{
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.normal = normalize(transpose(inverse(mat3(model)))*(-normal));
	vs_out.texCoord = texCoord;
	vs_out.fragPos = vec3(model * vec4(position, 1.0f));
}