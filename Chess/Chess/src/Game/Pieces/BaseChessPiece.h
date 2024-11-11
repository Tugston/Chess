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

		std::string GetTypeName();


		//setters
		void SetOffset(glm::vec2 offset, const bool& whiteTurn);
		void SetStartOffset(glm::vec2 offset, const bool& whiteTurn);

		//returns the inherited textureID
		unsigned int GetTextureID() { return textureID; };

		//returns the array position this piece is at in the sprite renderer array of all sprites
		unsigned int GetArrayIndex();

		//determines if the position is between offset and boundary
		//returns true if within bounds
		bool HitDetection(glm::vec2 position, const bool& whiteTurn);

		//Get all move location
		//returns all the positions to be determined later; ie capturing, same team blocking, and what not
		bool GetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage, const bool& whiteTurn);

	

	private:

		//returns all possible moves for each piece
		std::vector<glm::vec2> GetAvailableMoves(const glm::vec2& mousePosition);

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
		
	};

}