#include "Game.h"

#include"../GraphicsEngine/Core/Macros.h"

using namespace Chess;

Game::Game() : quit(false) {
	window = new GraphicsEngine::Window();

	shader = new GraphicsEngine::Shader("src/GraphicsEngine/Shaders/Vertex/BoardVertex.txt",
		"src/GraphicsEngine/Shaders/Frag/BoardFrag.txt");
	
}

Game::~Game()
{
	delete window;
}

void Game::Start()
{
	//std::cout << glGetString(GL_VERSION);

	shader->Use();
	
	glCheck(glGenVertexArrays(1, &vao));
	glCheck(glGenBuffers(1, &vbo));
	glCheck(glGenBuffers(1, &ebo));

	glCheck(glBindVertexArray(vao));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW));

	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW));
	
	glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	glCheck(glEnableVertexAttribArray(0));


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Tick();
}

void Game::Draw()
{

	glClearColor(.48f, .85f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->Use();

	glBindVertexArray(vao);

	glCheck(glBindVertexArray(vao));
	//glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	SDL_GL_SwapWindow(window->GetWindow());
	
	
}

void Game::Tick()
{
	while (!quit)
	{
		SDL_Event keyEvent;
		while (SDL_PollEvent(&keyEvent))
		{
			switch (keyEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
			}
		}


		Draw();
	}

	
}
