/*
加载图片为TEXTURE
*/
#include <GL/glew.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

GLuint loadTexture(const std::string filepath)
{
	cv::Mat img = cv::imread(filepath, 1);
	if (img.empty())
	{
		std::cout << "Texture picture is empty" << std::endl;
		throw "Texture picture is empty";
	}
	//debug用
	//cv::imshow(filepath, img);
	cv::Size sz = img.size();
	cv::flip(img, img, 0);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	img.release();
	return texture;
}

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	GLenum glattanchmentType;
	if (!depth && !stencil)
		glattanchmentType = GL_RGB;
	else if (depth && !stencil)
		glattanchmentType = GL_COLOR_COMPONENTS;
	else
		glattanchmentType = GL_STENCIL_INDEX;
	glTexImage2D(GL_TEXTURE_2D, 0, glattanchmentType, 800, 600, 0, glattanchmentType, GL_UNSIGNED_INT_24_8, NULL);
}