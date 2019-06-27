#version 330 core

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;


struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

void main()
{
	//环境光
	vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
	
	//漫反射光照
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords))* light.diffuse;

	//镜面光照
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, TexCoords)) * light.specular;

	vec3 result = ambient + diffuse + specular;
    //vec3 result = specular * objectColor;
	color = vec4(result, 1.0f);
}