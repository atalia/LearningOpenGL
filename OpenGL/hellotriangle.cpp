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
const GLchar *fragmentShaderSource = "#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

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

	//��������ɫ��
	GLuint vertexShader;//������ɫ������
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//��ʼ������
	glShaderSource(vertexShader, 1, &vertexaShaderSource, NULL);//��ɫ����ShaderԴ��
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
	std::cout << "fragmentShader: " << fragmentShader << std::endl;
	//������ɫ������
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//�����ɫ������״̬
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
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
	//��ɫ�����ָ㶨

	//���붥��
	GLfloat verticles[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint VBO;//����VBO
	GLuint VAO; //���嶥��������󣬴洢�������Ե���
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);//��仰���VBO�����ʼ�����ڴ�֮ǰ������ʹ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO����󶨵�GL_ARRAY_BUFFER(target name),֮���GL_ARRAY_BUFFERִ�в�������ı�VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticles), verticles, GL_STATIC_DRAW);//���������ݸ��Ƶ��ڴ���
	
	//������ɫ����������ָ���κ��Զ�������Ϊ��ʽ�����롣
	//��ʹ����к�ǿ������Ե�ͬʱ��������ȷ��ζ�����Ǳ����ֶ�ָ���������ݵ���һ�����ֶ�Ӧ������ɫ������һ���������ԡ�
	//���ԣ����Ǳ�������Ⱦǰָ��OpenGL����ν��Ͷ������ݡ�
	//��ʼ���Ͷ������� ���Ӷ�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// ��һ������ -> �ڶ���shaderԴ���У�������layout(location = 0)�����˶������Ե�λ��ֵ ��Ӧ�Ĵ�����ַΪ0���������Ӧ
	// �ڶ��������� ÿһ��������������������Ӧvec3,��3��ֵ
	// ���������������ݵ�����
	// ���ĸ��������Ƿ���Ҫ��׼��
	// ��������������嶥����������֮ǰ����buffer�еľ���
	// ������������λ��������buffer����ʼλ�õ�ƫ������������Ҫ��һ��ǿ������ת���������ת��ΪVoid* ����
	glEnableVertexAttribArray(0);//��������Ĭ���ǽ�ֹ�ģ�����ָ����������Ϊλ��ֵ0�Ķ���������Ч

	//��ɽ��Ͷ�������
	std::cout << "VAO:" << VAO << " VBO: " << VBO << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	

	
	
	

	//��ѭ��
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.6f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}