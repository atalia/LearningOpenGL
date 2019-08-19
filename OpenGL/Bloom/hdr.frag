#version 330 core

in VS_OUT{
	vec2 texCoords;
}fs_in; 

out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	vec3 colorbase = texture(screenTexture, fs_in.texCoords).rgb;
	color = vec4(colorbase, 1.0f);
}