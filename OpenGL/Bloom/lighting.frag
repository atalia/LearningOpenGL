#version 330 core
out vec4 color;

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
	color = vec4(ambient + lighting, 1.0f);
}