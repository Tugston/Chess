#pragma once


#define GLM_ENABLE_EXPERIMENTAL

//libs
#include<SDL.h>
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<gtx/string_cast.hpp>

//std
#include<iostream>
#include<vector>
#include<map>

//_________GAME INCLUDES________
#include "../Game/Board.h"
#include "../Game/Pieces/BaseChessPiece.h"


//_________CORE INCLUDES________
#include "../GraphicsEngine/Core/Window.h"
#include "../GraphicsEngine/Core/Shader.h"
#include "../GraphicsEngine/Core/Drawable.h"
#include "../GraphicsEngine/Core/SpriteRenderer.h"

//________Component INCLUDES_________
#include "../GraphicsEngine/Components/Camera.h"


//game class that handles everything
namespace Chess {

	class BasePiece;

	enum Turn {
		Black,
		White
	};

	class Game {
	public:

	public:
		Game();
		~Game();

		void Start();
	private:
		void Draw(Uint32 DeltaTime);
		void Tick();

	
		void SetupPieces();
		void SwapTurn();
		void CapturePiece(int x, int y);

		glm::vec2 CastleCheck(Chess::BasePiece* kingPiece);
		void MakeCastlingMove(Chess::BasePiece* kingPiece, glm::vec2 mousePos, glm::vec2 rookIndexes);


		glm::vec2 ScreenPosToOffset(const glm::vec2& screenCoords);
		glm::vec2 OffsetToScreenPos(glm::vec2 offset);

		std::vector<glm::vec2> GetPiecePositions(Turn turn);
		
		
		void DropPiece();
		void PickupPiece();
		void CancelMove();
		void MovePieceWithMouse();


		std::vector<Chess::BasePiece*>* GetTeamPieces(Turn teamPieces);
		std::vector<Chess::BasePiece*>* GetOpponentPieces(Turn teamPieces);

		void PrintPieceMovesAtMousePos(glm::vec2 mousePosition);
		void PrintVec2Data(glm::vec2 x, std::string name);

		Chess::BasePiece* CreatePiece(Chess::PieceType type, Chess::PieceColor color, const glm::vec2& pos, int index);

	private:

		//CORE
		GraphicsEngine::Window* window;

		//components
		GraphicsEngine::Camera* camera;

		Chess::Board* board;
		GraphicsEngine::SpriteRenderer* spriteRenderer;

		std::vector<GraphicsEngine::Drawable*> drawables;


		std::vector<Chess::BasePiece*> WhitePieces;
		std::vector<Chess::BasePiece*> BlackPieces;
		Chess::BasePiece* currentSelectedPiece;


		bool quit;

		Uint32 lastUpdateTime;

		Turn currentTurn;

		//all the moves a piece can go to when you pick it up
		std::vector<glm::vec2> moves;


		//mouse x and y position
		int x, y;

		//stores the rook positions for castling
		glm::vec2 currentRookIndex = glm::vec2{ -1,-1 };
	};
}