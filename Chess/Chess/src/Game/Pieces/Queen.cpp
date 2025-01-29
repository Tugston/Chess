#include "Queen.h"

Chess::Queen::Queen(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(QUEEN, color, screenPosition, arrayIndex)
{
	if (color == WHITE) {
		textureID = 6;
	}
	else {
		textureID = 8;
	}
}

Chess::Queen::~Queen()
{
}

void Chess::Queen::Selected()
{
}

void Chess::Queen::DeSelected()
{
}

std::string Chess::Queen::GetTypeName()
{
	return "Queen";
}

std::vector<glm::vec2> Chess::Queen::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
	const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{ false, false, false, false, false, false, false, false };

	const std::vector<glm::vec2>& tPos = SortTeamPositions(whitePiecePositions, blackPiecePositions, true);
	const std::vector<glm::vec2>& oPos = SortTeamPositions(whitePiecePositions, blackPiecePositions, false);

	for (int i = 1; i < 8; i++)
	{
		BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y), positions, tPos, oPos, PieceDetection, 0);
		BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + i), positions, tPos, oPos, PieceDetection, 1);
		BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - i), positions, tPos, oPos, PieceDetection, 2);
		BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y), positions, tPos, oPos, PieceDetection, 3);
		BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y - i), positions, tPos, oPos, PieceDetection, 4);
		BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y + i), positions, tPos, oPos, PieceDetection, 5);
		BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y - i), positions, tPos, oPos, PieceDetection, 6);
		BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y + i), positions, tPos, oPos, PieceDetection, 7);
	}

	return positions;
}
