#pragma comment(lib, "OpenGL32")
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

const GLchar *vertexaShaderSource = "#version 330 core\n layout(location=0) in position;\n void main(){gl_Postion = vec4(positon.x, position.y, positino.z, 1.0f);}";

const GLchar *fragmentShaderSource = "#version 330 core\n out vec4 color\;\n void main(){color = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

int main()
{
	//初始化GL
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Fail to init glew" << std::endl;
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
	
	//处理顶点着色器
	GLuint vertexShader;//定义着色器对象
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//初始化对象
	glShaderSource(vertexShader,1 , &vertexaShaderSource, NULL);//着色器绑定Shader源码
	glCompileShader(vertexShader);//编译着色器
	std::cout << "VBO: " << VBO << "vertexShader: " << vertexShader;
	//检查 顶点着色器编译状态
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
		return -1;
	}

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
		return -1;
	}
	//激活着色器对象
	glUseProgram(shaderProgram);
	//删除片段着色器对象 + 顶点着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	return 0;
}