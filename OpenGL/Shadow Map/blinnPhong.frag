#version 330 core

in VS_OUT
{
    vec3 normal;
    vec2 texcoords;
    vec3 fragPositon;
}fs_in;

out vec4 color;

uniform sampler2D texture0;

layout(std140) uniform Light{
    vec3 lightPos;
    vec3 viewPos;
    //bool blinn;
};

uniform bool blinn;

void main()
{
    vec4 colorOrigin = texture(texture0, fs_in.texcoords);

    float ambient = 0.05;

    vec3 lightDir = normalize(lightPos - fs_in.fragPositon);
    
    float diff = max(dot(lightDir , fs_in.normal), 0);


    vec3 viewDir = normalize(viewPos - fs_in.fragPositon);

    float spec = 0.0f;
    if(blinn)
    {
        vec3 halfDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(halfDir, fs_in.normal), 0.0), 32.0);
    }
    else
    {   
        vec3 reflectDir = reflect(-lightDir, fs_in.normal);
        spec =  pow(max(dot(reflectDir, viewDir), 0.0), 8.0);
    }
    
    color = vec4((ambient + diff + spec) * colorOrigin.rgb, 1.0f);

}