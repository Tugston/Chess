#include "BaseChessPiece.h"

#include "../../GraphicsEngine/Core/Macros.h"

Chess::BasePiece::BasePiece(const PieceType& type, const PieceColor& color, const glm::vec2& offset, const unsigned int& arrayIndex) :
	type(type), color(color), offset(offset), position(offset), arrayIndex(arrayIndex), startingOffset(offset)
{
	boundary = glm::vec2(offset.x + 1, offset.y + 1);
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

bool Chess::BasePiece::HitDetection(glm::vec2 position)
{
	//I'll try to fix this later, but the screen and board coords are all messed up so I'm just manually changing them
	//I have been at this for hours now and can't get it to work

	position.x = -position.x - 8;

	std::cout << "x: " << position.x << "y: " << position.y << std::endl;

	return position == startingOffset;
	//return (position.x <= boundary.x && position.y <= boundary.y && position.x >= startingOffset.x && position.y >= startingOffset.y);
}

