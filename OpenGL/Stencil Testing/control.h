#pragma once
#include "Camera.h"

bool keys[1024];

bool firstMouse = true;
GLfloat lastX = 400;
GLfloat lastY = 300;

static Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));//ȫ��Ψһ�����


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_callback(GLFWwindow* window, double posX, double posY);

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

void move(Camera& camera, GLfloat& deltaTime);