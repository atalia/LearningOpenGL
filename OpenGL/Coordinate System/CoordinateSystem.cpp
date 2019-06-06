#pragma comment(lib,"OpenGL32.lib")
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <highgui/highgui.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

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

void matrixShow(glm::mat4& matrix)
{
	std::cout << std::string(10, '-') << std::endl;
	for (int i = 0; i < matrix.length(); ++i)
	{
		for (int j = 0; j < matrix[0].length(); ++j)
		{
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::string(10, '-') << std::endl;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//创建glfw的window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Texture", NULL, NULL);

	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "GLEW INIT FAILURE" << std::endl;
		system("pause");
		return -1;
	}
	
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	GLfloat vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	Shader shader("./Coordinate System/shader.vert", "./Coordinate System/shader.frag");
	//Shader shader("./Transformation/shader.vert", "./Transformation/shader.frag");

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// 创建纹理对象并绑定ID
	GLuint texture;
	glGenTextures(1, &texture);
	// 绑定纹理对象，让之后在glBindTexture(GL_TEXTURE_2D, 0)之前的指令配置当前绑定的纹理
	glBindTexture(GL_TEXTURE_2D, texture);
	
	//设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//设置纹理过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//绑定图片
	cv::Mat image = cv::imread("./Transformation/avatar.jpeg", 1);
	if (image.empty())
	{
		std::cout << "Texture picture is empty" <<std::endl;
		system("pause");
		return -1;
	}
	cv::Size sz = image.size();
	
	cv::flip(image, image, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());

	//生成多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);

	image.release();
	glBindTexture(GL_TEXTURE_2D, 0);




	while (!glfwWindowShouldClose(window))
	{
		// 轮询检查
		glfwPollEvents();
		// Render Start
		// 设置清屏所用的颜色
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //状态设置函数
		// 清空颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT); //状态应用函数

		glBindTexture(GL_TEXTURE_2D, texture);

		GLfloat currentTime = (GLfloat)glfwGetTime();

		shader.Use();
		//第一个萝莉头
		glBindVertexArray(VAO);
		
		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));



		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(60.0f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT , 0.01f, 1000.0f);

		GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
		
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glm::mat4 trans(1.0f);

		
	
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

		trans = glm::rotate(trans, glm::radians(currentTime * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		
		GLuint transformLoc = glGetUniformLocation(shader.Program, "transform");

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glm::mat2 texcordTrans(1.0f);

		GLuint textcoordTranformLoc = glGetUniformLocation(shader.Program, "texcoordTransform");
		glUniformMatrix2fv(textcoordTranformLoc, 1, GL_FALSE, glm::value_ptr(texcordTrans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);
		
		
		//第二个萝莉头
		glBindVertexArray(VAO);

		


		trans = glm::mat4(1.0f);

		trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.f));

		trans = glm::rotate(trans, glm::radians(currentTime * 90.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		texcordTrans[0][0] = -1;

		
		glUniformMatrix2fv(textcoordTranformLoc, 1, GL_FALSE, glm::value_ptr(texcordTrans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shader.~Shader();
	glfwTerminate();
	return 0;
}
