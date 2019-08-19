#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in VS_OUT{
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
}fs_in;

struct Light{
	vec3 Position;
	vec3 Color;
};

uniform Light lights[4];
uniform sampler2D diffuseTexture;

void main()
{
	vec3 baseColor = texture(diffuseTexture, fs_in.texCoord).rgb;
	vec3 ambient = 0.00 * baseColor;
	vec3 normal = normalize(fs_in.normal);
	vec3 lighting  = vec3(0.0f);
	for(int i = 0; i < 4; ++i)
	{
		vec3 lightDir = normalize(lights[i].Position - fs_in.fragPos);
		float diff = max(dot(lightDir, normal), 0.0f);
		vec3 diffuse = lights[i].Color * diff * baseColor;
		vec3 result = diffuse;
		float distance = length(fs_in.fragPos - lights[i].Position);
		result *= 1.0 / (distance * distance);
		lighting += result;
	}
	vec3 result = ambient + lighting;
	float bright = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if(bright > 1.0f)
		BrightColor = vec4(result, 1.0f);
	else
		BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	FragColor = vec4(result, 1.0f);
}