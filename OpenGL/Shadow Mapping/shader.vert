#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoords;

out VS_OUT{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec4 fragPosInLightSpace;
}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.fragPos = vec3(model * vec4(position,1.0f));
	vs_out.normal = transpose(inverse(mat3(model))) * normal;
	vs_out.texCoords = texCoords;
	vs_out.fragPosInLightSpace = lightSpaceMatrix * model * vec4(position, 1.0f);
}