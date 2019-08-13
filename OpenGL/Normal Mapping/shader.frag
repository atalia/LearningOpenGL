#version 330 core

in VS_OUT{
	vec2 texCoord;
	vec3 fragPos;
	mat3 TBN;
}fs_in;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

void main()
{
	vec3 baseColor = texture(diffuseTexture, fs_in.texCoord).rgb;
	vec3 ambient = 0.05 * baseColor;
	
	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	
	vec3 normal = texture(normalTexture, fs_in.texCoord).rgb;
	normal = 2 * normal - 1.0f;
	normal = normalize(fs_in.TBN * normal);

	float diff = max(dot(lightDir , normal), 0.0f);
	
	vec3 diffuse = diff * lightColor * baseColor;
	
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);

	vec3 specular = spec * lightColor * baseColor;

	//color = vec4(ambient + diffuse + specular, 1.0f);
	color = vec4(normal * 0.5 + 0.5, 1.0f);
}