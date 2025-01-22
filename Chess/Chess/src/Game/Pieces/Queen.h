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

		std::string GetTypeName();

	protected:
		std::vector<glm::vec2> GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
			const std::vector<glm::vec2>& blackPiecePositions) override;


	};
}