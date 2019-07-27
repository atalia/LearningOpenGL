#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT{
    vec3 color;
}geo_in[];


out GEO_OUT{
    vec3 color;
}geo_out;

void build(vec4 position)
{
    geo_out.color = geo_in[0].color;

    gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);    // 1:bottom-left   
    EmitVertex();   
    gl_Position = position + vec4( 0.2f, -0.2f, 0.0f, 0.0f);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2f,  0.2f, 0.0f, 0.0f);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2f,  0.2f, 0.0f, 0.0f);    // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0f,  0.4f, 0.0f, 0.0f);    // 5:top
    geo_out.color = vec3(1.0f, 1.0f, 1.0f);
    EmitVertex();
    EndPrimitive();

}

void main()
{
    build(gl_in[0].gl_Position);
}