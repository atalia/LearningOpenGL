#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>


int main()
{
	//���붥��
	GLfloat verticles[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint VBO;//����VBO
	glGenBuffers(1, &VBO);//��仰���VBO�����ʼ����֮ǰ������ʹ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO����󶨵�GL_ARRAY_BUFFER(target name),֮���GL_ARRAY_BUFFERִ�в�������ı�VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticles), verticles, GL_STATIC_DRAW);//���������ݸ��Ƶ��ڴ���
	
	return 0;
}