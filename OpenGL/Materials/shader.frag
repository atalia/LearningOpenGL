#version 330 core

in vec3 Normal;
in vec3 fragPos;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

void main()
{
	//环境光
	vec3 ambient = material.ambient * lightColor;
	
	//漫反射光照
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * material.diffuse * lightColor;

	//镜面光照
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * material.specular * lightColor;

	vec3 result = ambient + diffuse + specular;
    //vec3 result = specular * objectColor;
	color = vec4(result, 1.0f);
}