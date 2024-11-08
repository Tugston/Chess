#pragma once

#include "../../GraphicsEngine/Components/BasePiece.h"

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

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
		BasePiece(PieceType type, PieceColor color, glm::vec2 offset);
		~BasePiece();


		glm::vec2 GetOffset() { return offset; };
		PieceType GetType() { return type; };


		//returns the inherited textureID
		unsigned int GetTextureID() { return textureID; };

	protected:
		PieceType type;
		PieceColor color;

		glm::vec2 offset;
		
	};

}