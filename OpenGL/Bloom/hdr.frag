#version 330 core

in VS_OUT{
	vec2 texCoords;
}fs_in; 

out vec4 color;

uniform float exposure = 1.0f;
uniform bool bloom = true;
uniform sampler2D screenTexture;
uniform sampler2D BrightTexture;

void main()
{
	const float gamma = 2.2f;
	vec3 colorbase = texture(screenTexture, fs_in.texCoords).rgb;
	vec3 bloomcolor = texture(BrightTexture, fs_in.texCoords).rgb;
	if(bloom)
		colorbase += bloomcolor;
	vec3 result = vec3(1.0) - exp(-colorbase * exposure);
	result = pow(result, vec3(1.0f/gamma));
	color = vec4(result, 1.0f);
	//color = vec4(colorbase, 1.0f);
}