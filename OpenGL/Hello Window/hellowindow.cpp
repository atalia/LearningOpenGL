#pragma comment(lib,"OpenGL32")
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "tutoral 1", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	
	//��glfw��window��������
	glfwMakeContextCurrent(window);
	
	//ע��ص�����
	glfwSetKeyCallback(window, key_callback);

	//glewExperimental = GL_TRUE;
	//��ʼ��glew
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW INIT FAILURE" << std::endl;
		return -1;
	}
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
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
		// ��������
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}