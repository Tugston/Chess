#include "Pawn.h"

#include<iostream>

Chess::Pawn::Pawn(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex) 
	: BasePiece(PAWN, color, screenPosition, arrayIndex)
{
	if (color == WHITE) {
		textureID = 10;
	}
	else if (color == BLACK) {
		textureID = 12;
	}
}

Chess::Pawn::~Pawn()
{
}

void Chess::Pawn::Selected()
{
}

void Chess::Pawn::DeSelected()
{
}

std::string Chess::Pawn::GetTypeName()
{
	return "Pawn";
}

std::vector<glm::vec2> Chess::Pawn::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
	const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{ false, false, false, false };
	bool canMoveForward = true;

	if (color == WHITE) {

		//check moves for blocking
		for (int i = 0; i < blackPiecePositions.size(); i++)
		{
			if (blackPiecePositions[i] == glm::vec2(mousePosition.x, mousePosition.y + 1)) {
				canMoveForward = false;
			}


			//left diagonal capture
			if (blackPiecePositions[i] == glm::vec2(mousePosition.x - 1, mousePosition.y + 1)) {
				BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
			}

			//right diagonal capture
			if (blackPiecePositions[i] == glm::vec2(mousePosition.x + 1, mousePosition.y + 1)) {
				BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
			}
		}

		//double move at start
		if (canMoveForward) {
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);

			if (mousePosition.y == -6) {
				BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
			}
		}

		return positions;

	}
	else if (color == BLACK) {

		for (int i = 0; i < whitePiecePositions.size(); i++)
		{
			if (whitePiecePositions[i] == glm::vec2(mousePosition.x - 1, mousePosition.y - 1)) {
				BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
			}

			if (whitePiecePositions[i] == glm::vec2(mousePosition.x + 1, mousePosition.y - 1)) {
				BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
			}
		}

		if (canMoveForward) {
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);

			if (mousePosition.y == -1) {
				BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
			}
		}

		return positions;

	}

	//make compiler happy, cause "NoT aLl PaThS rEtUrN a VaLuE!!!!"
	return std::vector<glm::vec2>();
}


