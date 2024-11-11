#include "Shader.h"

#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include<fstream>
#include<iostream>

#include"Macros.h"

GraphicsEngine::Shader::Shader(const std::string& vertexFilePath, const std::string& fragFilePath)
{
	ShaderProgramSource src = ParseShader(vertexFilePath, fragFilePath);
	this->program = CreateShader(src.VertexShader, src.FragmentShader);
}

void GraphicsEngine::Shader::Use()
{
	glCheck(glUseProgram(program));
}

void GraphicsEngine::Shader::SetUniformMat4(const glm::mat4& x, const std::string& name)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(x));
}

void GraphicsEngine::Shader::SetUniformVec4(const glm::vec4& x, const std::string& name)
{
	glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(x));
}

void GraphicsEngine::Shader::SetUniformVec2(const glm::vec2& x, const std::string& name)
{
	glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(x));
}

void GraphicsEngine::Shader::SetUniformInt(const int& x, const std::string& name)
{
	glCheck(glUniform1i(glGetUniformLocation(program, name.c_str()), x));
}

GraphicsEngine::ShaderProgramSource GraphicsEngine::Shader::ParseShader(const std::string& vertexFilePath, const std::string& fragFilePath)
{
	std::string vertSrc;
	std::string fragSrc;

	std::fstream fin;

	//open vertex file and read src code
	fin.open(vertexFilePath);
	if (!fin.is_open()) {
		std::cout << "Warning: Vertex Shader File Could Not Be Open!\n";
	}
	else {

		while (!fin.eof())
		{
			std::string line;
			std::getline(fin, line);
			vertSrc += line + '\n';
		}
	}

	fin.close();

	//do the same for frag
	fin.open(fragFilePath);
	if (!fin.is_open()) {
		std::cout << "Warning: Fragment Shader Could Not Be Open!\n";
	}
	else {

		while (!fin.eof())
		{
			std::string line;
			std::getline(fin, line);
			fragSrc += line + '\n';
		}
	}

	fin.close();


	return ShaderProgramSource(vertSrc, fragSrc);
}

int GraphicsEngine::Shader::CompileShader(unsigned int type, const std::string& src)
{
	//create and compile shader in gl
	  //std::cout << src << "\n\n";
	const char* source = src.c_str();
	unsigned int id = glCreateShader(type);
	glCheck(glShaderSource(id, 1, &source, NULL));
	glCheck(glCompileShader(id));

	//error checking
	int x;
	char info[512];
	glCheck(glGetShaderiv(id, GL_COMPILE_STATUS, &x));
	if (!x) {
		glCheck(glGetProgramInfoLog(id, 512, NULL, info));
		std::cout << "\n\nShader Compilation Error: " << info << "\n";
	}

	return id;
}

unsigned int GraphicsEngine::Shader::CreateShader(const std::string& vertShader, const std::string& fragShader)
{
	unsigned int tempProgram = glCreateProgram();

	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertShader);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragShader);

	//link, validate, and merge the created shaders to the program
	glCheck(glAttachShader(tempProgram, vertexShader));
	glCheck(glAttachShader(tempProgram, fragmentShader));
	glCheck(glLinkProgram(tempProgram));

	//error checking
	int x;
	char info[512];
	glGetProgramiv(tempProgram, GL_LINK_STATUS, &x);
	if (!x) {
		glGetProgramInfoLog(tempProgram, 512, NULL, info);
		std::cout << "\n\nShader Attach Error: " << info << "\n";
	}

	glCheck(glValidateProgram(tempProgram));
	
	//shader is created can clear memory
	glCheck(glDeleteShader(vertexShader));
	glCheck(glDeleteShader(fragmentShader));

	return tempProgram;
}


