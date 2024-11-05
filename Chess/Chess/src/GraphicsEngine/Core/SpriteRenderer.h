#pragma once


#include<vector>

//#include<stb_image.h>


#include "../Core/Drawable.h"

namespace GraphicsEngine {

	class SpriteRenderer : public Drawable {
	public:

	public:
		SpriteRenderer();
		~SpriteRenderer();

		void SetupBuffer();
		void Draw();

		Shader* GetShader() { return this->shader; };
		glm::mat4 GetModelMatrix() { return this->modelMatrix; };

		void AddSpriteData(glm::vec2 translation);
	private:
		//data for all the sprites in the game that need to be rendered
		std::vector<float> vertexData;
		std::vector<unsigned int> spriteIndices;

		std::vector<glm::vec2> translations;

		unsigned int instanceVBO;
		
	};

}