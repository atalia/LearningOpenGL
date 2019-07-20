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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	img.release();
	return texture;
}

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, const int& width, const int& height)
{
	GLenum glattanchmentType;
	if (!depth && !stencil)
		glattanchmentType = GL_RGB;
	else if (depth && !stencil)
		glattanchmentType = GL_COLOR_COMPONENTS;
	else
		glattanchmentType = GL_STENCIL_INDEX;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glTexImage2D(target, level, internalFormat, width, height, boarder, format, type, data);
	//internamFormat: Speicifies the number of color components in the texture;
	//format: Specifies the format of pixel data
	//type: Specifies the date type of pixel data

	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, glattanchmentType, width, height, 0, glattanchmentType, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}