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
	cv::Size sz = img.size();
	cv::flip(img, img, 0);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
	img.release();
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}