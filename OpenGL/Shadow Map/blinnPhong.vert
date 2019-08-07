#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoords;

out VS_OUT
{
    vec3 normal;
    vec2 texcoords;
    vec3 fragPositon;
}vs_out;

layout(std140) uniform Matrices
{
    mat4 project;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = project * view * model * vec4(position, 1.0f);
    vs_out.normal = normal;
    vs_out.texcoords = texcoords;
    vs_out.fragPositon = position;
}