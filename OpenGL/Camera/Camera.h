#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera
{
private:
	glm::vec3 cameraPos;
	glm::vec3 targetPos;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
public:
	Camera(glm::vec3, glm::vec3, glm::vec3);
	~Camera();
	
	void up(GLfloat);
	void down(GLfloat);
	void left(GLfloat);
	void right(GLfloat);

	glm::mat4 view();

};