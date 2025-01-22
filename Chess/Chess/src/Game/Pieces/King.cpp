#include "King.h"

Chess::King::King(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(KING, color, screenPosition, arrayIndex)
{
	if (color == WHITE) {
		textureID = 7;
	}
	else {
		textureID = 9;
	}
}

Chess::King::~King()
{
}

void Chess::King::Selected()
{
}

void Chess::King::DeSelected()
{
}

std::vector<glm::vec2> Chess::King::GetAvailableMoves(glm::vec2 currentPosition)
{
	return std::vector<glm::vec2>();
}

std::string Chess::King::GetTypeName()
{
	return "King";
}

std::vector<glm::vec2> Chess::King::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions, const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{ false, false, false, false, false, false, false, false };

	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
	BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 4);
	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 5);
	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 6);
	BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 7);

	return positions;
}
