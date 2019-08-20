#version 330 core

in VS_OUT{
	vec2 texCoords;
}fs_in; 

out vec4 color;

uniform bool horizontal;

uniform sampler2D screenTexture;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec3 result = weight[0] * texture(screenTexture, fs_in.texCoords).rgb;
    vec2 texOffset = 1.0 / textureSize(screenTexture, 0);
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += weight[i] * texture(screenTexture, fs_in.texCoords + vec2(i * texOffset.x, 0)).rgb;
            result += weight[i] * texture(screenTexture, fs_in.texCoords - vec2(i * texOffset.x, 0)).rgb;
        }
    }
    else
    {
         for(int i = 1; i < 5; ++i)
        {
            result += weight[i] * texture(screenTexture, fs_in.texCoords + vec2(0, i * texOffset.y)).rgb;
            result += weight[i] * texture(screenTexture, fs_in.texCoords - vec2(0, i * texOffset.y)).rgb;
        }
    }
	color = vec4(result, 1.0f);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}