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

		std::string GetTypeName() override;

		glm::vec2 CastleAvailable(std::vector<BasePiece*>* teamPieces, std::vector<BasePiece*>* opponentPieces,
			std::vector<glm::vec2>& boardMoves);

		//Adjust the rooks offset
		//rookIndexes.x = left rook; rookIndexes.y = right
		//returns the rook index that moved
		int MakeCastleMove(std::vector<BasePiece*>& teamPieces, glm::vec2 mousePos, glm::vec2 rookIndexes);

	protected:
		std::vector<glm::vec2> GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
			const std::vector<glm::vec2>& blackPiecePositions) override;


	private:

		//determine if there are team pieces blocking castle
		//as well as oposing pieces move locations and poisitons
		bool CheckCastlingClear(std::vector<BasePiece*>* teamPieces, std::vector<BasePiece*>* opponentPieces, int rookIndex);

	};

}