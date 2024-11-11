#include "Pawn.h"

#include<iostream>

Chess::Pawn::Pawn(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex) 
	: BasePiece(PAWN, color, screenPosition, arrayIndex)
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


