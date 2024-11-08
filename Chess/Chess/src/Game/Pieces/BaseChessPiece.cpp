#include "BaseChessPiece.h"


Chess::BasePiece::BasePiece(PieceType type, PieceColor color, glm::vec2 offset) :
	type(type), color(color), offset(offset)
{
}

Chess::BasePiece::~BasePiece()
{
}
