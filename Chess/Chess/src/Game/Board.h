#pragma once

#include<vector>

#include "../GraphicsEngine/Core/Drawable.h"
#include "../GraphicsEngine/Core/Shader.h"

namespace Chess {

	class Board : public GraphicsEngine::Drawable {
	public:
		Board();

		void Draw();

		//getters
		GraphicsEngine::Shader* GetShader() { return shader; };
		glm::mat4 GetModelMatrix() { return modelMatrix; };

	private:
		//adds all the data to the vertexData and indices
		void SetupData();
	private:
		std::vector<float> vertexData{};
		std::vector<unsigned int> indices{};
	};

}