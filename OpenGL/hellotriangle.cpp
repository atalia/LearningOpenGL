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
	//首先需要制定glversion
	//初始化GL
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

	//输入顶点
	GLfloat verticles[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint VBO;//定义VBO
	glGenBuffers(1, &VBO);//这句话会对VBO对象初始化，在此之前不可以使用VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO对象绑定到GL_ARRAY_BUFFER(target name),之后对GL_ARRAY_BUFFER执行操作均会改变VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticles), verticles, GL_STATIC_DRAW);//将顶点数据复制到内存中
	std::cout << "VBO: " << VBO << std::endl;
	
	//处理顶点着色器
	GLuint vertexShader;//定义着色器对象
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//初始化对象
	glShaderSource(vertexShader,1 , &vertexaShaderSource, NULL);//着色器绑定Shader源码
	glCompileShader(vertexShader);//编译着色器
	std::cout << "vertexShader: " << vertexShader << std::endl;
	//检查 顶点着色器编译状态
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

	//处理片段着色器
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
	//处理着色器程序
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//检查着色器程序状态
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::LINK::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		system("pause");
		return -1;
	}
	std::cout << "shaderProgram: " << shaderProgram << std::endl;
	//激活着色器程序对象
	glUseProgram(shaderProgram);
	//删除片段着色器对象 + 顶点着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//至此，我们的数据已经传输给了着色器

	//主循环
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.6f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	return 0;
}