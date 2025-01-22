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

void Chess::BasePiece::SetStartOffset(glm::vec2 offset)
{
	offset.x = -offset.x - 8;

	
	this->startingOffset = offset;
	//boundary = glm::vec2(offset.x + 1, offset.y + 1);
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

bool Chess::BasePiece::HitDetection(glm::vec2 position, const bool& whiteTurn)
{
	//I'll try to fix this later, but the screen and board coords are all messed up so I'm just manually changing them
	//I have been at this for hours now and can't get it to work
	position.x = -position.x - 8;

	std::cout << "x: " << position.x << "y: " << position.y << std::endl;

	return position == startingOffset;
	//return (position.x <= boundary.x && position.y <= boundary.y && position.x >= startingOffset.x && position.y >= startingOffset.y);
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

	//extra mouse position check (fixes some bugs that happen every now and then)
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

void Chess::BasePiece::BoundsCheck(const glm::vec2& position, std::vector<glm::vec2>& positions, const std::vector<glm::vec2>& whitePositions,
	const std::vector<glm::vec2>& blackPositions, std::vector<bool>& PieceDetections, unsigned int currentIndex)
{
	//just tracks if the current position has already been determined to be clear
	bool positionPushedBack = false;

	//check if this branch has already resulted in finding a piece
	if (!PieceDetections[currentIndex]) {
		if (color == WHITE) {
			//if white piece loop has same spot, then don't push back the position, and set the PieceDetection index to true
			//if black piece loop has same spot, then push back that position, and set the PieceDetection index to true

			//loop through all the white pieces
			for (int i = 0; i < whitePositions.size(); i++) {
				if (whitePositions[i] == position) {
					PieceDetections[currentIndex] = true;
					positionPushedBack = true;
					break;
				}
			}

			//loop through all the black pieces
			if (!positionPushedBack) {
				for (int i = 0; i < blackPositions.size(); i++) {
					if (blackPositions[i] == position) {
						PieceDetections[currentIndex] = true;
						positions.push_back(position);
						positionPushedBack = true;
						takeIndexes.push_back(i);
						break;
					}
				}
			}


		}
		else {
			// do the same for black but flipped

			for (int i = 0; i < whitePositions.size(); i++) {
				if (whitePositions[i] == position) {
					PieceDetections[currentIndex] = true;
					positions.push_back(position);
					positionPushedBack = true;
					takeIndexes.push_back(i);
					break;
				}
			}

			if (!positionPushedBack) {
				for (int i = 0; i < blackPositions.size(); i++)
				{
					if (blackPositions[i] == position) {
						PieceDetections[currentIndex] = true;
						positionPushedBack = true;
						break;
					}
				}
			}
		}



		//if the position is just an empty space
		if (!positionPushedBack && position.x >= 0 && position.y <= 0 && position.x <= 7 && position.y >= -7)
		{
			positions.push_back(position);
		}
	}
}


