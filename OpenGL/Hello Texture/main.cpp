#pragma comment(lib,"OpenGL32.lib")
#include "Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/highgui/highgui.hpp>


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
	if (GLEW_OK != glewInit())
	{
		std::cout << "GLEW INIT FAILURE" << std::endl;
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLfloat vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	GLfloat indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	Shader shader("shader.vert", "shader.vert");


	GLuint VAO, VBO, VEO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &VEO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// ����������󲢰�ID
	GLuint texture;
	glGenTextures(1, &texture);
	// �����������֮����glBindTexture(GL_TEXTURE_2D, 0)֮ǰ��ָ�����õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, texture);
	
	//���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//�����������
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//���ɶ༶��Զ����
	glGenerateMipmap(texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//��ͼƬ
	cv::Mat image = cv::imread("container.jpg", CV_LOAD_IMAGE_UNCHANGED);
	if (image.empty())
	{
		std::cout << "Texture picture is empty" <<std::endl;
		return -1;
	}
	cv::Size sz = image.size();
	
	cv::flip(image, image, 0);

	glTexImage2D(texture, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());

	image.release();
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!glfwWindowShouldClose(window))
	{
		// ��ѯ���
		glfwPollEvents();
		// Render Start
		// �����������õ���ɫ
		glClearColor(0.5f, 0.6f, 0.7f, 0.0f); //״̬���ú���
		// �����ɫ����
		glClear(GL_COLOR_BUFFER_BIT); //״̬Ӧ�ú���
		// Render End
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);
		// ��������
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VEO);

	glfwTerminate();
	return 0;
	return 0;
}