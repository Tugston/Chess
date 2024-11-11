#pragma once

#include "../Pieces/BaseChessPiece.h"

namespace Chess {

	class Bishop : public BasePiece {
	public:

	public:

		Bishop(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex);
		~Bishop();


		void Selected();
		void DeSelected();
		
		
	private:

	};

}