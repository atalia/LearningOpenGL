# 帧缓冲(Frame Buffer)

## 总结

在OpenGL中，渲染管线中的顶点、纹理等经过一系列处理后，最终显示在2D屏幕设备上，渲染管线的最终目的地就是**帧缓冲区**。

帧缓冲区包括

1. 颜色缓冲区(Color buffer)
2. 深度缓冲区(Depth buffer)
3. 模板缓冲区(Stencil buffer)

帧缓冲区可以分类两类

1. 窗口系统提供的帧缓冲区(window-system-provided framebuffer)
2. 应用程序帧缓冲区(application-created framebuffer object)


### 窗口系统提供的帧缓冲区

窗口系统会自动创建 默认的缓冲区，这个默认的缓冲区是至今为止我们一直所使用命令的作用对象。

### 应用程序帧缓冲区

OpenGL也允许我们手动创建一个帧缓冲区，并将渲染结果**重定向**到这个缓冲区。在创建时允许我们自定义帧缓冲区的一些特性，这个自定义的帧缓冲区同默认的帧缓冲区一样，自定义的帧缓冲区也包含颜色缓冲区、深度和模板缓冲区。

帧缓冲 是渲染管线的输出结果， 是窗口系统渲染画面的对象

## 帧缓冲对象(Frame buffer object, FBO)

OpenGL中，通过创建一个帧缓冲对象(FBO)实现对自定义缓冲区的管理, 自定义的缓冲区也包含

1. 颜色缓冲区(Color buffer)
2. 深度缓冲区(Depth buffer)
3. 模板缓冲区(Stencil buffer)

这3类缓冲区，这些逻辑上的缓冲区(logic buffers)在FBO称之为可附加对象(attachment)，存储形式均为二维像素数组。  
进一步地，创建可附加对象(attachment) 有2种类型选择:

1. Texture object attachments. (纹理对象附件, 可选附加类型如下)

 > - Color buffer
 > - Depth color  
 > - Stencil color

2. Renderbuffer object attachments. （渲染缓冲对象附件, 可选附加类型如下）
> - Depth buffer
> - Stencil Buffer

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/fbo.png)

一个完整的帧缓冲要求满足以下[条件](https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/05%20Framebuffers/):

1. 至少加入一个附件（颜色、深度、模板缓冲）
2. 至少有一个是颜色附件
3. 所有的附件都应该是已经完全做好的（已经存储在内存之中）
4. 每个缓冲都应该有同样数目的样本。

### 帧缓冲API

1. 创建帧缓冲

```c++
GLuint fbo;
glGenFramebuffers(1, &fbo);
```

2. 绑定帧缓冲

```c++
glBindFramebuffer(GL_FRAMEBUFFER, fbo);
```

3. 解绑帧缓冲

> 绑定自定义的帧缓冲后，后续所有渲染操作将渲染到当前绑定的帧缓冲的附加缓冲中，由于我们的帧缓冲不是默认的帧缓冲，渲染命令对窗口的视频输出不会产生任何影响。出于这个原因，它被称为离屏渲染（off-screen rendering），就是渲染到一个另外的缓冲中。为了让所有的渲染操作对主窗口产生影响我们必须通过绑定为0来使默认帧缓冲被激活

```c++
glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

4. 删除帧缓冲

```c++
glDeleteFramebuffers(1, &fbo);
```

5. 检查帧缓冲是否准备满足条件

```c++
glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE
```

## 纹理对象附件(Texture object attachments)

当把一个纹理附加到帧缓冲上的时候，所有渲染命令会写入到纹理上，就像它是一个普通的颜色/深度或者模板缓冲一样。使用纹理的好处是，所有渲染操作的结果都会被储存为一个纹理图像，这样我们就可以简单的在着色器中使用了。
使用纹理对象附件的缺点就是需要针对特定纹理格式转化OpenGL原生渲染格式数据。但相对于renderbuffer object attachments,  texture object attachments **CPU可调用**


### 流程

1. 生成帧缓冲纹理Texture
```c++
GLuint texture;
glGenTextures(1, &texture);
//颜色纹理附件
glBindTexture(GL_TEXTURE_2D, texture);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

//深度和模板纹理附件
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureDepthStencilbuffer, 0);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```
2. 将生成的Texutre附加到帧缓冲上

```c++
//帧缓冲绑定颜色附件
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, texture, 0);
//帧缓冲绑定深度和模板附件
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureDepthStencilbuffer, 0);
```

### 结果展示

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/depthstencil2texture.png)

## 渲染缓冲对象附件(Renderbuffer object attachments)

渲染缓冲对象也是一个缓冲，它可以是一堆字节、整数、像素或者其他东西。渲染缓冲对象的一大优点是，它以OpenGL原生渲染格式储存它的数据，因此在离屏渲染到帧缓冲的时候，这些数据就相当于被优化过的了。

渲染缓冲对象将所有渲染数据直接储存到它们的缓冲里，而不会进行针对特定纹理格式的任何转换，这样它们就成了一种快速可写的存储介质了。但是渲染缓冲对象只存在GPU中，**CPU无法读取**。


> Since renderbuffer objects are generally write-only they are often used as depth and stencil attachments, since most of the time we don't really need to read values from the depth and stencil buffers but still care about depth and stencil testing. We need the depth and stencil values for testing, but don't need to sample these values so a renderbuffer object suits this perfectly. When we're not sampling from these buffers, a renderbuffer object is generally preferred since it's more optimized.

> 由于渲染缓冲对象通常是只写的，它们经常作为深度和模板附件来使用，由于大多数时候，我们不需要从深度和模板缓冲中读取数据，但仍关心深度和模板测试。我们就需要有深度和模板值提供给测试，但不需要对这些值进行采样（sample），所以深度缓冲对象是完全符合的。当我们不去从这些缓冲中采样的时候，渲染缓冲对象通常很合适，因为它们等于是被优化过的。

### 流程

1. 创建Renderbuffer object

```c++
GLuint rbo;
glGenRenderbuffers(1, &rbo);
```

2. 绑定渲染缓冲

```c++
glBindRenderbuffer(GL_RENDERBUFFER, rbo);
```

3. 创建一个深度和模板渲染缓冲对象

```c++
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
```

4. 渲染缓冲(RBO)绑定帧缓冲对象(FBO)

```c++
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
```

### 效果

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/depthstencil2rbo.png)

## 后处理

给当前帧输出增加一些效果

### 流程

1. 创建并绑定自定义缓冲对象
2. 将颜色缓冲绑定纹理对象附件
3. 将深度和模板缓冲绑定 渲染缓冲对象
4. 绘制当前帧
5. 将纹理绘制到窗口默认缓冲区
6. 处理颜色纹理附件

```c++
//1
GLuint fbo;
glGenFramebuffers(1, &fbo);
glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//2
GLuint textureColorbuffer = generateAttachmentTexture(false, false);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
//3
GLuint rbo;
glGenRenderbuffers(1, &rbo);
glBindRenderbuffer(GL_RENDERBUFFER, rbo);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
glBindRenderbuffer(GL_RENDERBUFFER, 0);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
glBindFramebuffer(GL_FRAMEBUFFER, 0);

...

glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//5
initBuffer();
drawSomething();

glBindFramebuffer(GL_FRAMEBUFFER, 0);

//6
initBuffer();
ShaderpostProcess.use();
drawColorBufferTexture();

```
### 效果

```c++
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/depthstencil2rbo.png)
 
```c++
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
```

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/depthstencil2rbo.png)

### [一些涉及图像处理的应用](https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/05%20Framebuffers/)

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/shaper.png)


### 后视镜效果

> 实现思路: 绘制场景两次:一次正常绘制,另一次摄像机旋转180度后绘制.

![image](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/rearview%20mirror.gif)
