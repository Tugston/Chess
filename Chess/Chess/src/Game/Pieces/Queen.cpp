#include "Queen.h"

Chess::Queen::Queen(PieceColor color, glm::vec2 offset) : BasePiece(QUEEN, color, offset)
{
	if (color == WHITE) {
		textureID = 6;
	}
	else {
		textureID = 8;
	}
}

Chess::Queen::~Queen()
{
}

void Chess::Queen::Selected()
{
}

void Chess::Queen::DeSelected()
{
}
