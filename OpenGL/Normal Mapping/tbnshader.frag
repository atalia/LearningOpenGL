#version 330 core

in VS_OUT{
	vec2 texCoord;
	vec3 TBNFragPos;
	vec3 TBNViewPos;
	vec3 TBNLightPos;
}fs_in;

out vec4 color;


uniform vec3 lightColor;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

void main()
{
	vec3 baseColor = texture(diffuseTexture, fs_in.texCoord).rgb;
	vec3 ambient = 0.05 * baseColor;
	
	vec3 lightDir = normalize(fs_in.TBNLightPos - fs_in.TBNFragPos);
	
	vec3 normal = texture(normalTexture, fs_in.texCoord).rgb;
	normal = normalize(2 * normal - 1.0f);

	float diff = max(dot(lightDir , normal), 0.0f);
	
	vec3 diffuse = diff * lightColor * baseColor;
	
	vec3 viewDir = normalize(fs_in.TBNViewPos - fs_in.TBNFragPos);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);

	vec3 specular = spec * lightColor * baseColor;

	color = vec4(ambient + diffuse + specular, 1.0f);
	//color = vec4(normal * 0.5 + 0.5, 1.0f);
	//color = vec4(fs_in.TBN[2], 1.0f);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}