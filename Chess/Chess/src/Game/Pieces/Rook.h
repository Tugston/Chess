#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class Rook : public BasePiece{
	public:


	public:
		Rook(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex);
		~Rook();

		void Selected();
		void DeSelected();

		std::vector<glm::vec2> GetAvailableMoves(glm::vec2 currentPosition);

	private:

	};

}