#pragma once
#include<string>

#define GLM_ENABLE_EXPERIMENTAL

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<gtx/string_cast.hpp>

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

		 void SetUniformMat4(const glm::mat4& x, const std::string& name);
		 void SetUniformVec4(const glm::vec4& x, const std::string& name);
	private:
		ShaderProgramSource ParseShader(const std::string& vertexFilePath, const std::string& fragFilePath);
		int CompileShader(unsigned int type, const std::string& src);
		unsigned int CreateShader(const std::string& vertShader, const std::string& fragShader);
	 private:
		 unsigned int program;
	};

}