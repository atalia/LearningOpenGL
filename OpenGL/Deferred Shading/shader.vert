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
	vec3 t;
	vec3 b;
	vec3 n;
	mat3 model;
	mat3 TBN;
}vs_out;

void main()
{
	mat4 model = mat4(instanceMatrix_1, instanceMatrix_2, instanceMatrix_3, instanceMatrix_4);
	gl_Position = projection * view * model * vec4(postion, 1.0f);
	vs_out.fragPos = vec3(model * vec4(postion, 1.0f));
	vs_out.texCoord = texCoord;
	vs_out.normal = normalize(transpose(inverse(mat3(model))) * normal);
	vec3 tangent = vec3(model * vec4(tbn_tangent, 1.0f));
	vec3 bitangent = vec3(model * vec4(tbn_bitangent, 1.0f));
	vec3 t_normal = vec3(model * vec4(tbn_normal, 1.0f));
	vs_out.t = tangent;
	vs_out.b = bitangent;
	vs_out.n = t_normal;
	vs_out.model = mat3(model);
	//vs_out.TBN = mat3(tangent, bitangent, t_normal);
	vs_out.TBN = transpose(mat3(tangent, bitangent, t_normal)); 
}

