#pragma once
#include "Camera.h"

static bool keys[1024];

static bool firstMouse = true;
static GLfloat lastX = 400;
static GLfloat lastY = 300;

static Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));//全局唯一的相机


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_callback(GLFWwindow* window, double posX, double posY);

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

void move(Camera& camera, GLfloat& deltaTime);