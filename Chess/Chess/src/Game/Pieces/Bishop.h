#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class Bishop : public BasePiece {
	public:

	public:

		Bishop(PieceColor color, glm::vec2 offset);
		~Bishop();


		void Selected();
		void DeSelected();

	private:

	};

}