#version 330 core

out vec4 fragColor;

in VS_OUT{
	vec3 fragPos;
	vec2 texCoord;
	vec3 normal;
	vec3 t;
	vec3 b;
	vec3 n;
	mat3 model;
	mat3 TBN;
}fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform vec3 lightPos;

void main()
{
	vec3 baseColor = texture(diffuseTexture, fs_in.texCoord).rgb;
	vec3 ambient = 0.05 * baseColor;
	vec3 normal = normalize(fs_in.TBN * texture(normalTexture, fs_in.texCoord).rgb);
	//normal = vec3(0.5f, 0.5f, 0.5f) + 0.5f * normal;
	//normal = fs_in.normal;
	//fragColor = vec4(fs_in.b, 1.0f);
	mat3 m = fs_in.model * fs_in.TBN;
	fragColor = vec4(fs_in.b , 1.0f);
}
