#pragma once
#ifndef SHADER_H
#define SHADER_H
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include<GL/glew.h>
class Shader
{
	private:
		std::string vert;
		std::string frag;
		std::string geo;
	public:
		GLuint Program;
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
		std::string vertSource();
		std::string fragSource();
		std::string geoSource();
    ~Shader();
	void Use();
};
#endif // !SHADER_H
