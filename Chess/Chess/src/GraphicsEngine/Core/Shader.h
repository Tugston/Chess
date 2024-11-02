#pragma once
#include<string>

namespace GraphicsEngine {

	//shader source code struct
	//the shaders are all compiled together, so may as well make a struct to pass them in
	struct ShaderProgramSource {
		ShaderProgramSource(const std::string& VertexShader, const std::string& FragmentShader)
		: VertexShader(VertexShader), FragmentShader(FragmentShader){}
		std::string VertexShader;
		std::string FragmentShader;
	};

	class Shader {
	 public:
		 Shader(const std::string& vertexFilePath, const std::string& fragFilePath);
		 void Use();
	private:
		ShaderProgramSource ParseShader(const std::string& vertexFilePath, const std::string& fragFilePath);
		int CompileShader(unsigned int type, const std::string& src);
		unsigned int CreateShader(const std::string& vertShader, const std::string& fragShader);
	 private:
		 unsigned int program;
	};

}