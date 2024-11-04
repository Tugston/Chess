#include "Game.h"

#include "../GraphicsEngine/Core/Macros.h"


using namespace Chess;

Game::Game() : quit(false){
	window = new GraphicsEngine::Window();

	//shader = new GraphicsEngine::Shader("src/GraphicsEngine/Shaders/Vertex/BoardVertex.txt",
		//"src/GraphicsEngine/Shaders/Frag/BoardFrag.txt");

	camera = new GraphicsEngine::Camera;
	
	//board has gameplay material, so need a direct pointer as well as pushing it into the drawables
	//in order to be drawn
	board = new Board;
	drawables.push_back(board);

	model = glm::translate(model, glm::vec3(.5f, .5f, -1.0f));
	model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
}

Game::~Game()
{
	for (int i = 0; i < drawables.size(); i++)
	{
		delete drawables[i];
	}

	drawables.clear();

	delete board;

	delete window;
}

void Game::Start()
{
	//std::cout << glGetString(GL_VERSION);

	//shader->Use();
	
	/*
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
	*/

	Tick();
}

void Game::Draw(Uint32 DeltaTime)
{

	glClearColor(.48f, .85f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);



	//shader->SetUniformMat4(camera->GetViewMatrix(), "view");
	//shader->SetUniformMat4(camera->projection, "projection");
	//shader->SetUniformMat4(model, "model");

	//glBindVertexArray(vao);

	//glCheck(glBindVertexArray(vao));
	//glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (int i = 0; i < drawables.size(); i++)
	{
		drawables.at(i)->GetShader()->Use();

		drawables.at(i)->GetShader()->SetUniformMat4(camera->GetViewMatrix(), "view");
		drawables.at(i)->GetShader()->SetUniformMat4(camera->projection, "projection");
		drawables.at(i)->GetShader()->SetUniformMat4(drawables.at(i)->GetModelMatrix(), "model");

		drawables.at(i)->Draw();
	}


	SDL_GL_SwapWindow(window->GetWindow());
	
	
}

void Game::Tick()
{
	while (!quit)
	{
		Uint32 currentTime = SDL_GetTicks();
		Uint32 DeltaTime = currentTime - lastUpdateTime;


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


		Draw(DeltaTime);

		if (DeltaTime >= 1)
			lastUpdateTime = currentTime;
	}

	
}
