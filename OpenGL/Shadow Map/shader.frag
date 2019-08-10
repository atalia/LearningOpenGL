#version 330 core

in VS_OUT{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec4 fragPosInLightSpace;
}fs_in;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float calculateShadow()
{
	vec3 projCoords = fs_in.fragPosInLightSpace.xyz/fs_in.fragPosInLightSpace.w;
	//fs_in.fragPosInLightSpace.xy NDC
	projCoords = projCoords.xyz * 0.5 + 0.5;
	float depthValueInShadowmap = texture(shadowMap, projCoords.xy).r;
	float shadow = projCoords.z <= depthValueInShadowmap ? 0.0f : 1.0f;
	return shadow;
}

void main()
{
	vec3 baseColor = texture(diffuseTexture, fs_in.texCoords).rgb;
	vec3 ambient = 0.15 * baseColor;

	vec3 normal = normalize(fs_in.normal);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	float diffuseFactor = max(dot(lightDir , normal), 0.0f);
	vec3 diffuse = diffuseFactor * lightColor * baseColor;

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = viewPos - fs_in.fragPos;
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(halfwayDir, normal), 0.0),64.0);

	vec3 specular = spec * lightColor * baseColor;

	float shadow = calculateShadow();

	vec3 light = ambient + (1.0f - shadow) * (diffuse + specular);

	color = vec4(light , 1.0f);
	//color = texture(diffuseTexture, fs_in.texCoords);
	//color = mix(texture(shadowMap, fs_in.texCoords), texture(diffuseTexture, fs_in.texCoords), 0.5);
	//color = vec4(0.5f, 0.5f, 0.5f, 0.5f);
}