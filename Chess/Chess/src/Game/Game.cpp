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

	WhitePieces.clear();
	BlackPieces.clear();

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

	//initliaze each piece with starting moves
	for (int i = 0; i < WhitePieces.size(); i++) {
		std::vector<glm::vec2> tempStorage;
		WhitePieces[i]->SetMoves(WhitePieces[i]->GetStartOffset(), tempStorage, true, GetPiecePositions(White), GetPiecePositions(Black));
	}
	for (int i = 0; i < BlackPieces.size(); i++) {
		std::vector<glm::vec2> tempStorage;
		BlackPieces[i]->SetMoves(WhitePieces[i]->GetStartOffset(), tempStorage, false, GetPiecePositions(White), GetPiecePositions(Black));
	}
	

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

	//stores the rook positions for castling
	glm::vec2 currentRookIndex = glm::vec2{ -1,-1 };

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

					PrintPieceMovesAtMousePos(glm::vec2(x, y));

					if (currentSelectedPiece)
					{
						bool canMove = currentSelectedPiece->GetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves);
						
						for (int i = 0; i < moves.size(); i++)
						{
							PrintVec2Data(moves.at(i), "Move " + std::to_string(i));
						}

						if (canMove) {

							//
							MakeCastlingMove(currentSelectedPiece, glm::vec2(x,y), currentRookIndex);

							//move piece
							currentSelectedPiece->SetOffset(ScreenPosToOffset(glm::vec2(x * -1, y)));
							currentSelectedPiece->SetStartOffset(ScreenPosToOffset(glm::vec2(x * -1, y)));

							//set moves when dropping piece for castling and en passant
							currentSelectedPiece->SetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves, (currentTurn == White) ? true : false,
								GetPiecePositions(White), GetPiecePositions(Black));

							CapturePiece(x, y);

							//clear moves
							moves.clear();
							board->DisplayMoves(moves);

							spriteRenderer->MoveSpriteInstance(currentSelectedPiece->GetOffset(), currentSelectedPiece->GetArrayIndex());


							PrintVec2Data(glm::vec2(x, y), "Mouse Position");
							PrintVec2Data(ScreenPosToOffset(glm::vec2(x, y)), "Mouse Offset - add 8 to x for board pos");
							PrintVec2Data(currentSelectedPiece->GetStartOffset(), "Piece Starting Offset - Dropped");


							currentSelectedPiece->SetHasMoved(true);

							currentSelectedPiece = nullptr;
							SwapTurn();
						}
					}
					else {

						//old deubgging
						//PrintPieceMovesAtMousePos(glm::vec2(x, y));

						//left click to pickup piece according to turn
						if (currentTurn == White) {

							for (int i = 0; i < WhitePieces.size(); i++)
							{
								if (WhitePieces[i]->HitDetection(ScreenPosToOffset(glm::vec2(x * -1, y)), true)) {
									currentSelectedPiece = WhitePieces[i];

									//set moves when picking up piece for taking to work
									currentSelectedPiece->SetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves, true,
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

									//set moves when picking up piece for taking to work
									currentSelectedPiece->SetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves, false,
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

						//check castling
						currentRookIndex = CastleCheck(currentSelectedPiece);
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

	//ALL THESE PROBLEMS WERE CAUSED BECAUSE I COULD NOT COUNT, I STARTED THE INDEX AT 15 INSTEAD OF 16!!!!
	//**LEAVING THIS HERE BECAUSE I AM DUMB**
	//---------------------------------------------------------------------------------------//
	//I AM RE-REALIZING THIS A MONTH LATER!!!! (This is why I had a 2 offset on capturing)
	//original black piece positions were off by 1 so my duct tape solution was to slide them over and adjust the position of the sprite
	//which is why these go from 700 - 1400, and the white go from 800 - 1500
	BlackPieces.push_back(new Rook(BLACK, ScreenPosToOffset(glm::vec2(800, 0)), 16)); //started 15
	BlackPieces.push_back(new Knight(BLACK, ScreenPosToOffset(glm::vec2(900, 0)), 17));
	BlackPieces.push_back(new Bishop(BLACK, ScreenPosToOffset(glm::vec2(1000, 0)), 18));
	BlackPieces.push_back(new Queen(BLACK, ScreenPosToOffset(glm::vec2(1100, 0)), 19));
	BlackPieces.push_back(new King(BLACK, ScreenPosToOffset(glm::vec2(1200, 0)), 20));
	BlackPieces.push_back(new Bishop(BLACK, ScreenPosToOffset(glm::vec2(1300, 0)), 21));
	BlackPieces.push_back(new Knight(BLACK, ScreenPosToOffset(glm::vec2(1400, 0)), 22));
	BlackPieces.push_back(new Rook(BLACK, ScreenPosToOffset(glm::vec2(1500, 0)), 23));

	for (int i = 1; i < 9; i++)
	{
		BlackPieces.push_back(new Pawn(BLACK, ScreenPosToOffset(glm::vec2(i * 100 + 700, 100)), i + 23));
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
	//this loop only occurs a few times so shouldnt hurt throwing this here to use in white and black
	glm::vec2 mousePos = ScreenPosToOffset(glm::vec2(x * -1, y));
	mousePos.x = -mousePos.x - 8;

	//capture any pieces
	for (int i = 0; i < currentSelectedPiece->GetTakeIndexes().size(); i++)
	{

		if (currentTurn == White) {
			PrintVec2Data(BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetStartOffset(), "START OFFSET OF TAKE INDEX PIECE");
			PrintVec2Data(mousePos, "CURRENT MOUSE POSITION");
			PrintVec2Data(glm::vec2(BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex(), -1), "X VALUE IS ARRAY INDEX");

			if (BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetStartOffset() == mousePos) {

				//set array index -= 1 for all pieces after the taken one
				//for (int y = currentSelectedPiece->GetTakeIndexes().at(i); y < BlackPieces.size(); y++) {
				//	BlackPieces[y]->SetArrayIndex(BlackPieces[y]->GetArrayIndex() - 2);
			//	}
				
				spriteRenderer->RemoveSprite(BlackPieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex()); // + 1

				for (int y = currentSelectedPiece->GetTakeIndexes().at(i); y < BlackPieces.size(); y++) {
					BlackPieces[y]->SetArrayIndex(BlackPieces[y]->GetArrayIndex() - 1);
				}
				
				delete BlackPieces.at(currentSelectedPiece->GetTakeIndexes().at(i));
				BlackPieces.erase(BlackPieces.begin() + currentSelectedPiece->GetTakeIndexes().at(i));
				break;
			}
		}
		else if (currentTurn == Black) {
			PrintVec2Data(WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetStartOffset(), "START OFFSET OF TAKE INDEX PIECE");
			PrintVec2Data(mousePos, "CURRENT MOUSE POSITION");
			PrintVec2Data(glm::vec2(WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex(), -1), "X VALUE IS ARRAY INDEX");

			if (WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetStartOffset() == mousePos) {

				spriteRenderer->RemoveSprite(WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex());

				//for (int y = currentSelectedPiece->GetTakeIndexes().at(i); y >= 0; y--) { //y < WhitePiece.size()
					//WhitePieces[y]->SetArrayIndex(WhitePieces[y]->GetArrayIndex() + 2); // + 1
				//}

				for (int y = currentSelectedPiece->GetTakeIndexes().at(i); y < WhitePieces.size(); y++)
				{
					WhitePieces[y]->SetArrayIndex(WhitePieces[y]->GetArrayIndex() - 1);
				}

				for (int y = 0; y < BlackPieces.size(); y++) {
					BlackPieces[y]->SetArrayIndex(BlackPieces[y]->GetArrayIndex() - 1);
				}

				//spriteRenderer->RemoveSprite(WhitePieces[currentSelectedPiece->GetTakeIndexes().at(i)]->GetArrayIndex() + 17); //17
				delete WhitePieces.at(currentSelectedPiece->GetTakeIndexes().at(i));
				WhitePieces.erase(WhitePieces.begin() + currentSelectedPiece->GetTakeIndexes().at(i));
				break;
			}
		}
	}
}

glm::vec2 Chess::Game::CastleCheck(Chess::BasePiece* kingPiece)
{
	int leftRookIndex = -1;
	int rightRookIndex = -1;

	bool leftClear = false;
	bool rightClear = false;

	//verify all the king stuff before doing rook checks and everthing
	if (kingPiece && kingPiece->GetTypeName() == "King" && kingPiece->GetHasMoved() == false) {
		if (currentTurn == White) {
			//check if the two rooks have moved first, before checking if there are pieces between rooks and king
			for (int i = 0; i < WhitePieces.size(); i++)
			{
				if (WhitePieces[i]->GetTypeName() == "Rook" && WhitePieces[i]->GetHasMoved() == false) {
					if (WhitePieces[i]->GetStartOffset().x == 0) {
						leftRookIndex = i;
					}
					else { //there are only 2 rooks, so if it hasnt moved and isn't in the 0 column, then it must be the right rook
						rightRookIndex = i;
					}
				}
			}
		}
		else if (currentTurn == Black) { //exact opposite for black obviously
			for (int i = 0; i < BlackPieces.size(); i++) {
				if (BlackPieces[i]->GetTypeName() == "Rook" && BlackPieces[i]->GetHasMoved() == false) {
					if (BlackPieces[i]->GetStartOffset().x == 0) {
						rightRookIndex = i;
					}
					else {
						leftRookIndex = i;
					}
				}
			}
		}

		if(leftRookIndex >= 0)
			leftClear = CheckCastlingClear(leftRookIndex, kingPiece, true);
		if(rightRookIndex >= 0)
			rightClear = CheckCastlingClear(rightRookIndex, kingPiece, false);
	}

	//finally add the moves
	if (currentTurn == White) {
		if (leftClear) {
			kingPiece->AddMove(glm::vec2(kingPiece->GetStartOffset().x - 2, kingPiece->GetStartOffset().y), moves);
		}
		else if (rightClear) {
			kingPiece->AddMove(glm::vec2(kingPiece->GetStartOffset().x + 2, kingPiece->GetStartOffset().y), moves);
		}
	}
	else {
		if (leftClear) {
			kingPiece->AddMove(glm::vec2(kingPiece->GetStartOffset().x + 2, kingPiece->GetStartOffset().y), moves);
		}
		else if (rightClear) {
			kingPiece->AddMove(glm::vec2(kingPiece->GetStartOffset().x - 2, kingPiece->GetStartOffset().y), moves);
		}
	}

	//rook indexs will automatically be updated back to -1 in tick if rook is not valid, since I am setting it -1 above
	return glm::vec2{ leftRookIndex, rightRookIndex };
}

bool Chess::Game::CheckCastlingClear(int rookIndex, Chess::BasePiece* kingPiece, bool leftCheck)
{

	if (currentTurn == White && rookIndex != -1) {

		if (leftCheck) {
			for (int i = rookIndex + 1; i < kingPiece->GetArrayIndex(); i++){
				if (WhitePieces[i]->GetStartOffset().y == -7) {
					std::cout << "LEFT ROOK CHECK: FALSE\n";
					return false;
				}
			}

			//check if opposing pieces have moves that block castling
			for (int i = 0; i < BlackPieces.size(); i++){
				for (int y = 0; y < BlackPieces[i]->GetMoves().size(); y++) {
					if (BlackPieces[i]->GetMoves().at(y).y == -7 && (BlackPieces[i]->GetMoves().at(y).x == 2 ||
						BlackPieces[i]->GetMoves().at(y).x == 4 || BlackPieces[i]->GetMoves().at(y).x == 3)) {
						std::cout << "Piece Blocking Long Castle";
						return false;
					}
				}
			}
		}
		else {
			for (int i = kingPiece->GetArrayIndex() + 1; i < rookIndex; i++) {
				if (WhitePieces[i]->GetStartOffset().y == -7) {
					std::cout << "RIGHT ROOK CHECK: FALSE\n";
					return false;
				}
			}

			//long castling oposing piece check
			for (int i = 0; i < BlackPieces.size(); i++) {
				for (int y = 0; y < BlackPieces[i]->GetMoves().size(); y++) {
					if (BlackPieces[i]->GetMoves().at(y).y == -7 && (BlackPieces[i]->GetMoves().at(y).x == 4 ||
						BlackPieces[i]->GetMoves().at(y).x == 5 || BlackPieces[i]->GetMoves().at(y).x == 6)) {
						std::cout << "Piece Blocking Short Castle";
						return false;
					}
				}
			}

		}

	}

	std::cout << "Passed";
	return true;
}

void Chess::Game::MakeCastlingMove(Chess::BasePiece* kingPiece, glm::vec2 mousePos, glm::vec2 rookindexes) {

	if (kingPiece->GetTypeName() != "King" || rookindexes.x == -1 && rookindexes.y == -1) [[likely]] {
		return;
	}
	
	if (currentTurn == White) {
		
	}
	else if (currentTurn == Black) {
	
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

void Chess::Game::PrintPieceMovesAtMousePos(glm::vec2 mousePosition)
{
	mousePosition = ScreenPosToOffset(glm::vec2(mousePosition.x * -1, mousePosition.y));
	mousePosition.x = -mousePosition.x - 8;

	for (int i = 0; i < WhitePieces.size(); i++)
	{
		if (WhitePieces[i]->GetStartOffset() == mousePosition) {
			std::cout << "\n\n****************************\n";
			std::cout << "--PIECE UNDER MOUSE INFORMATION-- \n";
			std::cout << "Piece Type: " << WhitePieces[i]->GetTypeName();
			std::cout << "\nPiece Color: White";
			std::cout << "\nPiece Position: " << "X - " << WhitePieces[i]->GetStartOffset().x << ", Y - " <<
				WhitePieces[i]->GetStartOffset().y;
			std::cout << "\nCurrent Stored Moves:";
			for (int y = 0; y < WhitePieces[i]->GetMoves().size(); y++)
			{
				PrintVec2Data(WhitePieces[i]->GetMoves().at(y), "Move " + std::to_string(y + 1));
			}
			std::cout << "\n--PIECE UNDER MOUSE INFORMATION--\n";
			std::cout << "\n\n****************************\n";

			return;
		}
	}

	for (int i = 0; i < BlackPieces.size(); i++)
	{
		if (BlackPieces[i]->GetStartOffset() == mousePosition) {
			std::cout << "\n\n****************************\n";
			std::cout << "--PIECE UNDER MOUSE INFORMATION-- \n";
			std::cout << "Piece Type: " << BlackPieces[i]->GetTypeName();
			std::cout << "\nPiece Color: Black";
			std::cout << "\nPiece Position: " << "X - " << BlackPieces[i]->GetStartOffset().x << ", Y - " <<
				BlackPieces[i]->GetStartOffset().y;
			std::cout << "\nCurrent Stored Moves:";
			for (int y = 0; y < BlackPieces[i]->GetMoves().size(); y++)
			{
				PrintVec2Data(BlackPieces[i]->GetMoves().at(y), "Move " + std::to_string(y + 1));
			}
			std::cout << "--PIECE UNDER MOUSE INFORMATION--\n";
			std::cout << "\n\n****************************\n";

			return;
		}
	}
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
