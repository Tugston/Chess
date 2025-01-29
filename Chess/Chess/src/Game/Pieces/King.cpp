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

std::string Chess::King::GetTypeName()
{
	return "King";
}

glm::vec2 Chess::King::CastleAvailable(std::vector<BasePiece*>* teamPieces, std::vector<BasePiece*>* opponentPieces,
	std::vector<glm::vec2>& boardMoves) {

	int leftRookIndex = -1;
	int rightRookIndex = -1;

	bool leftClear = false;
	bool rightClear = false;

	//cant castle after the king moves
	if (hasMoved)
		return glm::vec2(leftRookIndex, rightRookIndex);

	for (int i = 0; i < teamPieces->size(); i++) {
		if (teamPieces->at(i)->GetTypeName() == "Rook" && !teamPieces->at(i)->GetHasMoved()) {
			if (teamPieces->at(i)->GetStartOffset().x == 0) 
				leftRookIndex = teamPieces->at(i)->GetArrayIndex();
			else   //there are only 2 rooks that can castle, so if its not the first one it has to be the second
				rightRookIndex = teamPieces->at(i)->GetArrayIndex();
		}
	}

	if (rightRookIndex >= 0)
		leftClear = CheckCastlingClear(teamPieces, opponentPieces, rightRookIndex);
	if (leftRookIndex >= 0)
		rightClear = CheckCastlingClear(teamPieces, opponentPieces, leftRookIndex);
	
	//finally add the moves
	if (color == WHITE) {
		if (leftClear)
			AddMove(glm::vec2(this->startingOffset.x + 2, this->startingOffset.y), boardMoves);
		if (rightClear)
			AddMove(glm::vec2(this->startingOffset.x - 2, this->startingOffset.y), boardMoves);
	}
	else {
		if (leftClear)
			AddMove(glm::vec2(this->startingOffset.x + 2, this->startingOffset.y), boardMoves);
		if (rightClear)
			AddMove(glm::vec2(this->startingOffset.x - 2, this->startingOffset.y), boardMoves);
	}
	
	//returning the rook indexes for the sprite renderer
	return glm::vec2(leftRookIndex, rightRookIndex); 
}



int Chess::King::MakeCastleMove(std::vector<BasePiece*>& teamPieces, glm::vec2 mousePos, glm::vec2 rookIndexes) {
	
	if (rookIndexes.x == -1 && rookIndexes.y == -1)
		return -1;

	//have to do the annoying offset thing I do everywhere
	mousePos.x = -mousePos.x - 8;

	//interpolate the directions based on the team
	int leftXOffset = 3;//blackLeft + (whiteLeft - blackLeft) * color;
	int rightXOffset = 2;//blackRight + (whiteRight - blackRight) * color;
	
	if (color == BLACK)
		rookIndexes -= glm::vec2(16, 16); //offset for black

	//check if the square clicked is actually the castling square and move the rook
	if (mousePos.x - 2 == this->startingOffset.x) {

		teamPieces.at(rookIndexes.y)->SetPreCalculatedStartOffset(glm::vec2(teamPieces.at(rookIndexes.y)->GetStartOffset().x - rightXOffset,
			teamPieces.at(rookIndexes.y)->GetStartOffset().y));

		teamPieces.at(rookIndexes.y)->SetPreCalculatedOffset(glm::vec2(teamPieces.at(rookIndexes.y)->GetOffset().x - rightXOffset,
			teamPieces.at(rookIndexes.y)->GetOffset().y));

		return rookIndexes.y;
	}
	else if (mousePos.x + 2 == this->startingOffset.x) {

		teamPieces.at(rookIndexes.x)->SetPreCalculatedStartOffset(glm::vec2(teamPieces.at(rookIndexes.x)->GetStartOffset().x + leftXOffset,
			teamPieces.at(rookIndexes.x)->GetStartOffset().y));

		teamPieces.at(rookIndexes.x)->SetPreCalculatedOffset(glm::vec2(teamPieces.at(rookIndexes.x)->GetStartOffset().x + leftXOffset,
			teamPieces.at(rookIndexes.x)->GetStartOffset().y));

		return rookIndexes.x;
	}
	
}

std::vector<glm::vec2> Chess::King::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions, const std::vector<glm::vec2>& blackPiecePositions)
{
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection = std::vector<bool>{ false, false, false, false, false, false, false, false };
	const std::vector<glm::vec2>& tPos = SortTeamPositions(whitePiecePositions, blackPiecePositions, true);
	const std::vector<glm::vec2>& oPos = SortTeamPositions(whitePiecePositions, blackPiecePositions, false);

	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y + 1), positions, tPos, oPos, PieceDetection, 0);
	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y), positions, tPos, oPos, PieceDetection, 1);
	BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y - 1), positions, tPos, oPos, PieceDetection, 2);
	BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - 1), positions, tPos, oPos, PieceDetection, 3);
	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y - 1), positions, tPos, oPos, PieceDetection, 4);
	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y), positions, tPos, oPos, PieceDetection, 5);
	BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y + 1), positions, tPos, oPos, PieceDetection, 6);
	BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + 1), positions, tPos, oPos, PieceDetection, 7);

	return positions;
}

bool Chess::King::CheckCastlingClear(std::vector<BasePiece*>* teamPieces, std::vector<BasePiece*>* opponentPieces, int rookIndex) {
	
	int startingY; //the starting y pos of the team

	//add a rook offset here to keep the vector in bounds
	int rookOffset = 0;
	
	if (rookIndex == -1)
		return false;

	if (color == WHITE)
		startingY = -7;
	else {
		startingY = 0;
		rookOffset = 16; //offset the rook index to fit inside the black piece array
	}

	//left rook (long castle)
	if (rookIndex - rookOffset == 0) {

		//team pieces in the way
		for (int i = rookIndex + 1; i < this->arrayIndex; i++)
			if (teamPieces->at(i - rookOffset)->GetStartOffset().y == startingY)
				return false;
		

		//opposing piece moves, and piece positions in the way
		for (int i = 0; i < opponentPieces->size(); i++) {
			
			if (opponentPieces->at(i)->GetStartOffset().y == startingY && (opponentPieces->at(i)->GetStartOffset().x == 1 ||
				opponentPieces->at(i)->GetStartOffset().x == 2 || opponentPieces->at(i)->GetStartOffset().x == 3))
				return false;
			
			for (int y = 0; y < opponentPieces->at(i)->GetMoves().size(); y++)
				if (opponentPieces->at(i)->GetMoves().at(y).y == startingY && (opponentPieces->at(i)->GetMoves().at(y).x == 2 ||
					opponentPieces->at(i)->GetMoves().at(y).x == 3 || opponentPieces->at(i)->GetMoves().at(y).x == 4))
					return false;

		}

	}
	else { //all the same stuff but for the other rook

		for (int i = this->arrayIndex + 1; i < rookIndex; i++)
			if (teamPieces->at(i - rookOffset)->GetStartOffset().y == startingY)
				return false;

		for (int i = 0; i < opponentPieces->size(); i++) {

			if (opponentPieces->at(i)->GetStartOffset().y == startingY && (opponentPieces->at(i)->GetStartOffset().x == 5 ||
				opponentPieces->at(i)->GetStartOffset().x == 6))
				return false;

			for (int y = 0; y < opponentPieces->at(i)->GetMoves().size(); y++)
				if (opponentPieces->at(i)->GetMoves().at(y).y == startingY && (opponentPieces->at(i)->GetStartOffset().x == 4 ||
					opponentPieces->at(i)->GetStartOffset().x == 5 || opponentPieces->at(i)->GetStartOffset().x == 6))
					return false;

		}
	
	}

	return true;
}
