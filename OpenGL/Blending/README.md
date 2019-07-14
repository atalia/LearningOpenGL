# 混合(Blending)

## 背景  

在OpenGL中，物体透明技术通常被叫做混合(Blending)。透明是物体（或物体的一部分）非纯色而是混合色，这种颜色来自于不同浓度的自身颜色和它后面的物体颜色。一个有色玻璃窗就是一种透明物体，玻璃有自身的颜色，但是最终的颜色包含了所有玻璃后面的颜色。这也正是混合这名称的出处，因为我们将多种（来自于不同物体）颜色混合为一个颜色，透明使得我们可以看穿物体。

## 忽略片段

### 需求介绍

> 有些图像并不关心半透明度，但也想基于纹理的颜色值显示一部分。例如，创建像草这种物体你不需要花费很大力气，通常把一个草的纹理贴到2D四边形上，然后把这个四边形放置到你的场景中。可是，草并不是像2D四边形这样的形状，而只需要显示草纹理的一部分而忽略其他部分。

#### **实现方式**

借助GLSL的```discard```指令，在片段着色器中，增加逻辑判断，实现目的。

> *For Example*

```glsl
#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D texture1;


void main()
{
	vec4 texColor = texture(texture1, TexCoord);
	//NOTE: GLSL为我们提供了discard命令，它保证了片段不会被进一步处理，这样就不会进入颜色缓冲。
	if(texColor.a < 0.1f) //如果片段对应贴图采样区域的alpha为0，此时不处理该片段
		discard;
	color = texColor;
}
```

### 效果展示

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/discard.png)

### **Note**

1. 丢弃片段的方式，不能使我们获得渲染半透明图像
2. 对于全透明物体，比如草叶，我们选择简单的丢弃透明像素而不是混合，这样就减少了令我们头疼的问题（没有深度测试问题）。

