#include "Knight.h"

Chess::Knight::Knight(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(KNIGHT, color, screenPosition, arrayIndex)
{
	if (color == WHITE) {
		textureID = 4;
	}
	else {
		textureID = 14;
	}
}

Chess::Knight::~Knight()
{
}

void Chess::Knight::Selected()
{
}

void Chess::Knight::DeSelected()
{
}

std::vector<glm::vec2> Chess::Knight::GetAvailableMoves(glm::vec2 currentPosition)
{
	return std::vector<glm::vec2>();
}

std::string Chess::Knight::GetTypeName()
{
	return "Knight";
}

std::vector<glm::vec2> Chess::Knight::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
	const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{ false, false, false, false, false, false, false, false };

	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y - 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
	BoundsCheck(glm::vec2(mousePosition.x - 2, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y - 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
	BoundsCheck(glm::vec2(mousePosition.x + 2, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y + 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 4);
	BoundsCheck(glm::vec2(mousePosition.x + 2, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 5);
	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y + 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 6);
	BoundsCheck(glm::vec2(mousePosition.x - 2, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 7);

	return positions;
}
