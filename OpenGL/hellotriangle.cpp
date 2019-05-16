#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>


int main()
{
	//输入顶点
	GLfloat verticles[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint VBO;//定义VBO
	glGenBuffers(1, &VBO);//这句话会对VBO对象初始化，之前不可以使用VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO对象绑定到GL_ARRAY_BUFFER(target name),之后对GL_ARRAY_BUFFER执行操作均会改变VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticles), verticles, GL_STATIC_DRAW);//将顶点数据复制到内存中
	
	return 0;
}