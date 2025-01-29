#pragma once

#include "../../GraphicsEngine/Components/BasePiece.h"

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

#include <string>
#include <vector>

namespace Chess {

	enum PieceColor {
		BLACK,
		WHITE
	};
	enum PieceType {
		PAWN,
		ROOK,
		KNIGHT,
		BISHOP,
		KING,
		QUEEN
	};

	class BasePiece : public GraphicsEngine::BasePiece {
	
	public:
		BasePiece(const PieceType& type, const PieceColor& color, const glm::vec2& offset, const unsigned int& arrayIndex);
		~BasePiece();


		//getters
		glm::vec2 GetOffset() { return offset; };
		glm::vec2 GetStartOffset() { return startingOffset; };
		

		PieceType GetType() { return type; };

		bool GetHasMoved() { return hasMoved; };

		virtual std::string GetTypeName();


		//setters
		void SetOffset(glm::vec2 offset);
		void SetPreCalculatedOffset(glm::vec2 offset);
		void SetStartOffset(glm::vec2 offset);
		void SetPreCalculatedStartOffset(glm::vec2 offset);
		void SetArrayIndex(unsigned int index);
		void SetHasMoved(bool hasMoved);

		//returns the inherited textureID
		unsigned int GetTextureID() { return textureID; };

		//returns based on all sprites
		unsigned int GetArrayIndex();

		std::vector<unsigned int> GetTakeIndexes() { return takeIndexes; };

		//determines if poistion is the same as this piece start offset
		bool HitDetection(glm::vec2 position);

		
		void SetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage, const bool& whiteTurn,
			const std::vector<glm::vec2>& whitePiecePostions, const std::vector<glm::vec2>& blackPiecePositions);

		void AddMove(glm::vec2 movePosition, std::vector<glm::vec2>& moveGameStorage);

		//Get moves with mouse position check
		bool GetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage);

		//get all possible already existing move locations
		//disregards mouse checks
		std::vector<glm::vec2> GetMoves() { return moves; };

		
		PieceColor GetColor() { return color; };

	protected:
		//returns all possible moves for each piece
		virtual std::vector<glm::vec2> GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
			const std::vector<glm::vec2>& blackPiecePositions);

		

		//checks to see if the position is logical within the board
		void BoundsCheck(const glm::vec2& position, std::vector<glm::vec2>& positions, const std::vector<glm::vec2>& teamPositions,
			const std::vector<glm::vec2>& opponentPositions, std::vector<bool>& PieceDetections, unsigned int currentIndex);

		//helper function for the bounds check to recevie the correct teams
		//bool same team flag; true = same team positions, false = opposing team positions
		std::vector<glm::vec2> SortTeamPositions(const std::vector<glm::vec2>& whitePositions, const std::vector<glm::vec2>& blackPositions, bool sameTeam);

	protected:
		//drawing stuff
		PieceType type;
		PieceColor color;
		glm::vec2 offset;

		//original offset of the piece before it is moved with the mouse (just used for right clicking)
		glm::vec2 startingOffset;

		//actual board position; ie 1,1 or 3,1
		glm::vec2 position;

		unsigned int arrayIndex;

		//storing the actual offset of the moves rather than the actual board position or rather the gl offset value
		std::vector<glm::vec2> moveOffsets;
		
		//store the moves of each piece for the overload GetMoves function that doesn't need to calculate anything
		std::vector<glm::vec2> moves;

		//list of all the index's for opposing pieces
		std::vector<unsigned int> takeIndexes;

		//tracks moves for castling and en-passant
		bool hasMoved;
	};

}