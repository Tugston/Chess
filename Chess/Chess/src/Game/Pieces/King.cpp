#include "King.h"

Chess::King::King(PieceColor color, glm::vec2 offset) : BasePiece(KING, color, offset)
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
