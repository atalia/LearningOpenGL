/* 
测试贴图用片段着色器
*/
#version 330 core

out vec4 color;

in VS_OUT{
	vec2 texCoord;
}fs_in;

uniform sampler2D screenTexture;

void main()
{
	color = texture(screenTexture, fs_in.texCoord);
}