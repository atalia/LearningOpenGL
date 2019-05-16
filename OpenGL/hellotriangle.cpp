#pragma comment(lib, "OpenGL32")
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

const GLchar *vertexaShaderSource = "#version 330 core\n layout(location=0) in position;\n void main(){gl_Postion = vec4(positon.x, position.y, positino.z, 1.0f);}";

const GLchar *fragmentShaderSource = "#version 330 core\n out vec4 color\;\n void main(){color = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

int main()
{
	//��ʼ��GL
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Fail to init glew" << std::endl;
		return -1;
	}

	//���붥��
	GLfloat verticles[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint VBO;//����VBO
	glGenBuffers(1, &VBO);//��仰���VBO�����ʼ�����ڴ�֮ǰ������ʹ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO����󶨵�GL_ARRAY_BUFFER(target name),֮���GL_ARRAY_BUFFERִ�в�������ı�VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticles), verticles, GL_STATIC_DRAW);//���������ݸ��Ƶ��ڴ���
	
	//��������ɫ��
	GLuint vertexShader;//������ɫ������
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//��ʼ������
	glShaderSource(vertexShader,1 , &vertexaShaderSource, NULL);//��ɫ����ShaderԴ��
	glCompileShader(vertexShader);//������ɫ��
	std::cout << "VBO: " << VBO << "vertexShader: " << vertexShader;
	//��� ������ɫ������״̬
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	//����Ƭ����ɫ��
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::Fragment::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	//������ɫ������
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//�����ɫ������״̬
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::LINK::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}
	//������ɫ������
	glUseProgram(shaderProgram);
	//ɾ��Ƭ����ɫ������ + ������ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	return 0;
}