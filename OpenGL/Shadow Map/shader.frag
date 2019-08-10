#version 330 core

in VS_OUT{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec3 fragPosInLightSpace;
}fs_in;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 baseColor = texture(diffuseTexture, fs_in.texCoords).rgb;
	vec3 ambient = 0.15 * baseColor;

	vec3 normal = normalize(fs_in.normal);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	float diffuseFactor = max(dot(lightDir , fs_in.normal), 0.0f);
	vec3 diffuse = diffuseFactor * lightColor * baseColor;

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = viewPos - fs_in.fragPos;
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(halfwayDir, fs_in.normal), 0.0),64.0);

	vec3 specular = spec * lightColor * baseColor;

	float shadow = 0.0f;

	color = vec4(ambient + diffuse + specular, 1.0f);

}