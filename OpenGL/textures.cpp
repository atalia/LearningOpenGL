#pragma comment(lib,"OpenGL32.lib")
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/highgui/highgui.hpp>
#include "Hello Texture/shader.h"

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
	//����glfw��window
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
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	Shader shader("E:\\LearningOpenGL\\OpenGL\\Hello Texture\\shader.vert", "E:\\LearningOpenGL\\OpenGL\\Hello Texture\\shader.frag");


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

	// ����������󲢰�ID
	GLuint texture0;
	GLuint texture1;
	glGenTextures(1, &texture0);
	// �����������֮����glBindTexture(GL_TEXTURE_2D, 0)֮ǰ��ָ�����õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, texture0);

	//���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//�����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��ͼƬ
	cv::Mat image = cv::imread("E:\\LearningOpenGL\\OpenGL\\Hello Texture\\wall.jpg", CV_LOAD_IMAGE_UNCHANGED);
	if (image.empty())
	{
		std::cout << "Texture1 picture is empty" << std::endl;
		system("pause");
		return -1;
	}

	cv::Size sz = image.size();
	
	cv::flip(image, image, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());

	//���ɶ༶��Զ����
	glGenerateMipmap(GL_TEXTURE_2D);

	image.release();
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture1);
	// �����������֮����glBindTexture(GL_TEXTURE_2D, 0)֮ǰ��ָ�����õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, texture1);

	//���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//�����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��ͼƬ
	image = cv::imread("E:\\LearningOpenGL\\OpenGL\\Hello Texture\\container.jpg", CV_LOAD_IMAGE_UNCHANGED);
	if (image.empty())
	{
		std::cout << "Texture2 picture is empty" << std::endl;
		system("pause");
		return -1;
	}
	sz = image.size();

	cv::flip(image, image, 0);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());

	//���ɶ༶��Զ����
	glGenerateMipmap(GL_TEXTURE_2D);

	image.release();
	glBindTexture(GL_TEXTURE_2D, 0);

	
	while (!glfwWindowShouldClose(window))
	{
		// ��ѯ���
		glfwPollEvents();
		// Render Start
		// �����������õ���ɫ
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f); //״̬���ú���
		// �����ɫ����
		glClear(GL_COLOR_BUFFER_BIT); //״̬Ӧ�ú���
		
		shader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glUniform1i(glGetUniformLocation(shader.Program, "Texture_1"), 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader.Program, "Texture_2"), 1);

		
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		// ��������
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}