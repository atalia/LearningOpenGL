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
#include <sstream>
#include <string>


const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const int VERSION_MINOR = 3;
const int VERSION_MAJOR = 3;
const int NR_POINT_LIGHTS = 4;

glm::vec3 lightPosOri(1.2f, 1.0f, 2.0f); //加一个灯的位置，为了区分物体和光源，将光源的shader和物体的shader区分开

bool keys[1024];

bool firstMouse = true;
GLfloat lastX = 400;
GLfloat lastY = 300;

Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));


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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light Casters", NULL, NULL);
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
	

	Shader lightShader =   Shader("./Light casters/lamp.vert", "./Light casters/lamp.frag");
	Shader containShader = Shader("./Light casters/shader.vert", "./Light casters/shader.frag");

	//std::cout << lightShader.vertSource() << std::endl;
	//std::cout << lightShader.fragSource() << std::endl;

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

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPosition[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)

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

	cv::Mat image = cv::imread("./Light casters/container_diffuse.png", 1);//0 为灰度加载，1为BGR加载
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

	image = cv::imread("./Light casters/container_specular.png", 1);
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
		//glm::vec3 lightPos = glm::vec3(lightPosOri.x + deltaPos, lightPosOri.y + deltaPos, lightPosOri.z);
		glm::vec3 lightPos = glm::vec3(lightPosOri.x, lightPosOri.y , lightPosOri.z);

		//std::cout << lightPos.x <<", " <<lightPos.y << ", " <<lightPos.z << std::endl;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		containShader.Use();
		
		//不需要改

		
		//定向光
		glUniform3f(glGetUniformLocation(containShader.Program, "directlight.direction"), -0.2f, -1.0f, -0.3f);
		
		glUniform3f(glGetUniformLocation(containShader.Program, "directlight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(containShader.Program, "directlight.diffuse"), 0.4f, 0.4f, 0.4f) ;
		glUniform3f(glGetUniformLocation(containShader.Program, "directlight.specular"), 0.5f, 0.5f, 0.5f);
		
		
		//点光源
		for (int i = 0; i < NR_POINT_LIGHTS; ++i)
		{
			std::stringstream ss;
			ss << "pointlights[";
			ss << i;
			ss << "]";
			std::string pointlight = ss.str();
			//std::cout << pointlight << std::endl;
			glUniform3f(glGetUniformLocation(containShader.Program, (pointlight + std::string(".position")).c_str()), pointLightPosition[i].x, pointLightPosition[i].y, pointLightPosition[i].z);

			glUniform3f(glGetUniformLocation(containShader.Program, (pointlight + std::string(".ambient")).c_str()), 0.05f, 0.05f, 0.05f);
			glUniform3f(glGetUniformLocation(containShader.Program, (pointlight + std::string(".diffuse")).c_str()), 0.8f, 0.8f, 0.8f);
			glUniform3f(glGetUniformLocation(containShader.Program, (pointlight + std::string(".specular")).c_str()), 1.0f, 1.0f, 1.0f);

			glUniform1f(glGetUniformLocation(containShader.Program, (pointlight + std::string(".constant")).c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(containShader.Program, (pointlight + std::string(".linear")).c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(containShader.Program, (pointlight + std::string(".quadratic")).c_str()), 0.032f);
		}

		
		
		glUniform3f(glGetUniformLocation(containShader.Program, "spotlight.position"), camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);

		glUniform3f(glGetUniformLocation(containShader.Program, "spotlight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(containShader.Program, "spotlight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(containShader.Program, "spotlight.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(containShader.Program, "spotlight.innerCutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(containShader.Program, "spotlight.outCutOff"), glm::cos(glm::radians(30.0f)));
		glUniform3f(glGetUniformLocation(containShader.Program, "spotlight.direction"), camera.getCameraFront().x, camera.getCameraFront().y, camera.getCameraFront().z);

		glUniform1f(glGetUniformLocation(containShader.Program, "spotlight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(containShader.Program, "spotlight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(containShader.Program, "spotlight.quadratic"), 0.032f);

		//std::cout << camera.getCameraFront().x << ", "<<camera.getCameraFront().y<<","<<camera.getCameraFront().z << std::endl;
		
		
		//std::cout << camera.getCameraPos().x<<", "<< camera.getCameraPos().y<<", "<< camera.getCameraPos().z << std::endl;
		//材质的光照贴图
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glUniform1i(glGetUniformLocation(containShader.Program, "material.diffuse"), 0);
		glUniform1f(glGetUniformLocation(containShader.Program, "material.shininess"), 32.0f);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
		glUniform1i(glGetUniformLocation(containShader.Program, "material.specular"), 1);
		
		
		//光的属性
		//glm::vec3 lightColor = glm::vec3(sin(currentTime * 2.0f), sin(currentTime * 0.7f), sin(currentTime * 1.3));
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 diffColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffColor * glm::vec3(0.2f);

		//std::cout << (ambientColor.x, ambientColor.y, ambientColor.z) << std::endl;
		//glUniform3f(lightPosLoc, 0.0f, 0.0, -1.0f);


		glUniform3f(glGetUniformLocation(containShader.Program, "viewPos"), camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);

		glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		
		
		
		glUniformMatrix4fv(glGetUniformLocation(containShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.getView()));
		glUniformMatrix4fv(glGetUniformLocation(containShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(containVAO);

		glm::mat4 model = glm::mat4(1.0f);
		for (GLuint i = 0; i < sizeof(cubePositions)/sizeof(cubePositions[0]); ++i)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(containShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
		}

		glBindVertexArray(0);
		lightShader.Use();

		
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.getView()));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		
		glBindVertexArray(lightVAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (GLuint i = 0; i < NR_POINT_LIGHTS; ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPosition[i]);
			model = glm::scale(model, glm::vec3(1.0f));

			glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
	
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &containVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
	return 0;
}