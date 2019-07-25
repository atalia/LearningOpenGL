#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 color;

uniform vec3 viewPosition;
uniform samplerCube skybox;
uniform float refraction;

void main()
{
    vec3 I = normalize(fragPosition - viewPosition);
    float ratio = 1.0/refraction;
    vec3 R = refract(I, normalize(fragNormal), ratio);
    R.y = - R.y;
    color = texture(skybox, R);
}