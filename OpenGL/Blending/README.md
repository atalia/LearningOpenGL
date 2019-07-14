# 混合(Blending)

## 背景  

在OpenGL中，物体透明技术通常被叫做混合(Blending)。透明是物体（或物体的一部分）非纯色而是混合色，这种颜色来自于不同浓度的自身颜色和它后面的物体颜色。一个有色玻璃窗就是一种透明物体，玻璃有自身的颜色，但是最终的颜色包含了所有玻璃后面的颜色。这也正是混合这名称的出处，因为我们将多种（来自于不同物体）颜色混合为一个颜色，透明使得我们可以看穿物体。

## 1. 忽略片段

### 需求介绍

> 有些图像并不关心半透明度，但也想基于纹理的颜色值显示一部分。例如，创建像草这种物体你不需要花费很大力气，通常把一个草的纹理贴到2D四边形上，然后把这个四边形放置到你的场景中。可是，草并不是像2D四边形这样的形状，而只需要显示草纹理的一部分而忽略其他部分。

#### **实现方式**

> 借助GLSL的```discard```指令，在片段着色器中，增加逻辑判断，实现目的。

*For Example*

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

## 2. 混合(Blend)

### 需求分析

渲染出不同的透明度级别，我们需要开启混合(Blending)

### API

1. 开启混合

```c++
glEnable(GL_BLEND)
```

2. 设置源和目标因子  

混合公式
```
C_result = C_source * F_source + C_target * F_target
```

> 源：**后**进入颜色缓冲的颜色  
> 目标：**先**进入颜色缓冲的颜色  

**不区分alpha和rgb**

```c++
void glBlendFunc(GLenum sfactor, GLenum dfactor)
```

**区分alpha和rgb**

```c++
void glBlendFuncSeperate(GLenum sfactorRGB, GLenum dfactorRGB,GLenum sfactorAlpha, GLenum dfactorAlpha)
```


#### **```sfactor```, ```dfactor```可选项**

|选项|值|
|:-:|:-:|
|GL_ZERO|0|
|GL_ONE|1|
|GL_SRC_COLOR|源颜色向量 C_source|
|GL_ONE_MINUS_SRC_COLOR|1−C_source|
|GL_DST_COLOR|目标颜色向量C_destination|
|GL_ONE_MINUS_DST_COLOR|1−C_destination|
|GL_SRC_ALPHA|C_source的alpha值|
|GL_ONE_MINUS_SRC_ALPHA|1− C_source的alpha值|
|GL_DST_ALPHA|C_destination的alpha值|
|GL_ONE_MINUS_DST_ALPHA|1− C_destination的alpha值|
|GL_CONSTANT_COLOR|常颜色向量C_constant|
|GL_ONE_MINUS_CONSTANT_COLOR|1−C_constant|
|GL_CONSTANT_ALPHA|C_constant的alpha值|
|GL_ONE_MINUS_CONSTANT_ALPHA|1− C_constant的alpha值|

其中C_constant借助 ```glBlendColor```设置

```c++
void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
```

*For Example*

```c++
glBlendFuncSeperate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ZERO) //源ALPHA影响最终的RGB,且最终的alpha元素为源ALPHA
```


OpenGL给了我们更多的自由，我们可以借助```glBlendEquation```改变方程源和目标部分的操作符。现在，源和目标元素已经相加了。如果我们愿意的话，我们还可以把它们相减。

```c++
void glBlendEquation(GLenum mode)
```

#### **```mode```可选项**

|**```mode```**|**作用**|
|:-:|:-:|
|GL_FUNC_ADD|默认的，彼此元素相加：C_result=Src+Dst|
|GL_FUNC_SUBTRACT|彼此元素相减： C_result=Src−Dst|
|GL_FUNC_REVERSE_SUBTRACT|彼此元素相减，但顺序相反：C_result=Dst−Src|

### 对比实验

```c++
glEnable(GL_BLEND);
glBlendFunc(GL_ZERO, GL_ZERO);
glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
```

- 不开启Blend的效果图

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/0.png)

- ```glBlendFunc(GL_ZERO, GL_ZERO)```



![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/1.png)

这里比较好理解，source和target都乘以一个0， 最终颜色为黑色```(0.0f, 0.0f, 0.0f)```

- ```glBlendFunc(GL_ONE, GL_ZERO)```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/2.png)

这里发现效果和未开启混合一致，因为最终输出颜色只由source决定

- ```glBlendFunc(GL_ZERO, GL_ONE)```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/3.png)

这里发现只有背景色，~~物体并没有渲染出来~~

**Why?**

最直观的原因在于：整个渲染过程，GL会首先将屏幕染色为背景色，渲染第一个物体的时候，需要与背景色混合，背景色为Target，待渲染物体为Source，混合方程设置了最终输出颜色只由Target决定，所以最终输出颜色只是背景色（这就是为什么我上面那句话加了删除线,其实物体还是渲染了的，只是最终算出来的颜色和环境一致而已）。

- ```glBlendFunc(GL_ONE, GL_ONE)```

调整绘制顺序为

1. 箱子
2. 地板

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/4.png)

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/6.png)

这里发现最终输出结果，物体变透明了（需要注意的是，这里的贴图并没有alpha通道）。

进一步地，我们把视角切换到container的内部，发现物体又不是透明的！但是颜色变淡了

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/5.png)

如果将绘制顺序调整为

1. 地板
2. 箱子

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/7.png)

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/8.png)

**不同绘制顺序的结果不一样**
**Why?**

1. 混色是一个更新Color Buffer的过程。通过更新Color Buffer的值，即使贴图没有alpha值，依旧可以实现这种透明的效果，即透明效果的实现只是一个更新Color Buffer的过程(背景色->地板->箱子), alpha值只是给这个过程提供了一个参考（毕竟到现在，我们都没用到带alpha通道的图片）。

2. Depth Testing，因为箱子在地板前，先绘制箱子，后绘制地板，那么地板更新时，是过不了Depth Testing，也就不会影响Color Buffer了。后续会专门讨论这个

### 不带alpha通道的贴图，alpha默认值是？

关闭Blend,将片段着色器代码改为

```glsl
#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D texture1;


void main()
{
	color = texture(texture1, TexCoord).aaaa;
}
```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/9.png)

即默认值为1.0f。

### Blend VS DepthTesting

设置参数如下

```c++
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

绘制顺序为

1. 地板
2. 箱子
3. window

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/10.png)

调整绘制顺序为

1. 地板
2. window
3. 箱子

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/11.png)

进一步地，调整绘制顺序为

1. window
2. 地板
3. 箱子

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/12.png)

### **结论**  

## Blend 发生在Depth Testing 后面，**只有Depth Testing 为Pass状态时，才会有后续的Blend过程！**

## 3. 渲染中使用Blend的原则

1. 要让Blend在多物体上有效，我们必须先绘制最远的物体，最后绘制最近的物体。（保证Depth Testing后能记录远处物体的Color Buffer）

2. 先绘制所有不透明物体。 为所有透明物体排序。（保证不透明物体在Color Buffer中, 保证多层Blend效果正确性）

> 放一个最终的效果图

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Blending/12.png)



