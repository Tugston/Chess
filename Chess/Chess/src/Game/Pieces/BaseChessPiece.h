#pragma once

#include "../../GraphicsEngine/Components/BasePiece.h"

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

#include <string>

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
		glm::vec2 GetBoundary() { return boundary; };

		PieceType GetType() { return type; };

		std::string GetTypeName();


		//setters
		void SetOffset(glm::vec2 offset);
		void SetStartOffset(glm::vec2 offset);

		//returns the inherited textureID
		unsigned int GetTextureID() { return textureID; };

		//returns the array position this piece is at in the sprite renderer array of all sprites
		unsigned int GetArrayIndex() { return arrayIndex; };

		//determines if the position is between offset and boundary
		//returns true if within bounds
		bool HitDetection(glm::vec2 position);

	protected:

	protected:
		//drawing stuff
		PieceType type;
		PieceColor color;
		glm::vec2 offset;

		//original offset of the piece before it is moved with the mouse (just used for right clicking)
		glm::vec2 startingOffset;

		//collision bounadry for mouse detection (opposite corner of the offset position)
		glm::vec2 boundary;

		//actual board position; ie 1,1 or 3,1
		glm::vec2 position;

		unsigned int arrayIndex;
		
	};

}