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

bool Chess::BasePiece::GetMoves(glm::vec2 mousePosition, std::vector<glm::vec2>& moveGameStorage, const bool& whiteTurn)
{
	mousePosition.x = -mousePosition.x - 8;

	//if (color == BLACK) {
		//mousePosition.x = mousePosition.x - 1;
	//}
	
	std::vector<glm::vec2> moves = this->GetAvailableMoves(startingOffset);
	moveGameStorage = moves;

	for (int i = 0; i < moves.size(); i++)
	{
		if (mousePosition == moves.at(i)) {
			std::cout << "Valid Spot!";
			return true;
		}
	}

	return false;
}

std::vector<glm::vec2> Chess::BasePiece::GetAvailableMoves(const glm::vec2& mousePosition)
{
	std::vector<glm::vec2> positions;

	//have 0 clue how to handle en passant with pieces

	if (color == WHITE)
	{
		if (type == PAWN) {
			positions.push_back(glm::vec2(mousePosition.x, mousePosition.y + 1));
			positions.push_back(glm::vec2(mousePosition.x - 1, mousePosition.y + 1));
			positions.push_back(glm::vec2(mousePosition.x + 1, mousePosition.y + 1));
			if (mousePosition.y == -6) {
				positions.push_back(glm::vec2(mousePosition.x, mousePosition.y + 2));
			}
			return positions;
		}
	}
	else if (color == BLACK) {
		std::cout << "Black";

		if (type == PAWN) {
			positions.push_back(glm::vec2(mousePosition.x, mousePosition.y - 1));
			positions.push_back(glm::vec2(mousePosition.x - 1, mousePosition.y - 1));
			positions.push_back(glm::vec2(mousePosition.x + 1, mousePosition.y - 1));
			if (mousePosition.y == -1) {
				positions.push_back(glm::vec2(mousePosition.x, mousePosition.y - 2));
			}
			return positions;
		}
	}

	if(type == ROOK)
	{
		//not gonna worry about going outside the board, may have to fix this later
		for (int i = 1; i < 8; i++)
		{
			positions.push_back(glm::vec2(mousePosition.x + i, mousePosition.y));
			positions.push_back(glm::vec2(mousePosition.x, mousePosition.y + i));
			positions.push_back(glm::vec2(mousePosition.x, mousePosition.y - i));
			positions.push_back(glm::vec2(mousePosition.x - i, mousePosition.y));
		}

		return positions;
	}
	else if (type == BISHOP)
	{
		for (int i = 1; i < 8; i++)
		{
			positions.push_back(glm::vec2(mousePosition.x + i, mousePosition.y - i));
			positions.push_back(glm::vec2(mousePosition.x + i, mousePosition.y + i));
			positions.push_back(glm::vec2(mousePosition.x - i, mousePosition.y - i));
			positions.push_back(glm::vec2(mousePosition.x - i, mousePosition.y + i));
		}

		return positions;
	}
	else if (type == KNIGHT)
	{
		positions.push_back(glm::vec2(mousePosition.x - 1, mousePosition.y - 2));
		positions.push_back(glm::vec2(mousePosition.x - 2, mousePosition.y - 1));
		positions.push_back(glm::vec2(mousePosition.x + 1, mousePosition.y - 2));
		positions.push_back(glm::vec2(mousePosition.x + 2, mousePosition.y - 1));
		positions.push_back(glm::vec2(mousePosition.x + 1, mousePosition.y + 2));
		positions.push_back(glm::vec2(mousePosition.x + 2, mousePosition.y + 1));
		positions.push_back(glm::vec2(mousePosition.x - 1, mousePosition.y + 2));
		positions.push_back(glm::vec2(mousePosition.x - 2, mousePosition.y + 1));
		return positions;
	}
	else if (type == KING) {
		positions.push_back(glm::vec2(mousePosition.x + 1, mousePosition.y + 1));
		positions.push_back(glm::vec2(mousePosition.x + 1, mousePosition.y));
		positions.push_back(glm::vec2(mousePosition.x + 1, mousePosition.y - 1));
		positions.push_back(glm::vec2(mousePosition.x, mousePosition.y - 1));
		positions.push_back(glm::vec2(mousePosition.x - 1, mousePosition.y - 1));
		positions.push_back(glm::vec2(mousePosition.x - 1, mousePosition.y));
		positions.push_back(glm::vec2(mousePosition.x - 1, mousePosition.y + 1));
		positions.push_back(glm::vec2(mousePosition.x, mousePosition.y + 1));
		return positions;
	}
	else if (type == QUEEN) {
		for (int i = 1; i < 8; i++)
		{
			positions.push_back(glm::vec2(mousePosition.x + i, mousePosition.y));
			positions.push_back(glm::vec2(mousePosition.x, mousePosition.y + i));
			positions.push_back(glm::vec2(mousePosition.x, mousePosition.y - i));
			positions.push_back(glm::vec2(mousePosition.x - i, mousePosition.y));
			positions.push_back(glm::vec2(mousePosition.x + i, mousePosition.y - i));
			positions.push_back(glm::vec2(mousePosition.x + i, mousePosition.y + i));
			positions.push_back(glm::vec2(mousePosition.x - i, mousePosition.y - i));
			positions.push_back(glm::vec2(mousePosition.x - i, mousePosition.y + i));
		}
		return positions;
	}

	//return positions;
}


