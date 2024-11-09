#pragma once

#include<vector>


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

		void AddSpriteData(const glm::vec2& offset);
		//moves specified instance
		void MoveSpriteInstance(glm::vec2 offset, const unsigned int& instanceNum);
		void SendSpriteInstancesToGPU();

		//sets the ID attribute 
		void AddSpriteID(int id);
		void SendTextureIdsToGPU();

		
	private:
		//data for all the sprites in the game that need to be rendered
		std::vector<float> vertexData;
		std::vector<unsigned int> spriteIndices;
		std::vector<float> textureIDs;

		std::vector<glm::vec2> translations;

		

		unsigned int instanceVBO;
		unsigned int texIDVBO; //vertex buffer for texture id's
		

		//temperary texture stuff
		unsigned int texture;
		
	};

}