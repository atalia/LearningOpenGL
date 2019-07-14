/*
加载图片为TEXTURE
*/
#include <GL/glew.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

GLuint loadTexture(const std::string filepath, cv::ImreadModes img_mode = cv::ImreadModes::IMREAD_COLOR)
{
	cv::Mat img = cv::imread(filepath, img_mode);
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

	if (img.channels() == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.width, sz.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.ptr());
	}
	else if (img.channels() == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
	else
		throw "Check picuture channel";

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, img.channels() == 4 ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, img.channels() == 4 ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	img.release();
	return texture;
}

