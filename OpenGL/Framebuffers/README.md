# 帧缓冲(Frame Buffer)

## 背景

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

1. Texture object attachments. (纹理附加对象, 可选附加类型如下)

 > - Color buffer
 > - Depth color  
 > - Stencil color

2. Renderbuffer object attachments. （渲染缓冲附加对象, 可选附加类型如下）

> - Depth buffer
> - Stencil Buffer

![avatar](https://github.com/atalia/LearningOpenGL/blob/master/OpenGL/Framebuffers/fbo.png)

