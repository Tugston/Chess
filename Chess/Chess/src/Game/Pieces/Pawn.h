#pragma once

#include"../Pieces/BaseChessPiece.h"

namespace Chess {

	class Pawn : public BasePiece {
	public:

	public:
		Pawn(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex);
		~Pawn();

		void Selected();
		void DeSelected();

	private:

	};

}