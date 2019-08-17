#version 330 core

in VS_OUT{
	vec2 texCoord;
	vec3 TBNFragPos;
	vec3 TBNViewPos;
	vec3 TBNLightPos;
}fs_in;

out vec4 color;


uniform vec3 lightColor;
uniform float height_scale;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D parallaxTexture;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
	const float minLayers =  8.0f;
	const float maxLayers = 32.0f;
	float numLayers = mix(maxLayers, minLayers, abs(dot( vec3(0.0f, 0.0f , 1.0f), viewDir))); // mix(a,b, f) = a*(1-f) + b*f
	//float numLayers = 10.0f;
	float layerDepth = 1.0f / numLayers;
	float currentLayerDepth = 0.0f;
	vec2 deltaTexCoords = viewDir.xy / viewDir.z * height_scale * layerDepth;

	vec2 currentTexCoords = texCoords;
	float currentMapValue = texture(parallaxTexture, currentTexCoords).r;

	while (currentMapValue > currentLayerDepth)
	{
		currentTexCoords -= deltaTexCoords;
		currentLayerDepth += layerDepth;
		currentMapValue = texture(parallaxTexture, currentTexCoords).r;
	}
	
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
	float beforeDepth = texture(parallaxTexture, prevTexCoords).r - currentLayerDepth + layerDepth;
	float afterDepth = currentMapValue - currentLayerDepth;
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords;
}

void main()
{
	vec3 viewDir = normalize(fs_in.TBNViewPos - fs_in.TBNFragPos);
	vec2 texCoords = parallaxMapping(fs_in.texCoord, viewDir);
	//vec2 texCoords = fs_in.texCoord;
	color = vec4(texCoords.x, texCoords.x, texCoords.x, texCoords.x);
	
	if(texCoords.x > 1.0f || texCoords.x < 0.0f || texCoords.y > 1.0f || texCoords.y < 0.0f)
		discard;
	vec3 baseColor = texture(diffuseTexture, texCoords).rgb;
	vec3 ambient = 0.05 * baseColor;
	
	vec3 lightDir = normalize(fs_in.TBNLightPos - fs_in.TBNFragPos);
	
	vec3 normal = texture(normalTexture, texCoords).rgb;

	normal = normalize(2 * normal - 1.0f);

	float diff = max(dot(lightDir , normal), 0.0f);
	
	vec3 diffuse = diff * lightColor * baseColor;
	
	//vec3 viewDir = normalize(fs_in.TBNViewPos - fs_in.TBNFragPos);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);

	vec3 specular = spec * lightColor * baseColor;

	color = vec4(ambient + diffuse + specular, 1.0f);
	
}