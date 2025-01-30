#include "Pawn.h"

#include<iostream>

Chess::Pawn::Pawn(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex) 
	: BasePiece(PAWN, color, screenPosition, arrayIndex), justDoubleMoved(false)
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

int Chess::Pawn::enPassant(glm::vec2 lastPieceMove, std::vector<BasePiece*>* opponentPieces){

	if (lastPieceMove == this->GetStartOffset()) {

	}

	return 0;
}

void Chess::Pawn::SetDoubleMoved(bool justDoubleMoved){
	this->justDoubleMoved = justDoubleMoved;
}

bool Chess::Pawn::GetDoubleMoved(){
	return justDoubleMoved;
}

std::vector<glm::vec2> Chess::Pawn::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
	const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{false, false, false, false};
	bool canMoveForward = true;

	int direction = 2 * static_cast<int>(color) - 1;

	const std::vector<glm::vec2>& tPos = SortTeamPositions(whitePiecePositions, blackPiecePositions, true);
	const std::vector<glm::vec2>& oPos = SortTeamPositions(whitePiecePositions, blackPiecePositions, false);

	//check pieces blocking forward progress
	for (int i = 0; i < tPos.size(); i++)
		if (tPos[i] == glm::vec2(mousePosition.x, mousePosition.y + direction))
			canMoveForward = false;

	//adding canMoveForward here just for the short circuit, I know it is kind of counter intuitive for the variable name
	for (int i = 0; i < oPos.size(); i++) {
		if (canMoveForward && oPos[i] == glm::vec2(mousePosition.x, mousePosition.y + direction))
			canMoveForward = false;

		//check the diagonal captures as well for opposing team
		if (oPos[i] == glm::vec2(mousePosition.x - 1, mousePosition.y + direction))
			BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y + direction), positions, tPos, oPos, PieceDetection, 0);
		if (oPos[i] == glm::vec2(mousePosition.x + 1, mousePosition.y + direction))
			BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y + direction), positions, tPos, oPos, PieceDetection, 1);
	}

	if (canMoveForward) {
		BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + direction), positions, tPos, oPos, PieceDetection, 2);

		//the double move start
		if (!hasMoved)
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + direction * 2), positions, tPos, oPos, PieceDetection, 3);
	}

	return positions;
}


