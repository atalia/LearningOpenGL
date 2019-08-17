#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 tangent;
layout(location=3) in vec3 bitangent;
layout(location=4) in vec3 normal;

out VS_OUT{
	vec2 texCoord;
	vec3 fragPos;
	mat3 TBN;
}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.texCoord = texCoord;
	vs_out.fragPos = vec3(model * vec4(position, 1.0f));
	vec3 T = vec3(model * vec4(tangent, 0.0f));
	vec3 B = vec3(model * vec4(bitangent, 0.0f));
	vec3 N = vec3(model * vec4(normal, 0.0f));
	//vec3 T = tangent;
	//vec3 B = bitangent;
	//vec3 N = normal;
	vs_out.TBN = mat3(T, B, N);// 大坑: TBN->WORLD
}