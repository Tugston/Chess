#include "SpriteRenderer.h"

#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include"../Core/Shader.h"
#include"../Core/Macros.h"

GraphicsEngine::SpriteRenderer::SpriteRenderer()
{
	this->shader = new GraphicsEngine::Shader("src/GraphicsEngine/Shaders/Vertex/SpriteVert.txt",
		"src/GraphicsEngine/Shaders/Frag/SpriteFrag.txt");

	vertexData = {
		-0.5f, -0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		 0.5f,  0.5f, 0.f,
		-0.5f,  0.5f, 0.f
	};

	spriteIndices = {
		0, 1, 2,
		0, 2, 3
	};

	modelMatrix = glm::translate(modelMatrix, glm::vec3(.5f, .5f, -1.f));
}

GraphicsEngine::SpriteRenderer::~SpriteRenderer()
{
}

void GraphicsEngine::SpriteRenderer::SetupBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	//vertex buffer setup
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//element setup
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, spriteIndices.size() * sizeof(unsigned int), spriteIndices.data(), GL_DYNAMIC_DRAW);

	//instance offset buffer setup
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec2), translations.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1); // update once per instance

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GraphicsEngine::SpriteRenderer::Draw()
{
	glCheck(glBindVertexArray(vao));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

	glCheck(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 32));

	glCheck(glBindVertexArray(0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void GraphicsEngine::SpriteRenderer::AddSpriteData(glm::vec2 translation)
{
	translations.push_back(translation);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec2), translations.data(), GL_DYNAMIC_DRAW);
}

