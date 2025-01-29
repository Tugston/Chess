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

Game::Game() : quit(false), lastUpdateTime(0), currentTurn(Turn::White), x(0), y(0){

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
	for (unsigned int i = 0; i < WhitePieces.size(); i++) {
		std::vector<glm::vec2> tempStorage;
		WhitePieces[i]->SetMoves(WhitePieces[i]->GetStartOffset(), tempStorage, true, GetPiecePositions(White), GetPiecePositions(Black));
	}
	for (unsigned int i = 0; i < BlackPieces.size(); i++) {
		std::vector<glm::vec2> tempStorage;
		BlackPieces[i]->SetMoves(WhitePieces[i]->GetStartOffset(), tempStorage, false, GetPiecePositions(White), GetPiecePositions(Black));
	}
	

	Tick();
}

void Game::Draw(Uint32 DeltaTime)
{

	glClearColor(.48f, .85f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (unsigned int i = 0; i < drawables.size(); i++)
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

					PrintPieceMovesAtMousePos(glm::vec2(x, y));

					if (currentSelectedPiece)
					{
						DropPiece();
					}
					else {
						PickupPiece();
					}
				}
				else {
					CancelMove();
				}
				break;
			default:
				break;
			}
		}

		MovePieceWithMouse();


		Draw(DeltaTime);

		if (DeltaTime >= 1)
			lastUpdateTime = currentTime;
	}

	
}

void Chess::Game::SetupPieces()
{
	//location offsets
	float xOffset = 800;
	
	const int WhiteYOffset = 700;
	const int BlackYOffset = 0;

	std::vector<PieceType> pieceOrder = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };


	//setup back rank pieces
	for (int i = 0; i < 8; i++) {
		WhitePieces.push_back(CreatePiece(pieceOrder[i], WHITE, glm::vec2(xOffset, WhiteYOffset), i));
		BlackPieces.push_back(CreatePiece(pieceOrder[i], BLACK, glm::vec2(xOffset, BlackYOffset), i + 16));
		xOffset += 100;
	}

	xOffset = 800;

	//setup pawns
	for (int i = 0; i < 8; i++) {
		WhitePieces.push_back(CreatePiece(PAWN, WHITE, glm::vec2(xOffset, WhiteYOffset - 100), i + 8));
		BlackPieces.push_back(CreatePiece(PAWN, BLACK, glm::vec2(xOffset, BlackYOffset + 100), i + 24));
		xOffset += 100;
	}

	
	//setup the renderer
	//doing 2 loops to make all the white pieces be added first, then all the black pieces, instead of alternating them in 1 loop
	for (size_t i = 0; i < WhitePieces.size(); i++) {
		spriteRenderer->AddSpriteData(WhitePieces.at(i)->GetOffset());
		spriteRenderer->AddSpriteID(WhitePieces.at(i)->GetTextureID());
	}

	for (size_t i = 0; i < BlackPieces.size(); i++) {
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
	glm::vec2 mousePos = ScreenPosToOffset(glm::vec2(x * -1, y));
	mousePos.x = -mousePos.x - 8;

	if (!currentSelectedPiece)
		return;
	
	for(size_t i = 0; i < currentSelectedPiece->GetTakeIndexes().size(); i++){

		if (GetOpponentPieces(currentTurn)->at(currentSelectedPiece->GetTakeIndexes().at(i))->GetStartOffset() == mousePos) {

			spriteRenderer->RemoveSprite(GetOpponentPieces(currentTurn)->at(currentSelectedPiece->GetTakeIndexes().at(i))->GetArrayIndex());

			//set all piece indexes after this current piece -= 1
			for (size_t y = currentSelectedPiece->GetTakeIndexes().at(i); y < GetOpponentPieces(currentTurn)->size(); y++)
				GetOpponentPieces(currentTurn)->at(y)->SetArrayIndex(GetOpponentPieces(currentTurn)->at(y)->GetArrayIndex() - 1);

			//black pieces are created after white, so if a white piece is removed all black pieces need to update as well
			if (currentTurn == Black)
				for (BasePiece* i : BlackPieces)
					i->SetArrayIndex(i->GetArrayIndex() - 1);

			//remove the piece and cleanup
			const int removeIndex = currentSelectedPiece->GetTakeIndexes().at(i);
			delete GetOpponentPieces(currentTurn)->at(removeIndex);
			GetOpponentPieces(currentTurn)->erase(GetOpponentPieces(currentTurn)->begin() + removeIndex);
			break;
		}
	} 
}


glm::vec2 Chess::Game::CastleCheck(Chess::BasePiece* kingPiece)
{
	if (kingPiece && kingPiece->GetTypeName() == "King")
		if (currentTurn == White)
			return dynamic_cast<King*>(kingPiece)->CastleAvailable(&WhitePieces, &BlackPieces, moves);
		else
			return dynamic_cast<King*>(kingPiece)->CastleAvailable(&BlackPieces, &WhitePieces, moves);
	else
		return glm::vec2(-1, -1); //just return -1, -1 if the cast fails, the castling move can handle it (shouldnt ever fail though)
}

void Chess::Game::MakeCastlingMove(Chess::BasePiece* kingPiece, glm::vec2 mousePos, glm::vec2 rookindexes) {

	int pieceIndex = -1;
	
	if (kingPiece->GetTypeName() == "King") {											
		pieceIndex = dynamic_cast<King*>(kingPiece)->MakeCastleMove(*GetTeamPieces(currentTurn),
			ScreenPosToOffset(glm::vec2(mousePos.x * -1, mousePos.y)), rookindexes);

		if (pieceIndex > -1) {

			if (kingPiece->GetColor() == BLACK)
				spriteRenderer->MoveSpriteInstance(GetTeamPieces(currentTurn)->at(pieceIndex)->GetStartOffset(), pieceIndex + 16);
			else
				spriteRenderer->MoveSpriteInstance(GetTeamPieces(currentTurn)->at(pieceIndex)->GetStartOffset(), pieceIndex);
		}
	}
}

glm::vec2 Chess::Game::ScreenPosToOffset(const glm::vec2& screenCoords)
{
	int cellSize = SCREENSIZE / 8;

	float x = (SCREENSIZE - screenCoords.x) / cellSize;
	float y = screenCoords.y / cellSize;

	//x = (8 - 1) - x;

	y *= -1;
	x *= -1;
	return glm::vec2((int)x, (int)y);
}

glm::vec2 Chess::Game::OffsetToScreenPos(glm::vec2 offset)
{
	int cellSize = SCREENSIZE / 8;

	offset.x *= -1;
	offset.y *= -1;

	float x = SCREENSIZE - (offset.x - 1) * cellSize;
	float y = offset.y * cellSize;

	return glm::vec2((int)x, (int)y);
}

std::vector<glm::vec2> Chess::Game::GetPiecePositions(Turn turn)
{
	std::vector<glm::vec2> x;

	for (size_t i = 0; i < GetTeamPieces(turn)->size(); i++)
		x.push_back(GetTeamPieces(turn)->at(i)->GetStartOffset());

	return x;
}

void Chess::Game::DropPiece() {

	bool canMove = currentSelectedPiece->GetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves);

	if (!canMove)
		return;

	MakeCastlingMove(currentSelectedPiece, glm::vec2(x, y), currentRookIndex);

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

void Chess::Game::PickupPiece() {

	//assigns piece according to turn
	for (size_t i = 0; i < GetTeamPieces(currentTurn)->size(); i++)
		if (GetTeamPieces(currentTurn)->at(i)->HitDetection(ScreenPosToOffset(glm::vec2(x * -1, y))))
			currentSelectedPiece = GetTeamPieces(currentTurn)->at(i);

	if(currentSelectedPiece) // may fail, whatever
		currentSelectedPiece->SetMoves(ScreenPosToOffset(glm::vec2(x * -1, y)), moves, true,
			GetPiecePositions(White), GetPiecePositions(Black));

	
	currentRookIndex = CastleCheck(currentSelectedPiece);
}

void Chess::Game::CancelMove() {

	if (currentSelectedPiece) {
		currentSelectedPiece->SetOffset(currentSelectedPiece->GetStartOffset());
		moves.clear();
		board->DisplayMoves(moves);
		spriteRenderer->MoveSpriteInstance(currentSelectedPiece->GetStartOffset(),
			currentSelectedPiece->GetArrayIndex());
		currentSelectedPiece = nullptr;
	}
}

void Chess::Game::MovePieceWithMouse() {
	if (currentSelectedPiece) {
		board->DisplayMoves(moves);
		SDL_GetMouseState(&x, &y);
		glm::vec2 movePosition = ScreenPosToOffset(glm::vec2(x, y));
		movePosition.x += 7;
		spriteRenderer->MoveSpriteInstance(movePosition, currentSelectedPiece->GetArrayIndex());
	}
}

std::vector<Chess::BasePiece*>* Chess::Game::GetTeamPieces(Turn teamPieces) {
	std::vector<Chess::BasePiece*>* allPieces[] = { &BlackPieces, &WhitePieces };
	return allPieces[static_cast<int>(teamPieces)];
}

std::vector<Chess::BasePiece*>* Chess::Game::GetOpponentPieces(Turn teamPieces) {
	std::vector<Chess::BasePiece*>* allPieces[] = { &BlackPieces, &WhitePieces };
	return allPieces[1 - static_cast<int>(teamPieces)];
}

void Chess::Game::PrintPieceMovesAtMousePos(glm::vec2 mousePosition)
{
	mousePosition = ScreenPosToOffset(glm::vec2(mousePosition.x * -1, mousePosition.y));
	mousePosition.x = -mousePosition.x - 8;

	for (size_t i = 0; i < WhitePieces.size(); i++)
	{
		if (WhitePieces[i]->GetStartOffset() == mousePosition) {
			std::cout << "\n\n****************************\n";
			std::cout << "--PIECE UNDER MOUSE INFORMATION-- \n";
			std::cout << "Piece Type: " << WhitePieces[i]->GetTypeName();
			std::cout << "\nPiece Color: White";
			std::cout << "\nPiece Position: " << "X - " << WhitePieces[i]->GetStartOffset().x << ", Y - " <<
				WhitePieces[i]->GetStartOffset().y;
			std::cout << "\nPiece Index: " << WhitePieces[i]->GetArrayIndex();
			std::cout << "\nCurrent Stored Moves:";
			for (size_t y = 0; y < WhitePieces[i]->GetMoves().size(); y++)
			{
				PrintVec2Data(WhitePieces[i]->GetMoves().at(y), "Move " + std::to_string(y + 1));
			}
			std::cout << "\n--PIECE UNDER MOUSE INFORMATION--\n";
			std::cout << "\n\n****************************\n";

			return;
		}
	}

	for (size_t i = 0; i < BlackPieces.size(); i++)
	{
		if (BlackPieces[i]->GetStartOffset() == mousePosition) {
			std::cout << "\n\n****************************\n";
			std::cout << "--PIECE UNDER MOUSE INFORMATION-- \n";
			std::cout << "Piece Type: " << BlackPieces[i]->GetTypeName();
			std::cout << "\nPiece Color: Black";
			std::cout << "\nPiece Index: " << BlackPieces[i]->GetArrayIndex();
			std::cout << "\nPiece Position: " << "X - " << BlackPieces[i]->GetStartOffset().x << ", Y - " <<
				BlackPieces[i]->GetStartOffset().y;
			std::cout << "\nCurrent Stored Moves:";
			for (size_t y = 0; y < BlackPieces[i]->GetMoves().size(); y++)
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
	for (size_t i = 0; i < name.length(); i++)
	{
		std::cout << "-";
	}

	std::cout << "\n" << name << ": \n";

	std::cout << "X: " << x.x << "\n";
	std::cout << "Y: " << x.y << "\n";

	for (size_t i = 0; i < name.length(); i++)
	{
		std::cout << "-";
	}
	
	std::cout << "\n\n";
}

Chess::BasePiece* Chess::Game::CreatePiece(Chess::PieceType type, Chess::PieceColor color, const glm::vec2& pos, int index) {
	
	switch (type) {
	case Chess::PieceType::BISHOP: return new Bishop(color, ScreenPosToOffset(pos), index);
	case Chess::PieceType::KING: return new King(color, ScreenPosToOffset(pos), index);
	case Chess::PieceType::KNIGHT: return new Knight(color, ScreenPosToOffset(pos), index);
	case Chess::PieceType::PAWN: return new Pawn(color, ScreenPosToOffset(pos), index);
	case Chess::PieceType::QUEEN: return new Queen(color, ScreenPosToOffset(pos), index);
	case Chess::PieceType::ROOK: return new Rook(color, ScreenPosToOffset(pos), index);
	default: return nullptr;
	}
}
