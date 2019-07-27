# 高级GLSL

## 官方WIKI

https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)

## 顶点着色器

### ```gl_PointSize```

> 着色器中影响点的大小默认是关闭的，当渲染的primitive是GL_POINTS时，你可以以像素的方式设置点的高度和宽度  

- ```gl_PointSize```设置需要OpenGL 开启```glEnable(GL_PROGRAM_POINT_SIZE)```

*For example*

把点的大小设置为裁切空间的z值，这样点的大小就等于顶点距离观察者的距离，这是一种影响点的大小的方式。当顶点距离观察者更远的时候，它就会变得更大。

```c++
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

```gl_FragCoord```的x和y元素是当前片段的**窗口空间坐标**（window-space coordinate）。它们的起始处是窗口的左下角。如果我们的窗口是800×600的，那么一个片段的窗口空间坐标x的范围就在0到800之间，y在0到600之间。

我们可以使用片段着色器基于片段的窗口坐标计算出一个不同的颜色。gl_FragCoord变量的一个常用的方式是与一个不同的片段计算出来的视频输出进行对比，通常在技术演示中常见。比如我们可以把屏幕分为两个部分，窗口的左侧渲染一个输出，窗口的右边渲染另一个输出。下面是一个基于片段的窗口坐标的位置的不同输出不同的颜色的片段着色器。

```c++
void main()
{
    if(gl_FragCoord.x < 400)
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else
        color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Advanced%20GLSL/advanced_glsl_fragcoord.png)

