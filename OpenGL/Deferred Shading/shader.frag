#version 330 core

out vec4 fragColor;

in VS_OUT{
	vec3 fragPos;
	vec2 texCoord;
	vec3 normal;
	mat3 TBN;
}fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform vec3 lightPos;

void main()
{
	vec3 baseColor = texture(diffuseTexture, fs_in.texCoord).rgb;
	vec3 ambient = 0.05 * baseColor;
	vec3 normal = texture(normalTexture, fs_in.texCoord).rgb;
	normal = normalize(2 * normal - 1.0f);
	fragColor = vec4(normalize(fs_in.TBN * normal), 1.0f);
}
