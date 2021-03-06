#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture_1;
uniform sampler2D Texture_2;

void main()
{
    color = mix(texture(Texture_1, TexCoord) ,texture(Texture_2, TexCoord), 0.4) ;
	//color = vec4(ourColor, 1.0f);
}