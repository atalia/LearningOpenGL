#version 330 core

layout(location=0) in vec2 position;
layout(location=1) in vec3 color;
layout(location=2) in vec2 offset;

out VS_OUT
{
    vec3 color;
} vs_out;

/* 对应Instancing.cpp 中方法1使用
layout (std140) uniform OFFSET
{
    vec2 offsets[100];
};
*/
/*对应Instancing.cpp 中方法2使用
uniform vec2 offsets[100];
*/
void main()
{
    //方法1，方法2使用
    //gl_Position = vec4(position + offsets[gl_InstanceID], 0.0f,  1.0f);
    //方法2使用
    gl_Position = vec4(position + offset, 0.0f,  1.0f);
    vs_out.color = color;
}
