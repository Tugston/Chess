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
		-0.5f, -0.5f, 0.f
	};

	spriteIndices = {
		0, 1, 3,
		1, 2, 3
	};

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -1.f));
}

GraphicsEngine::SpriteRenderer::~SpriteRenderer()
{
}

void GraphicsEngine::SpriteRenderer::SetupBuffer()
{
	glGenVertexArrays(1, &vao);
	
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, spriteIndices.size() * sizeof(unsigned int), spriteIndices.data(), GL_DYNAMIC_DRAW);


	for (int i = 0; i < 32; i++)
	{
		shader->SetUniformVec2(translations[i], ("offsets[" + std::to_string(i) + "]"));
	}
}

void GraphicsEngine::SpriteRenderer::Draw()
{
	glCheck(glBindVertexArray(vao));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

	glCheck(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, spriteIndices.data(), 32));
}

void GraphicsEngine::SpriteRenderer::AddSpriteData(glm::vec2 translation)
{
	translations.push_back(translation);
}

