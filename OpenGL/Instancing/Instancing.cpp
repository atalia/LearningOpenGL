#pragma comment(lib,"OpenGL32")
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <sstream>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const int VERSION_MINOR = 3;
const int VERSION_MAJOR = 3;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//创建glfw的window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Instancing", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	
	//绑定glfw的window到上下文
	glfwMakeContextCurrent(window);
	
	//注册回调函数
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	//初始化glew
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW INIT FAILURE" << std::endl;
		return -1;
	}
	
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	/*code start here*/

	Shader shader("./Instancing/instance.vert", "./Instancing/instance.frag");

	GLfloat quadVertices[] = {
		//  ---位置---   ------颜色-------
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};
	
	/*方法1 基于OPENGL uniform interface block 以及uniform buffer 、 glbuffersubdata实现		
	======================================================================================================================|
	|				                                                                                                      |
	|																													  |
	|																													  |
	|	GLuint vao, vbo;																								  |
	|	glGenBuffers(1, &vbo);																							  |
	|	glGenVertexArrays(1, &vao);																						  |
	|																													  |
	|	glBindVertexArray(vao);																							  |
	|	glBindBuffer(GL_ARRAY_BUFFER, vbo);																				  |
	|	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);								  |
	|	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));								  |
	|	glEnableVertexAttribArray(0);																					  |
	|	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));			  |
	|	glEnableVertexAttribArray(1);																					  |
	|	glBindBuffer(GL_ARRAY_BUFFER, 0);																				  |
	|	glBindVertexArray(0);																							  |
	|																													  |
	|	glm::vec2 translation[100];																						  |
	|	int index = 0;																									  |
	|	GLfloat offset = 0.1f;																							  |
	|																													  |
	|																													  |
	|	GLuint offsetbindpoint = 0;																						  |
	|	glUniformBlockBinding(shader.Program, glGetUniformBlockIndex(shader.Program, "OFFSET"), offsetbindpoint);		  |
	|																													  |
	|	GLuint ubo;																										  |
	|	glGenBuffers(1, &ubo);																							  |
	|	glBindBuffer(GL_UNIFORM_BUFFER, ubo);																			  |
	|	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(translation), NULL, GL_STATIC_DRAW);									  |
	|	//glBufferData(GL_UNIFORM_BUFFER, sizeof(translation), translation, GL_STATIC_DRAW);							  |
	|																													  |
	|																													  |
	|	for (int y = -10; y < 10; y += 2)																				  |
	|	{																												  |
	|		for (int x = -10; x < 10; x += 2)																			  |
	|		{																											  |
	|			translation[index] = glm::vec2(x/10.0f + offset, y/10.0f + offset);										  |
	|			//std::cout << x / 10.0f + offset << ","<< y/10.0f + offset << std::endl;								  |
	|			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::vec4), sizeof(glm::vec2), translation + index);	  |
	|			index += 1;																								  |
	|		}																											  |
	|	}																												  |
	|																													  |
	|	glBindBuffer(GL_UNIFORM_BUFFER, 0);																				  |
	|																													  |
	|	glBindBufferBase(GL_UNIFORM_BUFFER, offsetbindpoint, ubo);														  |
	|																												      |
	|=====================================================================================================================|
	
	*/
	//方法2. 直接申明uniform数组，然后定位到对应的offset，opengl获取glsl中具体index的数组变量，只能一个一个获取，无法通过偏移得到！
	/*
	GLuint vao, vbo;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);																							  
	glBindBuffer(GL_ARRAY_BUFFER, vbo);																				  
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);								  
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));								  
	glEnableVertexAttribArray(0);																					  
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));			  
	glEnableVertexAttribArray(1);																					  
	glBindBuffer(GL_ARRAY_BUFFER, 0);																				  
	glBindVertexArray(0);

	glm::vec2 translation[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)																				  
	{																											
		for (int x = -10; x < 10; x += 2)																		
		{																										
			translation[index++] = glm::vec2(x / 10.0f + offset, y / 10.0f + offset);																															
		}																										
	}


	shader.Use();
	for (GLuint i = 0; i < 100; ++i)
	{
		std::stringstream ss;
		ss << "offsets[" << i << "]";
		//std::cout << ss.str().c_str() << std::endl;
		glUniform2f(glGetUniformLocation(shader.Program, ss.str().c_str()), translation[i].x, translation[i].y);
	}
	*/
	glm::vec2 translation[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			translation[index++] = glm::vec2(x / 10.0f + offset, y / 10.0f + offset);
		}
	}
	GLuint vao, vbo, transvbo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &transvbo);
	glGenVertexArrays(1, &vao);


	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, transvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(translation), translation, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)(0));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{
		// 轮询检查
		glfwPollEvents();
		// Render Start
		// 设置清屏所用的颜色
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f); //状态设置函数
		// 清空颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT); //状态应用函数

		shader.Use();
		glBindVertexArray(vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		glBindVertexArray(0);
		// 交换缓冲
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glfwTerminate();
	return 0;
}