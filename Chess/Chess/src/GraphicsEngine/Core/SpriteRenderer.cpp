#include "SpriteRenderer.h"

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION

#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include"../Core/Shader.h"
#include"../Core/Macros.h"
#include "../Core/Dependencies/stb_image.h"

GraphicsEngine::SpriteRenderer::SpriteRenderer()
{
	this->shader = new GraphicsEngine::Shader("src/GraphicsEngine/Shaders/Vertex/SpriteVert.txt",
		"src/GraphicsEngine/Shaders/Frag/SpriteFrag.txt");

	vertexData = {
		 0.f, 0.f, 0.f,
		 0.f, 0.f, 
		 1.f, 0.f, 0.f,
		 1.f, 0, 
		 1.f, 1.f, 0.f,
		 1.f, 1.f,
		 0.f, 1.f, 0.f,
		 .0f, 1.f
	};

	spriteIndices = {
		0, 1, 3,
		1, 2, 3
	};

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 7.f, -1.f));


	//TEMPORARY STUFF BECAUSE I WILL ADD THIS TO THE SPRITES THEMSELVES

	stbi_set_flip_vertically_on_load(true);

	//setup nearest neighbor texture filetering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//setup alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);


	int width, height, nrmChnls;
	unsigned char* data = stbi_load("src/Game/Textures/8BitPieces.png", &width, &height, &nrmChnls, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	//generate the instance vbos
	glCheck(glGenBuffers(1, &texIDVBO));
	glCheck(glGenBuffers(1, &instanceVBO));
}

GraphicsEngine::SpriteRenderer::~SpriteRenderer()
{
}

void GraphicsEngine::SpriteRenderer::SetupBuffer()
{
	glCheck(glGenVertexArrays(1, &vao));
	glCheck(glBindVertexArray(vao));
	
	glCheck(glGenBuffers(1, &vbo));
	glCheck(glGenBuffers(1, &ebo));

	//vertex buffer setup
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW));
	
	glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	glCheck(glEnableVertexAttribArray(0));
	glCheck(glEnableVertexAttribArray(2));

	//element setup
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, spriteIndices.size() * sizeof(unsigned int), spriteIndices.data(), GL_DYNAMIC_DRAW));

	//instance offset buffer setup
	//instanceVBO buffer generated in constructor so AddSpriteData can access it
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec2), translations.data(), GL_DYNAMIC_DRAW));

	glCheck(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribDivisor(1, 1)); // update once per instance

	//instance texture id setup
	//texIDVBO buffer generate in constructor so AddSpriteID can bind to it
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, texIDVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER, textureIDs.size() * sizeof(float), textureIDs.data(), GL_STATIC_DRAW));

	glCheck(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0));
	glCheck(glEnableVertexAttribArray(3));
	glCheck(glVertexAttribDivisor(3, 1));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GraphicsEngine::SpriteRenderer::Draw()
{
	glCheck(glBindVertexArray(vao));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

	//draw the amnt of translations there are
	glCheck(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, translations.size()));
	

	//glCheck(glBindVertexArray(0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void GraphicsEngine::SpriteRenderer::AddSpriteData(const glm::vec2& offset)
{
	translations.push_back(offset);
}

void GraphicsEngine::SpriteRenderer::RemoveSprite(int index)
{
	translations.erase(translations.begin() + index);
	SendSpriteInstancesToGPU();
	textureIDs.erase(textureIDs.begin() + index);
	SendTextureIdsToGPU();
}

void GraphicsEngine::SpriteRenderer::SendSpriteInstancesToGPU()
{
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec2), translations.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GraphicsEngine::SpriteRenderer::MoveSpriteInstance(glm::vec2 offset, const unsigned int& instanceNum)
{
	if (instanceNum > translations.size()) {
		std::cout << "Error: Piece Out Of Range, Could Not Move It.";
		return;
	}

	//do the lazy workaround here as well, like in the base chess piece
	//offset.x = -offset.x - 7;

	translations.at(instanceNum) = offset;

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, instanceNum * sizeof(glm::vec2), sizeof(glm::vec2), &translations.at(instanceNum));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//SendSpriteInstancesToGPU();
}

void GraphicsEngine::SpriteRenderer::AddSpriteID(int id)
{
	textureIDs.push_back(id);
}

void GraphicsEngine::SpriteRenderer::SendTextureIdsToGPU()
{
	glBindBuffer(GL_ARRAY_BUFFER, texIDVBO);
	glBufferData(GL_ARRAY_BUFFER, textureIDs.size() * sizeof(float), textureIDs.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



