#include "BaseChessPiece.h"

#include "../../GraphicsEngine/Core/Macros.h"

Chess::BasePiece::BasePiece(const PieceType& type, const PieceColor& color, const glm::vec2& offset, const unsigned int& arrayIndex) :
	type(type), color(color), offset(offset), position(offset), arrayIndex(arrayIndex), startingOffset(offset)
{
	if (color == BLACK)
	{
		this->offset.x += 1;
		this->startingOffset.x += 1;

		//trying to figure out why the pickup picks up 1 to the right
	}
}

Chess::BasePiece::~BasePiece()
{
}

std::string Chess::BasePiece::GetTypeName()
{
	switch (this->type)
	{
	case KING:
		return "King";
	case QUEEN:
		return "Queen";
	case BISHOP:
		return "Bishop";
	case KNIGHT:
		return "Knight";
	case ROOK:
		return "Rook";
	case PAWN:
		return "Pawn";
	default:
		break;
	}

	return "";
}

void Chess::BasePiece::SetOffset(glm::vec2 offset, const bool& whiteTurn)
{
	offset.x = -offset.x - 8;
	this->offset = offset;
}

void Chess::BasePiece::SetStartOffset(glm::vec2 offset, const bool& whiteTurn)
{
	offset.x = -offset.x - 8;

	
	this->startingOffset = offset;
	//boundary = glm::vec2(offset.x + 1, offset.y + 1);
}

unsigned int Chess::BasePiece::GetArrayIndex()
{
	if (color == WHITE) {
		return arrayIndex;
	}
	else {
		return arrayIndex + 1;
	}
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


bool Chess::BasePiece::GetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage, const bool& whiteTurn,
	const std::vector<glm::vec2>& whitePiecePostions, const std::vector<glm::vec2>& blackPiecePositions)
{
	mousePosition.x = -mousePosition.x - 8;

	
	moves = this->GetAvailableMoves(startingOffset, whitePiecePostions, blackPiecePositions);
	moveGameStorage = moves;

	for (int i = 0; i < moves.size(); i++)
	{
		if (mousePosition == moves.at(i)) {
			return true;
		}
	}

	return false;
}

bool Chess::BasePiece::GetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage)
{
	mousePosition.x = -mousePosition.x - 8;
	moveGameStorage = moves;
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
	std::vector<glm::vec2> positions;
	std::vector<bool> PieceDetection; // determines if each track has hit a piece or not

	//have 0 clue how to handle en passant with pieces
	//or castling

	if (color == WHITE)
	{
		if (type == PAWN) {
			PieceDetection = std::vector<bool>{false, false, false, false};
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
			BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
			BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y + 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
			if (mousePosition.y == -6) {
				BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
			}

			return positions;
		}
	}
	else if (color == BLACK) {
		std::cout << "Black";

		if (type == PAWN) {
			PieceDetection = std::vector<bool>{ false, false, false, false};
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
			BoundsCheck(glm::vec2(mousePosition.x - 1, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
			BoundsCheck(glm::vec2(mousePosition.x + 1, mousePosition.y - 1), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
			if (mousePosition.y == -1) {
				BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - 2), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
			}
			return positions;
		}
	}

	if(type == ROOK)
	{
		PieceDetection = std::vector<bool>{ false, false, false, false };
		for (int i = 1; i < 8; i++)
		{
			
			BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
			BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
		}

		return positions;
	}
	else if (type == BISHOP)
	{
		PieceDetection = std::vector<bool>{ false, false, false, false };
		for (int i = 1; i < 8; i++)
		{
			BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
			BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
			BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
			BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
		}

		return positions;
	}
	else if (type == KNIGHT)
	{
		PieceDetection = std::vector<bool>{ false, false, false, false, false, false, false, false };
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
	else if (type == KING) {
		PieceDetection = std::vector<bool>{ false, false, false, false, false, false, false, false };
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
	else if (type == QUEEN) {
		PieceDetection = std::vector<bool>{ false, false, false, false, false, false, false, false };
		for (int i = 1; i < 8; i++)
		{
			BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 0);
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 1);
			BoundsCheck(glm::vec2(mousePosition.x, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 2);
			BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 3);
			BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 4);
			BoundsCheck(glm::vec2(mousePosition.x + i, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 5);
			BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y - i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 6);
			BoundsCheck(glm::vec2(mousePosition.x - i, mousePosition.y + i), positions, whitePiecePositions, blackPiecePositions, PieceDetection, 7);
		}
		return positions;
	}

	//return positions;
}

void Chess::BasePiece::BoundsCheck(const glm::vec2& position, std::vector<glm::vec2>& positions, const std::vector<glm::vec2>& whitePositions,
	const std::vector<glm::vec2>& blackPositions, std::vector<bool>& PieceDetections, unsigned int currentIndex)
{

	bool positionPushedBack = false;

	//check if this "branch" has already resulted in finding a piece
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
				}
			}

			for (int i = 0; i < blackPositions.size(); i++)
			{
				if (blackPositions[i] == position) {
					PieceDetections[currentIndex] = true;
					positionPushedBack = true;
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


