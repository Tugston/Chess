#include "Board.h"

#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include "../GraphicsEngine/Core/Macros.h"

Chess::Board::Board()
{
	SetupData();

	this->shader = new GraphicsEngine::Shader("src/GraphicsEngine/Shaders/Vertex/BoardVertex.txt",
		"src/GraphicsEngine/Shaders/Frag/BoardFrag.txt");
	
	//gl buffer setup
	glCheck(glBindVertexArray(vao));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW));

	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW));

	glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	glCheck(glEnableVertexAttribArray(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -1.0f));
}


void Chess::Board::Draw()
{

	glCheck(glBindVertexArray(vao));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	
	glDrawElements(GL_TRIANGLES, 6 * 64, GL_UNSIGNED_INT, 0);
}

void Chess::Board::SetupData()
{
	//chess board is 8x8, doesn't need window size or anything
	//ends at 7 because the first one is already entered
	for (int i = 0; i < 8; i++)
	{
		for (int y = 0; y < 8; y++) {
			//adding vertex data based on x and y, z always 0
			vertexData.push_back(i);
			vertexData.push_back(y);
			vertexData.push_back(0.f);
			vertexData.push_back(i + 1);
			vertexData.push_back(y);
			vertexData.push_back(0.f);
			vertexData.push_back(i + 1);
			vertexData.push_back(y + 1);
			vertexData.push_back(0.f);
			vertexData.push_back(i);
			vertexData.push_back(y + 1);
			vertexData.push_back(0.f);
		}

	}

	//populate the ebo indices
	for (int i = 0; i < 64; i++)
	{
		//indices.push_back(indices.at(i) + 4);

		unsigned int offset = i * 4;

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);

		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);
	}
}
