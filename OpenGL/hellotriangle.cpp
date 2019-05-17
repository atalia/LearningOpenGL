#pragma comment(lib, "OpenGL32.lib")
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

const int GL_VERSION_MAJOR = 3;
const int GL_VERSION_MINOR = 3;
const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 600;

const GLchar *vertexaShaderSource = "#version 330 core\n"
	"layout(location=0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0f);}\0";
const GLchar *fragmentShaderSource = "#version 330 core\n out vec4 color\;\n void main(){color = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

GLFWwindow* create_window()
{
	if (!glfwInit())
	{
		std::cout << "Failed to init glfw : "<<std::endl;
		system("pause");
		return NULL;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "hello triangle", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		std::cout << "Failed to create GLFW window" << std::endl;
		return window;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	return window;
}
int main()
{
	//������Ҫ�ƶ�glversion
	//��ʼ��GL
	GLFWwindow* window = create_window();
	if (window == NULL)
	{
		return -1;
	}
	glewExperimental = GL_TRUE;
	GLenum glInitStatus = glewInit();
	if (glInitStatus != GLEW_OK)
	{
		std::cout << "Fail to init glew : " << glewGetErrorString(glInitStatus)<<std::endl;
		system("pause");
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
	std::cout << "VBO: " << VBO << std::endl;
	
	//��������ɫ��
	GLuint vertexShader;//������ɫ������
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//��ʼ������
	glShaderSource(vertexShader,1 , &vertexaShaderSource, NULL);//��ɫ����ShaderԴ��
	glCompileShader(vertexShader);//������ɫ��
	std::cout << "vertexShader: " << vertexShader << std::endl;
	//��� ������ɫ������״̬
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		system("pause");
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
		system("pause");
		return -1;
	}
	std::cout << "fragmentShader: " << fragmentShader<<std::endl;
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
		system("pause");
		return -1;
	}
	std::cout << "shaderProgram: " << shaderProgram << std::endl;
	//������ɫ���������
	glUseProgram(shaderProgram);
	//ɾ��Ƭ����ɫ������ + ������ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//���ˣ����ǵ������Ѿ����������ɫ��

	//��ѭ��
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.6f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	return 0;
}