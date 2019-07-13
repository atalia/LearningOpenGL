# 总结下Stencil Test使用方法

## 背景

当片段着色器处理完片段之后，模板测试(Stencil Test) 就开始执行了，和深度测试一样，它能丢弃一些片段。仍然保留下来的片段进入深度测试阶段，深度测试可能丢弃更多。模板测试基于另一个缓冲，这个缓冲叫做模板缓冲(Stencil Buffer)。

## API

1. 开启 Stencil Test

```c++
// 开启Stencil Test
glEnable(GL_STENCIL_TEST)
```

```c++
// 关闭Stencil Test
glDisable(GL_STENCIL_TEST)
```

2. 清空Stencil Buffer

```c++
glClear(GL_STENCIL_BUFFER_BIT)
```

3. 控制Stencil Buffer是否能够写入

```c++
//Stncil Buffer可以写入
StencilMask(0xFF)
```

```c++
//Stencil Buffer不可以写入
StencilMask(0x00)

```

4. Stencil Buffer 判断函数

```c++
void glStencilFunc(GLenum func,  GLint ref, GLuint mask)
```

- ```func```：设置模板测试操作。这个测试操作应用到已经储存的模板值和glStencilFunc的ref值上，可用的选项是：GL_NEVER、GL_LEQUAL、GL_GREATER、GL_GEQUAL、GL_EQUAL、GL_NOTEQUAL、GL_ALWAYS。它们的语义和深度缓冲的相似。
- ```ref```：指定模板测试的引用值。模板缓冲的内容会与这个值对比。
- ```mask```：指定一个遮罩，在模板测试对比引用值和储存的模板值前，对它们进行按位与（and）操作，初始设置为1。  

> **For example**

```c++
glStencilFunc(GL_ALWAYS, 1, 0xFF)//Stencil Test always pass
```

> **Note**  

```glStencilFunc```只描述了OpenGL对模板缓冲做什么，而不是描述我们如何更新缓冲。
5. Stencil Buffer 判断后处理函数

```c++
void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
```

- ```sfail```： 如果模板测试失败将采取的动作。
- ```dpfail```： 如果模板测试通过，但是深度测试失败时采取的动作。
- ```dppass```： 如果深度测试和模板测试都通过，将采取的动作。  
> **可用参数**

|操作|描述|
|:--:|:--:|
|GL_KEEP|  保持现有的模板值|
|GL_ZERO|  将模板值置为0|
|GL_REPLACE|  将模板值设置为用```glStencilFunc```函数设置的ref值|
|GL_INCR|  如果模板值不是最大值就将模板值+1|
|GL_INCR_WRAP|	与GL_INCR一样将模板值+1，如果模板值已经是最大值则设为0|
|GL_DECR|	如果模板值不是最小值就将模板值-1|
|GL_DECR_WRAP|	与GL_DECR一样将模板值-1，如果模板值已经是最小值则设为最大值|
|GL_INVERT|	Bitwise inverts the current stencil buffer value.|

> *For example*

```c++
glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP) //任何测试的任何结果，模板缓冲都会保留它的值
```

> *总结*

```c++
glStencilFunc(GL_ALWAYS, 1, 0xFF); //Stencil Test 总是pass，此时的ref没有发挥用
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);// Stencil Test 和 Depth Test都pass的话，将Stencil Buffer 设置为ref，即上面的1，ref发生作用
```

## 物体轮廓操作流程  

```c++
//开启 Stencil Testing
glEnable(Stencil Test);
glStencilMask(0xFF); 
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//绘制不需要轮廓的物体
glStencilMask(0x00); 
drawSomeThingDoWithoutOutline();

//绘制需要轮廓的物体，先录制Stencil Buffer，物体的原始位置为1
glStencilMask(0xFF); 
glStencilFunc(GL_ALWAYS, 1, 0xFF); 
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
drawSomeThingNeedOutline();//绘制需要轮廓的原始物体


glStencilMask(0x00); 
glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //如果Stencil Buffer的值不等于1，就绘制该片段
//如果下面没有glStencilOp上述语句等价于
//glStencilFunc(GL_EQUAL, 0, 0xFF); 
glDisable(GL_DEPTH_TEST);
drawScaledSomeThingNeedOutline();//绘制需要轮廓的放大物体
glStencilMask(0xFF); 
glEnable(GL_DEPTH_TEST);

```

## 轮廓绘制效果图展示

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Stencil%20Testing/Stencil%20Testing.png)
![image](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Stencil%20Testing/Stencil%20Testing.gif)
