# 高级GLSL

## 官方WIKI

https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)

## 顶点着色器

### ```gl_PointSize```

> 着色器中影响点的大小默认是关闭的，当渲染的primitive是GL_POINTS时，你可以以像素的方式设置点的高度和宽度  

- ```gl_PointSize```设置需要OpenGL 开启```glEnable(GL_PROGRAM_POINT_SIZE)```

*For example*

把点的大小设置为裁切空间的z值，这样点的大小就等于顶点距离观察者的距离，这是一种影响点的大小的方式。当顶点距离观察者更远的时候，它就会变得更大。

```glsl
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    gl_PointSize = gl_Position.z;
}
```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Advanced%20GLSL/advanced_glsl_pointsize.png)

### ```gl_VertexID```

```gl_VertexID```是个整型变量，它储存着我们绘制的当前顶点的ID。

- 当进行索引渲染（indexed rendering，使用```glDrawElements```渲染）时，这个变量保存着当前绘制的顶点的**索引**。

- 当用的不是索引绘制（```glDrawArrays```）时，这个变量保存的是从渲染开始起直到当前处理的这个顶点的（当前顶点）**编号**。

## 片段着色器

### ```gl_FragCoord```

```glsl
void main()
{
    if(gl_FragCoord.x < 400)
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else
        color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Advanced%20GLSL/advanced_glsl_fragcoord.png)

### ```gl_FrontFacing```

```c++
#version 330 core
out vec4 color;
in vec2 TexCoords;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{
    if(gl_FrontFacing)
        color = texture(frontTexture, TexCoords);
    else
        color = texture(backTexture, TexCoords);
}
```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Advanced%20GLSL/advanced_glsl_frontfacing.png)

### ```gl_FragDepth```

|```gl_FragCoord```|```gl_FragDepth```|
|:----------------:|:----------------:|
|**只读**         |            **可写**|

> 如果着色器中没有像```gl_FragDepth```变量写入，它就会自动采用gl_FragCoord.z的值。

因为只要我们在片段着色器中对```gl_FragDepth```写入什么，OpenGL就会**关闭所有的前置深度测试**。它被关闭的原因是，在我们运行片段着色器之前OpenGL搞不清像素的深度值，因为片段着色器可能会完全改变这个深度值。

OpenGL 4.2

```glsl
layout (depth_<condition>) out float gl_FragDepth;
```

condition 值

|**Condition**|**描述**|
|:-:|:-:|
|any|默认值.|前置深度测试是关闭的，你失去了很多性能表现|
|greater|深度值只能比gl_FragCoord.z大|
|less|深度值只能设置得比gl_FragCoord.z小|
|unchanged|如果写入gl_FragDepth, 你就会写gl_FragCoord.z|

> 如果把深度条件定义为greater或less，OpenGL会假定你只写入比当前的像素深度值的深度值大或小的。

```glsl
#version 330 core
layout (depth_greater) out float gl_FragDepth;
out vec4 color;

void main()
{
    color = vec4(1.0f);
    gl_FragDepth = gl_FragCoord.z + 0.1f;
}
```

# 接口块(Interface blocks)

Vertex Shader

```glsl
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec2 TexCoords;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.TexCoords = texCoords;
}
```

Fragment Shader

```glsl
#version 330 core
out vec4 color;

in VS_OUT
{
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture;

void main()
{
    color = texture(texture, fs_in.TexCoords);
}
```

上述着色器代码中，VS_OUT为接口块，块名（block name）应该是一样的，但是实例名(vs_out, fs_in)可以是任意的。

# Uniform缓冲对象(Uniform buffer objects)

```glsl
#version 330 core
layout (location = 0) in vec3 position;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
}
```

前面，大多数例子里我们在每次渲染迭代，都为projection和view矩阵设置uniform。这个例子里使用了uniform缓冲对象，这非常有用，因为这些矩阵我们设置一次就行了。

> 在uniform块中的变量可以直接获取，而不用使用block名作为前缀。接着我们在缓冲中储存这些矩阵的值，每个声明了这个uniform块的着色器都能够获取矩阵。例如上述代码中project和view获取不需要使用Matrices

## Uniform块布局(Uniform block layout)

一个uniform块的内容被储存到一个缓冲对象中，实际上就是在一块内存中。因为这块内存也不清楚它保存着什么类型的数据，我们就必须告诉OpenGL哪一块内存对应着色器中哪一个uniform变量。

着色器一个Uniform块我们所希望知道的是每个变量的**大小**（以字节为单位）和**偏移量**（从block的起始处）

- 大小：(C++决定)每个元素的大小在OpenGL中都很清楚，直接与C++数据类型呼应，向量和矩阵是一个float序列（数组）。
- 偏移：(默认硬件决定)硬件能以它认为合适的位置方式变量。比如有些硬件可以在float旁边放置一个vec3。不是所有硬件都能这样做，在vec3旁边附加一个float之前，给vec3加一个边距使之成为4个（空间连续的）float数组。

GLSL布局

1. shared layout
2. packed layout
3. std140 layout

|layout|变量偏移量|
|:--:|:--:|
|shared layout|硬件决定，程序共享（一致）|
|packed layout|不确定，同一硬件上程序之间存在差异，因为编译器会优化|
|std140|规则决定，与硬件无关|

GLSL 默认使用的uniform内存布局叫做共享布局(shared layout)，叫共享是因为一旦偏移量被**硬件**定义，它们就会持续地被多个程序所共享。不同硬件使用之前需要查询。

### [std140布局](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt)

GLSL中每个变量类型比如int、float和bool被定义为4字节，每4字节被表示为N。

|类型|布局规范|
|:-:|:-:|
|像int和bool这样的标量|每个标量的基线为N|
|向量|每个向量的基线是2N或4N大小。这意味着vec3的基线为4N|
|标量与向量数组|每个元素的基线与vec4的相同|
|矩阵|被看做是存储着大量向量的数组，每个元素的基数与vec4相同|
|结构体|根据以上规则计算其各个元素，并且间距必须是vec4基线的倍数|

## Uniform缓冲

### API

1. 创建缓冲，并绑定对象

```c++
GLuint uboExampleBlock;
glGenBuffers(1, &uboExampleBlock);
glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
glBufferData(GL_UNIFORM_BUFFER, 150, NULL, GL_STATIC_DRAW); // 分配150个字节的内存空间
glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

2. 绑定点

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Advanced%20GLSL/advanced_glsl_binding_points.png)

> **Step1** 把uniform块设置到一个特定的绑定点上

```c++
GLuint lights_index = glGetUniformBlockIndex(shaderA.Program, "Lights");
glUniformBlockBinding(shaderA.Program, lights_index, 2);
```

```glsl
layout(std140, binding = 2) uniform Lights { ... };
```

> **Step2** uniform缓冲对象绑定到同样的绑定点

```c++ 
glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock);
// 或者
glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 150);
```

现在所有事情都做好了，我们可以开始向uniform缓冲添加数据了。我们可以使用glBufferSubData将所有数据添加为一个单独的字节数组或者更新缓冲的部分内容，只要我们愿意。为了更新uniform变量boolean，我们可以借助```glBufferSubData```更新uniform缓冲对象：

```
glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
GLint b = true; // GLSL中的布尔值是4个字节，因此我们将它创建为一个4字节的整数
glBufferSubData(GL_UNIFORM_BUFFER, 142, 4, &b);
glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

## 结语

uniform缓冲对象比单独的uniform有很多好处。

1. 一次设置多个uniform比一次设置一个速度快。

2. 如果你打算改变一个横跨多个着色器的uniform，在uniform缓冲中只需更改一次。

3. 使用uniform缓冲对象你可以在着色器中使用更多的uniform。OpenGL有一个对可使用uniform数据的数量的限制，可以用```GL_MAX_VERTEX_UNIFORM_COMPONENTS```来获取。当使用uniform缓冲对象中，这个限制的阈限会更高。所以无论何时，你达到了uniform的最大使用数量（比如做骨骼动画的时候），你可以使用uniform缓冲对象。
