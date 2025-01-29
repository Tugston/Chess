#include "BaseChessPiece.h"

#include "../../GraphicsEngine/Core/Macros.h"

Chess::BasePiece::BasePiece(const PieceType& type, const PieceColor& color, const glm::vec2& offset, const unsigned int& arrayIndex) :
	type(type), color(color), offset(offset), position(offset), arrayIndex(arrayIndex), startingOffset(offset), hasMoved(false)
{

}

Chess::BasePiece::~BasePiece()
{
	moveOffsets.clear();
	moves.clear();
	takeIndexes.clear();
}

std::string Chess::BasePiece::GetTypeName()
{
	return std::string();
}

void Chess::BasePiece::SetOffset(glm::vec2 offset)
{
	offset.x = -offset.x - 8;
	this->offset = offset;
}

void Chess::BasePiece::SetPreCalculatedOffset(glm::vec2 offset) {
	this->offset = offset;
}

void Chess::BasePiece::SetStartOffset(glm::vec2 offset)
{
	offset.x = -offset.x - 8;
	this->startingOffset = offset;
}

void Chess::BasePiece::SetPreCalculatedStartOffset(glm::vec2 offset) {
	this->startingOffset = offset;
}

void Chess::BasePiece::SetArrayIndex(unsigned int index)
{
	this->arrayIndex = index;
}

void Chess::BasePiece::SetHasMoved(bool hasMoved)
{
	this->hasMoved = hasMoved;
}

unsigned int Chess::BasePiece::GetArrayIndex()
{
	return arrayIndex;	
}

bool Chess::BasePiece::HitDetection(glm::vec2 position)
{
	position.x = -position.x - 8;
	return position == startingOffset;
}


void Chess::BasePiece::SetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage, const bool& whiteTurn,
	const std::vector<glm::vec2>& whitePiecePostions, const std::vector<glm::vec2>& blackPiecePositions)
{
	mousePosition.x = -mousePosition.x - 8;
	moves = this->GetAvailableMoves(startingOffset, whitePiecePostions, blackPiecePositions);
	moveGameStorage = moves;
}

void Chess::BasePiece::AddMove(glm::vec2 movePosition, std::vector<glm::vec2>& moveGameStorage) {
	moves.push_back(movePosition);
	moveGameStorage = moves;
}

bool Chess::BasePiece::GetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage)
{
	mousePosition.x = -mousePosition.x - 8;
	moveGameStorage = moves;

	//returns a mouse poistion check as well
	for (int i = 0; i < moves.size(); i++)
	{
		if (mousePosition == moves[i]) {
			return true;
		}
	}

	return false;
}

std::vector<glm::vec2> Chess::BasePiece::GetAvailableMoves(const glm::vec2& mousePosition, const std::vector<glm::vec2>& whitePiecePositions,
	const std::vector<glm::vec2>& blackPiecePositions)
{
	return std::vector<glm::vec2>{};
}

void Chess::BasePiece::BoundsCheck(const glm::vec2& position, std::vector<glm::vec2>& positions, const std::vector<glm::vec2>& teamPositions,
	const std::vector<glm::vec2>& opponentPositions, std::vector<bool>& PieceDetections, unsigned int currentIndex)
{

	//just tracks if the current position has already been determined to be clear
	bool positionPushedBack = false;

	if (!PieceDetections[currentIndex]) {

		//dont add position if its the same team
		for (int i = 0; i < teamPositions.size(); i++)
			if (teamPositions[i] == position) {
				PieceDetections[currentIndex] = true;
				positionPushedBack = true;
				break;
			}

		if(!positionPushedBack)
			for (int i = 0; i < opponentPositions.size(); i++)
				if (opponentPositions[i] == position) {
					PieceDetections[currentIndex] = true;
					positions.push_back(position);
					takeIndexes.push_back(i);
					positionPushedBack = true;
				}


		//if the position is just an empty space
		if (!positionPushedBack && position.x >= 0 && position.y <= 0 && position.x <= 7 && position.y >= -7)
			positions.push_back(position);

	}
	
}

std::vector<glm::vec2> Chess::BasePiece::SortTeamPositions(const std::vector<glm::vec2>& whitePositions,
	const std::vector<glm::vec2>& blackPositions, bool sameTeam) {
	if (sameTeam)
		return (color == WHITE) ? whitePositions : blackPositions;
	else
		return (color == WHITE) ? blackPositions : whitePositions;
}


