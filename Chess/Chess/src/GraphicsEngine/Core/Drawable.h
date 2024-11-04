#pragma once

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

namespace GraphicsEngine {

	class Shader;

	//base class for anything that is drawable, so game class can store them
	class Drawable {
	public:
		Drawable();
		~Drawable();
		virtual void Draw() = 0;

		virtual Shader* GetShader() = 0;
		virtual glm::mat4 GetModelMatrix() = 0;
		
	protected:
		unsigned int vao = 0;
		unsigned int vbo = 0;
		unsigned int ebo = 0;

		glm::mat4 modelMatrix = glm::mat4(1.f);

		GraphicsEngine::Shader* shader;
	};

}