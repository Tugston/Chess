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

		std::string GetTypeName() override;

		//returns the piece index that needs to be removed
		int enPassant(glm::vec2 lastPieceMove, std::vector<BasePiece*>* opponentPieces);

		void SetDoubleMoved(bool justDoubleMoved);
		bool GetDoubleMoved();

	protected:
		std::vector<glm::vec2> GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
			const std::vector<glm::vec2>& blackPiecePositions);

	private:
		bool justDoubleMoved;
	};

}