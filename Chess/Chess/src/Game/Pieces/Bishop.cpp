#include "Bishop.h"

#include<iostream>

Chess::Bishop::Bishop(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(BISHOP, color, screenPosition, arrayIndex)
{
	if (color == WHITE) {
		textureID = 5;
	}
	else {
		textureID = 15;
	}
}

Chess::Bishop::~Bishop()
{
}

void Chess::Bishop::Selected()
{
}

void Chess::Bishop::DeSelected()
{
}

std::string Chess::Bishop::GetTypeName()
{
	return "Bishop";
}

std::vector<glm::vec2> Chess::Bishop::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
	const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{ false, false, false, false }; // determines if each track has hit a piece or not

	for (int i = 1; i < 8; i++)
	{
		BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
		BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
		BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
		BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
	}

	return positions;
}



