#version 330 core

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;

struct Material
{
	sampler2D ambient;
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

//定向光、平行光
struct DirectLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectLight directlight;

vec3 calDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
	
	vec3 lightDir = normalize(-light.direction);
	float diff =  max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords)); 
	
	return ambient + diffuse + specular;
}

//点光源
struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};
uniform PointLight pointlight;
vec3 calPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, normal), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	float distance = length(light.position - fragPos);
	float attenuation = 1.0f/(light.constant + light.linear * distance + light.quadratic*distance*distance);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;

}

//聚光
struct SpotLight
{
	vec3 position;
	vec3 dirction;
	float innerCutOff;
	float outCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotlight;
vec3 calSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{	
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(-lightDir, light.dirction);

	float epsilon = light.innerCutOff - light.outCutOff;
	float intensity = clamp((theta-light.outCutOff)/epsilon, 0.0f, 1.0f);


	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	ambient *= intensity;

	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));
	diffuse *= intensity;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, normal), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	specular *= intensity;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos-fragPos);
	/*
	vec3 result = calDirectLight(directlight, norm ,viewDir);
	*/
	vec3 result = calSpotLight(spotlight, norm, fragPos, viewDir);
	color = vec4(result, 1.0f);
}
