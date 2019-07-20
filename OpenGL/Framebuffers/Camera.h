#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//相机初始化值
const GLfloat PITCH = 0.0f;
const GLfloat YAW = -90.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM_MAX = 60.0f;
const GLfloat ZOOM_MIN = 0.1f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;
	glm::vec3 cameraRight;
	glm::vec3 cameraFront;
	
	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;
	void updateCameraVectors();

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	~Camera();
	
	void setMovementSpeed(GLfloat);
	void setMouseSensitivity(GLfloat);
	void setZoom(GLfloat);
	GLfloat getZoom();

	glm::mat4 getView();

	glm::vec3 getCameraPos();

	glm::vec3 getCameraFront();
	void processKeyboard(Camera_Movement , GLfloat);
	void processMouseMovement(GLfloat, GLfloat, GLboolean constrainPitch = true);
	void processMouseScroll(GLfloat);

	GLfloat getYaw() const;

	void setYaw(GLfloat yaw);

	GLfloat getPitch() const;
	void setPitch(GLfloat pitch);
};