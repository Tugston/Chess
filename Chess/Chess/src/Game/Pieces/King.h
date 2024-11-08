#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class King : public BasePiece {
	public:


	public:
		King(PieceColor color, glm::vec2 offset);
		~King();

		void Selected();
		void DeSelected();

	private:



	};

}