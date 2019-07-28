#version 330 core

in VS_OUT
{
    vec3 color;
} fs_in;

out vec4 color;

void main()
{
   color = vec4(fs_in.color, 1.0f);
   //color = vec4(1.0f , 1.0f, 1.0f, 1.0f);
}