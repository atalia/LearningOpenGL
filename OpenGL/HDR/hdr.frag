#version 330 core

in VS_OUT{
	vec2 texCoords;
}fs_in; 

out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	color = texture(screenTexture, fs_in.texCoords);
}