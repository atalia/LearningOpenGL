#version 330 core

in VS_OUT
{
    vec3 normal;
    vec2 texcoords;
    vec3 fragPositon;
}fs_in;

out vec4 color;

uniform sampler2D texture0;

layout(std140) uniform Light{
    vec3 lightPos;
    vec3 viewPos;
    bool blinn;
};

void main()
{
    vec4 colorOrigin = texture(texture0, fs_in.texcoords);

    color = 0.05 * colorOrigin;
}