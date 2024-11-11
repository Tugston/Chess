#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class Queen : public BasePiece {
	public:

	public:

		Queen(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex);
		~Queen();


		void Selected();
		void DeSelected();

		std::vector<glm::vec2> GetAvailableMoves(glm::vec2 currentPosition);

	private:


	};
}