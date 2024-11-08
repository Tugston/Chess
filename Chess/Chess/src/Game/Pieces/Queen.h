#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class Queen : public BasePiece {
	public:

	public:

		Queen(PieceColor color, glm::vec2 offset);
		~Queen();


		void Selected();
		void DeSelected();

	private:


	};
}