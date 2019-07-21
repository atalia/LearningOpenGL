/*
加载图片为TEXTURE
*/
#include <GL/glew.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>

GLuint load2DTexture(const std::string filepath)
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

GLuint loadCubeTexture(const std::vector<std::string> filepaths)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (int i = 0; i < filepaths.size(); ++i)
	{
		cv::Mat img = cv::imread(filepaths[i]);
		if (img.empty())
		{
			std::cout <<"ERROR IN OPEN FILE" <<filepaths[i] << std::endl;
			throw "Texture is empty";
		}
		cv::Size sz = img.size();
		cv::flip(img, img, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, sz.width, sz.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;
}