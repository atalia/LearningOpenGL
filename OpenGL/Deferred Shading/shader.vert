#version 330 core

layout(location=0) in vec3 postion;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

layout(location=3) in vec3 tbn_tangent;
layout(location=4) in vec3 tbn_bitangent;
layout(location=5) in vec3 tbn_normal;

layout(location=6) in vec4 instanceMatrix_1;
layout(location=7) in vec4 instanceMatrix_2;
layout(location=8) in vec4 instanceMatrix_3;
layout(location=9) in vec4 instanceMatrix_4;


layout(std140) uniform Matrices{
	mat4 view;
	mat4 projection;
};

out VS_OUT{
	vec3 fragPos;
	vec2 texCoord;
	vec3 normal;
	mat3 TBN;
}vs_out;

void main()
{
	mat4 instanceMatrix = mat4(instanceMatrix_1, instanceMatrix_2, instanceMatrix_3, instanceMatrix_4);
	gl_Position = projection * view * instanceMatrix * vec4(postion, 1.0f);
	vs_out.fragPos = vec3(instanceMatrix * vec4(postion, 1.0f));
	vs_out.texCoord = texCoord;
	vs_out.normal = normalize(transpose(inverse(mat3(instanceMatrix))) * normal);
	vec3 tangent = normalize(mat3(instanceMatrix) * tbn_tangent);
	vec3 bitangent = normalize(mat3(instanceMatrix) * tbn_bitangent);
	vec3 t_normal = normalize(mat3(instanceMatrix) * tbn_normal);
	vs_out.TBN = mat3(tangent, bitangent, t_normal); 
}

