#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 tangent;
layout(location=3) in vec3 bitangent;
layout(location=4) in vec3 normal;

out VS_OUT{
	vec2 texCoord;
	vec3 TBNFragPos;
	vec3 TBNViewPos;
	vec3 TBNLightPos;
}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.texCoord = texCoord;
	vec3 T = vec3(model * vec4(tangent, 0.0f));
	vec3 B = vec3(model * vec4(bitangent, 0.0f));
	vec3 N = vec3(model * vec4(normal, 0.0f));
	mat3 WORLD2TBN = transpose(mat3(T, B, N));

	vs_out.TBNFragPos = WORLD2TBN *  vec3(model * vec4(position, 1.0f));
	vs_out.TBNLightPos = WORLD2TBN * lightPos;
	vs_out.TBNViewPos = WORLD2TBN * viewPos;
}
