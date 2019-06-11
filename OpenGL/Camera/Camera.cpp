#include "Camera.h"

Camera::Camera(glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp)
{
	cameraPos = camPos;
	cameraFront = camFront;
	targetPos = camFront + camPos;
	cameraUp = camUp;
}
Camera::~Camera()
{
	
}

void Camera::up(GLfloat speed)
{
	cameraPos += speed * cameraFront;
}
void Camera::down(GLfloat speed)
{
	cameraPos -= speed * cameraFront;
}

void Camera::left(GLfloat speed)
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

void Camera::right(GLfloat speed)
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

glm::mat4  Camera::view()
{
	glm::mat4 viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return viewMat;
}
