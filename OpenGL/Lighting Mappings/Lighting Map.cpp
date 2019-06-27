#pragma comment(lib, "openGL32.lib")
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"
#include "shader.h"
#include <opencv2/highgui/highgui.hpp>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const int VERSION_MINOR = 3;
const int VERSION_MAJOR = 3;

glm::vec3 lightPosOri(1.2f, 1.0f, 2.0f); //加一个灯的位置，为了区分物体和光源，将光源的shader和物体的shader区分开

bool keys[1024];

bool firstMouse = true;
GLfloat lastX = 400;
GLfloat lastY = 300;

Camera camera(glm::vec3(1.0f, 1.0f, 4.0f));


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key > 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void mouse_callback(GLFWwindow* window, double posX, double posY)
{
	//std::cout << posX << "," <<posY << std::endl;
	if (firstMouse)
	{
		lastX = posX;
		lastY = posY;
		firstMouse = false;
	}
	GLfloat xoffset = posX - lastX;
	GLfloat yoffset = posY - lastY;
	lastX = posX;
	lastY = posY;
	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

void move(Camera& camera, GLfloat& deltaTime)
{
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);
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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lighting Maps", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);
	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW INIT FAILURE" << std::endl;
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	

	Shader lightShader = Shader("./Lighting Mappings/lamp.vert", "./Lighting Mappings/lamp.frag");
	Shader containShader = Shader("./Lighting Mappings/shader.vert", "./Lighting Mappings/shader.frag");

	GLfloat vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//CUBE
	GLuint VBO, containVAO;
	glGenVertexArrays(1, &containVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(containVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//LIGHT
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Content End
	
	GLuint diffuse;
	glGenTextures(1, &diffuse);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	cv::Mat image = cv::imread("./Lighting Mappings/container_diffuse.png", 1);//0 为灰度加载，1为BGR加载
	if (image.empty())
	{
		std::cout << "Texture picture is empty" << std::endl;
		system("pause");
		return -1;
	}
	cv::Size sz = image.size();
	cv::flip(image, image, 0);
	//cv::imshow("image", image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());
	//std::cout << sz.height << sz.height << std::endl;
	image.release();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	GLuint specular;
	glGenTextures(1, &specular);
	glBindTexture(GL_TEXTURE_2D, specular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	image = cv::imread("./Lighting Mappings/container_specular.png", 1);
	if (image.empty())
	{
		std::cout<<"Texture picture is empty" << std::endl;
		system("pause");
		return -1;
	}
	sz = image.size();
	cv::flip(image, image, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());
	image.release();
	glBindTexture(GL_TEXTURE_2D, 0);



	GLfloat lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		GLfloat currentTime = (GLfloat)glfwGetTime();
		GLfloat deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		move(camera, deltaTime);
		float deltaPos = sin(currentTime);
		//std::cout << deltaPos << std::endl;
		glm::vec3 lightPos = glm::vec3(lightPosOri.x + deltaPos, lightPosOri.y + deltaPos, lightPosOri.z);

		//std::cout << lightPos.x <<", " <<lightPos.y << ", " <<lightPos.z << std::endl;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		containShader.Use();
		
		GLuint viewPosLoc = glGetUniformLocation(containShader.Program, "viewPos");
		GLuint matShineLoc = glGetUniformLocation(containShader.Program, "material.shininess");
		GLuint lightPosLoc = glGetUniformLocation(containShader.Program, "light.position");
		GLuint lightAmbientLoc = glGetUniformLocation(containShader.Program, "light.ambient");
		GLuint lightDiffLoc = glGetUniformLocation(containShader.Program, "light.diffuse");
		GLuint lightSpecLoc = glGetUniformLocation(containShader.Program, "light.specular");

		
		//材质的属性
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glUniform1i(glGetUniformLocation(containShader.Program, "material.diffuse"), 0);

		glUniform1f(matShineLoc, 32.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
		glUniform1i(glGetUniformLocation(containShader.Program, "material.specular"), 1);
		//光的属性
		//glm::vec3 lightColor = glm::vec3(sin(currentTime * 2.0f), sin(currentTime * 0.7f), sin(currentTime * 1.3));
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 diffColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffColor * glm::vec3(0.2f);

		//std::cout << (ambientColor.x, ambientColor.y, ambientColor.z) << std::endl;
		glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);//光源环境光
		glUniform3f(lightDiffLoc, diffColor.x, diffColor.y, diffColor.z);//光源漫反射
		glUniform3f(lightSpecLoc, 1.0f, 1.0f, 1.0f);//光源镜面反射
		glUniform3f(viewPosLoc, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);

		glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		
		GLuint modelLoc = glGetUniformLocation(containShader.Program, "model");
		
		GLuint viewLoc = glGetUniformLocation(containShader.Program, "view");
		
		GLuint projectionLoc = glGetUniformLocation(containShader.Program, "projection");
		

		glm::mat4 model = glm::mat4(1.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(containVAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		
		lightShader.Use();

		modelLoc = glGetUniformLocation(lightShader.Program, "model");
		viewLoc = glGetUniformLocation(lightShader.Program, "view");
		projectionLoc = glGetUniformLocation(lightShader.Program, "projection");
		
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.01f));
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(lightVAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &containVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
	return 0;
}