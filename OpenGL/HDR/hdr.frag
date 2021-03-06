#version 330 core

in VS_OUT{
	vec2 texCoords;
}fs_in; 

out vec4 color;

uniform sampler2D screenTexture;

uniform float exposure;

void main()
{
	const float gamma = 2.2f;
	vec3 colorbase = texture(screenTexture, fs_in.texCoords).rgb;
	//vec3 mapped = colorbase / (colorbase + vec3(1.0f));
	
	vec3 mapped = vec3(1.0f) - exp(-colorbase * exposure);

	mapped = pow(mapped, vec3(1.0f/gamma));
	color = vec4(mapped,1.0f);
}