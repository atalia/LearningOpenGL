# 几何着色器(Geometry Shader)

几何着色器 输入被声明为一个数组，因为大多数渲染基本图形由一个以上顶点组成，几何着色器接收一个基本图形的所有顶点作为它的输入。

```glsl
#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in VS_OUT{
    vec3 color;
}geo_in[];


out GEO_OUT{
    vec3 color;
}geo_out;


void main()
{
    geo_out.color = geo_in[0].color;
    gl_Position = gl_in[0].gl_Position + vec4(-0.1f, 0.0f, 0.0f, 0.0f);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1f, 0.0f, 0.0f, 0.0f);
    EmitVertex();
    EndPrimitive();
}
```

上述代码为一个几何着色器，其中输入的接口块(interface block）VS_OUT 在几何着色器代码中为

```glsl
out VS_OUT{
    vec3 color;
}vs_out;

```

可以看出几何着色器输入均以数组的形式存在！
