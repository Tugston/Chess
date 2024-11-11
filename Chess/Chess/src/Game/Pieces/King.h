#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class King : public BasePiece {
	public:


	public:
		King(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex);
		~King();

		void Selected();
		void DeSelected();

		std::vector<glm::vec2> GetAvailableMoves(glm::vec2 currentPosition);

	private:



	};

}