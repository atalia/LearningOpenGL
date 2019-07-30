#version 330 core


out vec4 color;

uniform sampler2DMS screenTextureMS;

void main()
{
    //color = texelFetch(screenTextureMS, ivec2(TexCoord), 3);
    color = texelFetch(screenTextureMS, ivec2(gl_FragCoord.xy), 3);
    /*
    if(gl_FragCoord.x < 400.0f)
        color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    else
        color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    */
}