#version 330 core

in VS_OUT
{
	vec3 normal;
	vec2 texCoord;
}fs_in;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	//color = texture(ourTexture, fs_in.texCoord);
	color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
