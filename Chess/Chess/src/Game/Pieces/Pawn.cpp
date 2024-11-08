#include "Pawn.h"

Chess::Pawn::Pawn(PieceColor color, glm::vec2 offset) : BasePiece(PAWN, color, offset)
{
	if (color == WHITE) {
		textureID = 10;
	}
	else if (color == BLACK) {
		textureID = 12;
	}
}

Chess::Pawn::~Pawn()
{
}

void Chess::Pawn::Selected()
{
}

void Chess::Pawn::DeSelected()
{
}
