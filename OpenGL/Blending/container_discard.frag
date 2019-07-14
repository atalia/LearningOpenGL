#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D texture1;


void main()
{
	vec4 texColor = texture(texture1, TexCoord);
	//NOTE: GLSL为我们提供了discard命令，它保证了片段不会被进一步处理，这样就不会进入颜色缓冲。
	if(texColor.a < 0.1f)
		discard;
	color = texColor;
	//color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}