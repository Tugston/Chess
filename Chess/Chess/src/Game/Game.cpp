#include "Game.h"

#include "../GraphicsEngine/Core/Macros.h"

//pieces
#include "Pieces/King.h"
#include "Pieces/Bishop.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"


using namespace Chess;

Game::Game() : quit(false), lastUpdateTime(0), currentTurn(Turn::White){

	//lightweight gl classes are created on constructor
	window = new GraphicsEngine::Window;
	camera = new GraphicsEngine::Camera;


	board = nullptr;
	spriteRenderer = nullptr;

	currentSelectedPiece = nullptr;
}

Game::~Game()
{
	for (GraphicsEngine::Drawable* i : drawables)
	{
		delete i;
	}

	drawables.clear();

	for (BasePiece* i : WhitePieces)
	{
		delete i;
	}

	for (BasePiece* i : BlackPieces)
	{
		delete i;
	}

	delete board;
	delete spriteRenderer;
	delete window;
}

void Game::Start()
{

	//heavier using gl classes created on start
	
	//board has gameplay material, so need a direct pointer as well as pushing it into the drawables
	//in order to be drawn easier
	board = new Board;
	drawables.push_back(board);

	spriteRenderer = new GraphicsEngine::SpriteRenderer;
	SetupPieces();
	

	Tick();
}

void Game::Draw(Uint32 DeltaTime)
{

	glClearColor(.48f, .85f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < drawables.size(); i++)
	{

		//can't bind shader in draw call in the class because I have to set the uniforms here,
		//and it needs camera data to properly configure them and I do not want to pass the camera
		//to the base drawable just for this
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
	//mouse x and y position
	int x, y = 0;

	while (!quit)
	{
		Uint32 currentTime = SDL_GetTicks();
		Uint32 DeltaTime = currentTime - lastUpdateTime;

		std::vector<glm::vec2> testPos;
		testPos.push_back(glm::vec2(1, 1));
		//testPos.push_back(glm::vec2(0, 0));
		//testPos.push_back(glm::vec2(1, 0));
		//testPos.push_back(glm::vec2(0, 1));

		SDL_Event keyEvent;
		while (SDL_PollEvent(&keyEvent))
		{
			switch (keyEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (keyEvent.button.button == SDL_BUTTON_LEFT) {

					SDL_GetMouseState(&x, &y);
					//y = SCREENSIZE - y;
					//x = SCREENSIZE - x;

					if (currentSelectedPiece)
					{
						bool canMove = currentSelectedPiece->GetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves);
						
						for (int i = 0; i < moves.size(); i++)
						{
							PrintVec2Data(moves.at(i), "Move " + std::to_string(i));
						}

						if (canMove) {
							//move piece
							currentSelectedPiece->SetOffset(ScreenPosToOffset(glm::vec2(x * -1, y)));
							currentSelectedPiece->SetStartOffset(ScreenPosToOffset(glm::vec2(x * -1, y)));

							CapturePiece(x, y);

							//clear moves
							moves.clear();
							board->DisplayMoves(moves);

							spriteRenderer->MoveSpriteInstance(currentSelectedPiece->GetOffset(), currentSelectedPiece->GetArrayIndex());


							PrintVec2Data(glm::vec2(x, y), "Mouse Position");
							PrintVec2Data(ScreenPosToOffset(glm::vec2(x, y)), "Mouse Offset - add 8 to x for board pos");
							PrintVec2Data(currentSelectedPiece->GetStartOffset(), "Piece Starting Offset - Dropped");


							currentSelectedPiece = nullptr;
							SwapTurn();
						}
					}
					else {

						//left click to pickup piece according to turn
						if (currentTurn == White) {

							for (int i = 0; i < WhitePieces.size(); i++)
							{
								if (WhitePieces[i]->HitDetection(ScreenPosToOffset(glm::vec2(x * -1, y)), true)) {
									currentSelectedPiece = WhitePieces[i];
									currentSelectedPiece->GetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves, (currentTurn == White) ? true : false, 
										GetPiecePositions(White), GetPiecePositions(Black));
									for (int i = 0; i < moves.size(); i++)
									{
										PrintVec2Data(moves.at(i), "Move " + std::to_string(i));
									}

									std::cout << currentSelectedPiece->GetTypeName() << "\n";
									PrintVec2Data(WhitePieces[i]->GetOffset(), "Piece Current Offset");
									PrintVec2Data(WhitePieces[i]->GetStartOffset(), "Piece Starting Offset");
									PrintVec2Data(glm::vec2(x, y), "Mouse Position");

								};
							}
						}
						else if (currentTurn == Black) {
							for (int i = 0; i < BlackPieces.size(); i++)
							{
								if (BlackPieces[i]->HitDetection(ScreenPosToOffset(glm::vec2(x * -1, y)), false)) {
									currentSelectedPiece = BlackPieces[i];
									currentSelectedPiece->GetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves, (currentTurn == White) ? true : false,
										GetPiecePositions(White), GetPiecePositions(Black));
									for (int i = 0; i < moves.size(); i++)
									{
										PrintVec2Data(moves.at(i), "Move " + std::to_string(i));
									}

									std::cout << currentSelectedPiece->GetTypeName() << "\n";
									PrintVec2Data(BlackPieces[i]->GetOffset(), "Piece Current Offset");
									PrintVec2Data(BlackPieces[i]->GetStartOffset(), "Piece Starting Offset");
									PrintVec2Data(glm::vec2(x, y), "Mouse Position");
								}
							}
						}

						PrintVec2Data(ScreenPosToOffset(glm::vec2(x, y)), "Mouse Offset - add 8 to x for board pos");
					}
				}
				else {
					if (currentSelectedPiece) {
						//pass in true here because for some reason the movement works with both black and white when using -8
						currentSelectedPiece->SetOffset(currentSelectedPiece->GetStartOffset());
						moves.clear();
						board->DisplayMoves(moves);
						spriteRenderer->MoveSpriteInstance(currentSelectedPiece->GetStartOffset(),
							currentSelectedPiece->GetArrayIndex());
						currentSelectedPiece = nullptr;

					}
				}
				break;
			default:
				break;
			}
		}

		//move the piece with the mouse position
		if (currentSelectedPiece) {
			board->DisplayMoves(moves);
			SDL_GetMouseState(&x, &y);
			glm::vec2 movePosition = ScreenPosToOffset(glm::vec2(x, y));
			movePosition.x += 7;
			spriteRenderer->MoveSpriteInstance(movePosition, currentSelectedPiece->GetArrayIndex());
		}


		Draw(DeltaTime);

		if (DeltaTime >= 1)
			lastUpdateTime = currentTime;
	}

	
}

void Chess::Game::SetupPieces()
{
	//location offsets
	float currentXOffset = 0;
	float currentYOffset = 0;

	


	//populate white pieces
	//could loop this, but whats the point of using the enum here if I just pass uints anyways
	WhitePieces.push_back(new Rook(WHITE, ScreenPosToOffset(glm::vec2(800, 700)), 0));
	WhitePieces.push_back(new Knight(WHITE, ScreenPosToOffset(glm::vec2(900, 700)), 1));
	WhitePieces.push_back(new Bishop(WHITE, ScreenPosToOffset(glm::vec2(1000, 700)), 2));
	WhitePieces.push_back(new Queen(WHITE, ScreenPosToOffset(glm::vec2(1100, 700)), 3));
	WhitePieces.push_back(new King(WHITE, ScreenPosToOffset(glm::vec2(1200, 700)), 4));
	WhitePieces.push_back(new Bishop(WHITE, ScreenPosToOffset(glm::vec2(1300, 700)), 5));
	WhitePieces.push_back(new Knight(WHITE, ScreenPosToOffset(glm::vec2(1400, 700)), 6));
	WhitePieces.push_back(new Rook(WHITE, ScreenPosToOffset(glm::vec2(1500, 700)), 7));

	//we can loop the pawns cause I am tired of copy pasting
	for (int i = 1; i < 9; i++)
	{
		WhitePieces.push_back(new Pawn(WHITE, ScreenPosToOffset(glm::vec2(i * 100 + 700, 600)), i + 7));
	}

	//original black piece positions were off by 1 so my duct tape solution was to slide them over and adjust the position of the sprite
	//which is why these go from 700 - 1400, and the white go from 800 - 1500
	BlackPieces.push_back(new Rook(BLACK, ScreenPosToOffset(glm::vec2(700, 0)), 15));
	BlackPieces.push_back(new Knight(BLACK, ScreenPosToOffset(glm::vec2(800, 0)), 16));
	BlackPieces.push_back(new Bishop(BLACK, ScreenPosToOffset(glm::vec2(900, 0)), 17));
	BlackPieces.push_back(new Queen(BLACK, ScreenPosToOffset(glm::vec2(1000, 0)), 18));
	BlackPieces.push_back(new King(BLACK, ScreenPosToOffset(glm::vec2(1100, 0)), 19));
	BlackPieces.push_back(new Bishop(BLACK, ScreenPosToOffset(glm::vec2(1200, 0)), 20));
	BlackPieces.push_back(new Knight(BLACK, ScreenPosToOffset(glm::vec2(1300, 0)), 21));
	BlackPieces.push_back(new Rook(BLACK, ScreenPosToOffset(glm::vec2(1400, 0)), 22));

	for (int i = 1; i < 9; i++)
	{
		BlackPieces.push_back(new Pawn(BLACK, ScreenPosToOffset(glm::vec2(i * 100 + 600, 100)), i + 22));
	} 

	for (int i = 0; i < WhitePieces.size(); i++)
	{
		spriteRenderer->AddSpriteData(WhitePieces.at(i)->GetOffset());
		spriteRenderer->AddSpriteID(WhitePieces.at(i)->GetTextureID());
	}

	for (int i = 0; i < BlackPieces.size(); i++)
	{
		spriteRenderer->AddSpriteData(BlackPieces.at(i)->GetOffset());
		spriteRenderer->AddSpriteID(BlackPieces.at(i)->GetTextureID());
	}

	spriteRenderer->SendSpriteInstancesToGPU();
	spriteRenderer->SendTextureIdsToGPU();

	spriteRenderer->SetupBuffer();
	drawables.push_back(spriteRenderer);
}

void Chess::Game::SwapTurn()
{
	currentTurn = (currentTurn == White ? Turn::Black : Turn::White);
}

void Chess::Game::CapturePiece(int x, int y)
{
	//capture any pieces
	for (int i = 0; i < currentSelectedPiece->GetTakeIndexes().size(); i++)
	{
		//this loop only occurs a few times so shouldnt hurt throwing this here to use in white and black
		glm::vec2 mousePos = ScreenPosToOffset(glm::vec2(x * -1, y));
		mousePos.x = -mousePos.x - 8;

		if (currentTurn == White) {
			PrintVec2Data(BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetStartOffset(), "START OFFSET OF TAKE INDEX PIECE");
			PrintVec2Data(ScreenPosToOffset(glm::vec2(x * -1, y)), "CURRENT MOUSE POSITION");
			PrintVec2Data(glm::vec2(BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex(), -1), "X VALUE IS ARRAY INDEX");

			if (BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetStartOffset() == mousePos) {

				//set array index -1 for all pieces after the taken one
				for (int y = currentSelectedPiece->GetTakeIndexes().at(i); y < BlackPieces.size(); y++) {
					BlackPieces[y]->SetArrayIndex(BlackPieces[y]->GetArrayIndex() - 1);
				}
				//TRYING TO FIX THE OFFSET ISSUES WHEN TAKING PIECE
				spriteRenderer->RemoveSprite(BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex());
				
				delete BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i) - 1];
				BlackPieces.erase(BlackPieces.begin() + currentSelectedPiece->GetTakeIndexes().at(i));
				break;
			}
		}
		else if (currentTurn == Black) {
			if (WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetStartOffset() == mousePos) {
				spriteRenderer->RemoveSprite(WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex());
				delete WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)];
				WhitePieces.erase(BlackPieces.begin() + currentSelectedPiece->GetTakeIndexes().at(i));
				break;
			}
		}
	}
}

glm::vec2 Chess::Game::ScreenPosToOffset(const glm::vec2& screenCoords)
{
	
	int cellSize = SCREENSIZE / 8;

	

	int x = (SCREENSIZE - screenCoords.x) / cellSize;
	int y = screenCoords.y / cellSize;

	//x = (8 - 1) - x;

	y *= -1;
	x *= -1;
	return glm::vec2(x, y);
	
}

glm::vec2 Chess::Game::OffsetToScreenPos(glm::vec2 offset)
{
	int cellSize = SCREENSIZE / 8;

	offset.x *= -1;
	offset.y *= -1;

	int x = SCREENSIZE - (offset.x - 1) * cellSize;
	int y = offset.y * cellSize;

	return glm::vec2(x, y);
}

std::vector<glm::vec2> Chess::Game::GetPiecePositions(Turn turn)
{
	std::vector<glm::vec2> x;
	if (turn == White) {
		for (int i = 0; i < WhitePieces.size(); i++) {
			x.push_back(WhitePieces[i]->GetStartOffset());
		}
	}
	else {
		for (int i = 0; i < BlackPieces.size(); i++)
		{
			x.push_back(BlackPieces[i]->GetStartOffset());
		}
	}

	return x;
}

void Chess::Game::PrintVec2Data(glm::vec2 x, std::string name)
{
	for (int i = 0; i < name.length(); i++)
	{
		std::cout << "-";
	}

	std::cout << "\n" << name << ": \n";

	std::cout << "X: " << x.x << "\n";
	std::cout << "Y: " << x.y << "\n";

	for (int i = 0; i < name.length(); i++)
	{
		std::cout << "-";
	}
	
	std::cout << "\n\n";
}
