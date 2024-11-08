#include "Rook.h"

Chess::Rook::Rook(PieceColor color, glm::vec2 offset) : BasePiece(ROOK, color, offset)
{
	if (color == WHITE) {
		textureID = 11;
	}
	else {
		textureID = 13;
	}
}

Chess::Rook::~Rook()
{
}

void Chess::Rook::Selected()
{
}

void Chess::Rook::DeSelected()
{
}
