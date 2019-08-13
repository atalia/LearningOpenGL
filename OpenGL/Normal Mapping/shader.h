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
	public:
		GLuint Program;
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		std::string vertSource();
		std::string fragSource();
    ~Shader();
	void Use();
};
#endif // !SHADER_H
