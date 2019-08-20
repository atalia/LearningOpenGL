#version 330 core

in VS_OUT{
	vec2 texCoords;
}fs_in; 

out vec4 color;

uniform sampler2D screenTexture;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec3 colorbase = texture(screenTexture, fs_in.texCoords).rgb;
    vec2 texOffset = 1/ textureSize(screenTexture, 0);
    for(int i = 0; i< 5;++i)
    {
        
    }
	color = vec4(colorbase, 1.0f);
}