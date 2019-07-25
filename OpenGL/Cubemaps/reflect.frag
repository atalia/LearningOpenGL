#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 color;

uniform vec3 viewPosition;
uniform samplerCube skybox;


void main()
{
    vec3 I = normalize(fragPosition - viewPosition);
    vec3 R = reflect(I , normalize(fragNormal));
    R.y = - R.y;
    color = texture(skybox, R);
    //color = texture(skybox, fragPostion);
    //vec3 position = fragPosition + vec3(0.5f ,0.5f, 0.5f);
    //color = vec4(position, 1.0f);
}