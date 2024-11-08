#include "Knight.h"

Chess::Knight::Knight(PieceColor color, glm::vec2 offset) : BasePiece(KNIGHT, color, offset)
{
	if (color == WHITE) {
		textureID = 4;
	}
	else {
		textureID = 14;
	}
}

Chess::Knight::~Knight()
{
}

void Chess::Knight::Selected()
{
}

void Chess::Knight::DeSelected()
{
}
