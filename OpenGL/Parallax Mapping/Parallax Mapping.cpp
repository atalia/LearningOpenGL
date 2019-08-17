#pragma comment(lib, "openGL32.lib")
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include "Camera.h"
#include "shader.h"

extern GLuint load2DTexture(const std::string filepath);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

const int VERSION_MINOR = 3;
const int VERSION_MAJOR = 3;

bool keys[1024];

bool firstMouse = true;
GLfloat lastX = 400;
GLfloat lastY = 300;
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));//全局唯一的相机

const glm::vec3 LIGHTPOSTION(0.5f, 1.0f, 0.3f);//全局唯一灯
const glm::vec3 LIGHTCOLOR(1.0f, 1.0f, 1.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_B)
	{
		if (action == GLFW_PRESS)
		{
			if (keys[key])
			{
				keys[key] = false;
			}
			else
			{
				keys[key] = true;
			}
		}
	}
	else if (key > 0 && key < 1024)
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

void renderQuad();

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//MULTISAMPLES
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Normal Map in WORLD", NULL, NULL);
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

	//Shader shader("./Normal Mapping/worldshader.vert","./Normal Mapping/worldshader.frag");
	Shader shader("./Normal Mapping/tbnshader.vert", "./Normal Mapping/tbnshader.frag");
	GLuint diffuseTexture = load2DTexture("./Normal Mapping/brickwall.jpg");
	GLuint normalTexture = load2DTexture("./Normal Mapping/brickwall_normal.jpg");
	shader.Use();
	glUniform3f(glGetUniformLocation(shader.Program, "lightPos"), LIGHTPOSTION.x, LIGHTPOSTION.y, LIGHTPOSTION.z);
	glUniform3f(glGetUniformLocation(shader.Program, "lightColor"), LIGHTCOLOR.x, LIGHTCOLOR.y, LIGHTCOLOR.z);

	GLfloat lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		GLfloat currentTime = glfwGetTime();
		GLfloat deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		move(camera, deltaTime);
		
		glm::mat4 model(1.0f);
		model = glm::rotate(model, sin(currentTime), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);

		shader.Use();
		glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, glm::value_ptr(camera.getCameraPos()));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.getView()));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
		glUniform1i(glGetUniformLocation(shader.Program, "diffuseTexture"), 0);

		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalTexture);
		glUniform1i(glGetUniformLocation(shader.Program, "normalTexture"), 1);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		renderQuad();
		glfwSwapBuffers(window);

	}
	glfwTerminate();
	return 0;
}


//将纹理空间坐标转到局部坐标 -> TBN
// TBN * model
void renderQuad()
{
	static GLuint quadVAO;
	if (quadVAO == 0)
	{
		glm::vec3 pos1(-1.0, 1.0, 0.0);
		glm::vec3 pos2(-1.0, -1.0, 0.0);
		glm::vec3 pos3(1.0, -1.0, 0.0);
		glm::vec3 pos4(1.0, 1.0, 0.0);
		// texture coordinates
		glm::vec2 uv1(0.0, 1.0);
		glm::vec2 uv2(0.0, 0.0);
		glm::vec2 uv3(1.0, 0.0);
		glm::vec2 uv4(1.0, 1.0);

		glm::vec3 deltaPos1 = pos1 - pos2;
		glm::vec3 deltaPos2 = pos3 - pos2;

		glm::vec2 deltauv1 = uv1 - uv2;
		glm::vec2 deltauv2 = uv3 - uv2;

		glm::vec3 tangent(0.0f);
		glm::vec3 bitangent(0.0f);

		float f = 1.0f / (deltauv1.x*deltauv2.y - deltauv1.y*deltauv2.x);

		tangent.x = f * (deltauv2.y * deltaPos1.x - deltauv1.y * deltaPos2.x);
		tangent.y = f * (deltauv2.y * deltaPos1.y - deltauv1.y * deltaPos2.y);
		tangent.z = f * (deltauv2.y * deltaPos1.z - deltauv1.y * deltaPos2.z);
		tangent = glm::normalize(tangent);

		bitangent.x = f * (-deltauv2.x * deltaPos1.x + deltauv1.x * deltaPos2.x);
		bitangent.y = f * (-deltauv2.x * deltaPos1.y + deltauv1.x * deltaPos2.y);
		bitangent.z = f * (-deltauv2.x * deltaPos1.z + deltauv1.x * deltaPos2.z);
		bitangent = glm::normalize(bitangent);
		
		glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));

		GLfloat quad[] = {
		pos1.x, pos1.y, pos1.z, uv1.x, uv1.y,
		pos2.x, pos2.y, pos2.z, uv2.x, uv2.y,
		pos3.x, pos3.y, pos3.z, uv3.x, uv3.y,
		pos1.x, pos1.y, pos1.z, uv1.x, uv1.y,
		pos3.x, pos3.y, pos3.z, uv3.x, uv3.y,
		pos4.x, pos4.y, pos4.z, uv4.x, uv4.y,
		};

		GLfloat TBN[] =
		{
			//      1,         0,         0,           0,           1,           0,        0,        0,        1,
			tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
			tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
			tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
			tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
			tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
			tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
		};

		//std::cout << tangent.x << tangent.y << tangent.z << std::endl;
		//std::cout << bitangent.x << bitangent.y << bitangent.z << std::endl;
		//std::cout << normal.x << normal.y << normal.z << std::endl;

		GLuint quadVBO[2];
		glGenBuffers(2, quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TBN), TBN, GL_STATIC_DRAW);

		glGenVertexArrays(1, &quadVAO);
		
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO[1]);
		
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(4);
		
		glBindVertexArray(0);
	}
	glBindVertexArray(quadVAO);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(0);
}

