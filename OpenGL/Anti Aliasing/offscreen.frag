#version 330 core

in VS_OUT{
    vec2 texCoord;
}fs_in;

out vec4 color;

uniform sampler2D texture0;

void main()
{
    color = texture(texture0, fs_in.texCoord);
    //color = vec4(1.0f,1.0f,1.0f,1.0f);
}
