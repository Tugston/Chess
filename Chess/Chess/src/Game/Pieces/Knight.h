#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class Knight : public BasePiece {
	public:


	public:
		Knight(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex);
		~Knight();

		void Selected();
		void DeSelected();

		std::vector<glm::vec2> GetAvailableMoves(glm::vec2 currentPosition);
	private:

	};

}