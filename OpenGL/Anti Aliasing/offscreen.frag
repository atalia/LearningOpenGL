#version 330 core

in VS_OUT{
    vec2 texCoord;
}fs_in;

out vec4 color;

uniform sampler2D texture0;

void main()
{
    color = texture(texture0, fs_in.texCoord);
}
