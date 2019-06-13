#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch):cameraFront(glm::vec3(0.0f,0.0f,1.0f)),movementSpeed(SPEED),mouseSensitivity(SENSITIVITY),zoom(ZOOM_MAX)
{
	this->cameraPos = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	
	this->updateCameraVectors();
}
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :cameraFront(glm::vec3(0.0f, 0.0f, 1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM_MAX)
{
	this->cameraPos = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;

	this->updateCameraVectors();
}

void Camera::setZoom(GLfloat zoom)
{
	this->zoom = zoom;
}
GLfloat Camera::getZoom()
{
	return glm::radians(this->zoom);
}
void Camera::setMovementSpeed(GLfloat speed)
{
	this->movementSpeed = speed;
}

void Camera::setMouseSensitivity(GLfloat sensitivity)
{
	this->mouseSensitivity = sensitivity;
}
Camera::~Camera()
{
	
}


void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(this->yaw)) * glm::sin(glm::radians(this->pitch));
	front.y = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
	front.z = glm::sin(glm::radians(this->yaw));
	this->cameraFront = glm::normalize(front);
	this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->worldUp));
	this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));
}



glm::mat4  Camera::getView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->movementSpeed * deltaTime;
	if (direction == FORWARD)
		this->cameraPos += this->cameraFront * velocity;
	else if (direction == BACKWARD)
		this->cameraPos -= this->cameraFront * velocity;
	else if (direction == LEFT)
		this->cameraPos -= this->cameraRight * velocity;
	else if (direction == RIGHT)
		this->cameraPos += this->cameraRight * velocity;
}


void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= this->mouseSensitivity;
	yoffset *= this->mouseSensitivity;
	this->yaw += xoffset;
	this->pitch += yoffset;

	if(constrainPitch)
	{
		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		else if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}
	this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yoffset)
{
	if (this->zoom < ZOOM_MIN)
		this->zoom = ZOOM_MIN;
	else if (this->zoom > ZOOM_MAX)
		this->zoom = ZOOM_MAX;
	else
		this->zoom -= yoffset;
}