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

	Shader shader("./Transformation/shader.vert", "./Transformation/shader.frag");


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
	GLuint texture;
	glGenTextures(1, &texture);
	// �����������֮����glBindTexture(GL_TEXTURE_2D, 0)֮ǰ��ָ�����õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, texture);
	
	//���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//�����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//��ͼƬ
	cv::Mat image = cv::imread("./Transformation/container.jpg", 1);
	if (image.empty())
	{
		std::cout << "Texture picture is empty" <<std::endl;
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


	while (!glfwWindowShouldClose(window))
	{
		// ��ѯ���
		glfwPollEvents();
		// Render Start
		// �����������õ���ɫ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //״̬���ú���
		// �����ɫ����
		glClear(GL_COLOR_BUFFER_BIT); //״̬Ӧ�ú���

		glBindTexture(GL_TEXTURE_2D, texture);

		shader.Use();
		glBindVertexArray(VAO);

		glm::mat4 trans(1.0f);

		GLfloat currentTime = (GLfloat)glfwGetTime();
	
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.f));

		trans = glm::rotate(trans, glm::radians(currentTime * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		GLuint transformLoc = glGetUniformLocation(shader.Program, "transform");

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


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