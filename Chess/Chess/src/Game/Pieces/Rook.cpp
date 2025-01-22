#include "Rook.h"

Chess::Rook::Rook(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(ROOK, color, screenPosition, arrayIndex)
{
	if (color == WHITE) {
		textureID = 11;
	}
	else {
		textureID = 13;
	}
}

Chess::Rook::~Rook()
{
}

void Chess::Rook::Selected()
{
}

void Chess::Rook::DeSelected()
{
}

std::string Chess::Rook::GetTypeName()
{
	return "Rook";
}

std::vector<glm::vec2> Chess::Rook::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions, const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{ false, false, false, false };
	for (int i = 1; i < 8; i++)
	{

		BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
		BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
		BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
		BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
	}

	return positions;
}
