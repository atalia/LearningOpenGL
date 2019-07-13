# 总结下Stencil Test使用方法

## 背景
当片段着色器处理完片段之后，模板测试(Stencil Test) 就开始执行了，和深度测试一样，它能丢弃一些片段。仍然保留下来的片段进入深度测试阶段，深度测试可能丢弃更多。模板测试基于另一个缓冲，这个缓冲叫做模板缓冲(Stencil Buffer)。

## API
1. 开启 Stencil Test
```c++
// 开启Stencil Test
glEnable(GL_STENCIL_TEST)
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
StencilMask(0xFF);
//Stencil Buffer不可以写入
StencilMask(0x00);
```
4. Stencil Buffer 判断函数
```c++
void glStencilFunc(GLenum func,  GLint ref, GLuint mask);
```
- **func**：设置模板测试操作。这个测试操作应用到已经储存的模板值和glStencilFunc的ref值上，可用的选项是：GL_NEVER、GL_LEQUAL、GL_GREATER、GL_GEQUAL、GL_EQUAL、GL_NOTEQUAL、GL_ALWAYS。它们的语义和深度缓冲的相似。
- **ref**：指定模板测试的引用值。模板缓冲的内容会与这个值对比。
- **mask**：指定一个遮罩，在模板测试对比引用值和储存的模板值前，对它们进行按位与（and）操作，初始设置为1。  
>> *For example*
>> 
    ```c++
    glStencilFunc(GL_ALWAYS, 1, 0xFF); //Stencil Test always pass
    ```

5. Stencil Buffer 判断后处理函数
```c++

```