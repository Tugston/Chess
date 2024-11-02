#include "Shader.h"

#include<GL/glew.h>

#include<fstream>
#include<iostream>

GraphicsEngine::Shader::Shader(const std::string& vertexFilePath, const std::string& fragFilePath)
{
	ShaderProgramSource src = ParseShader(vertexFilePath, fragFilePath);
	this->program = CreateShader(src.VertexShader, src.FragmentShader);
}

void GraphicsEngine::Shader::Use()
{
	glUseProgram(program);
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
	std::cout << src << "\n\n";
	const char* source = src.c_str();
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(type);

	//error checking
	int x;
	char info[512];
	glGetProgramiv(id, GL_LINK_STATUS, &x);
	if (!x) {
		glGetProgramInfoLog(id, 512, NULL, info);
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
	glAttachShader(tempProgram, vertexShader);
	glAttachShader(tempProgram, fragmentShader);
	glLinkProgram(tempProgram);

	//error checking
	int x;
	char info[512];
	glGetProgramiv(tempProgram, GL_LINK_STATUS, &x);
	if (!x) {
		glGetProgramInfoLog(tempProgram, 512, NULL, info);
		std::cout << "\n\nShader Compilation Error: " << info << "\n";
	}

	glValidateProgram(tempProgram);
	
	//shader is created can clear memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return tempProgram;
}


