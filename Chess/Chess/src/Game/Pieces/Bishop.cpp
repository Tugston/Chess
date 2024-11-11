#include "Bishop.h"

#include<iostream>

Chess::Bishop::Bishop(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(BISHOP, color, screenPosition, arrayIndex)
{
	if (color == WHITE) {
		textureID = 5;
	}
	else {
		textureID = 15;
	}
}

Chess::Bishop::~Bishop()
{
}

void Chess::Bishop::Selected()
{
}

void Chess::Bishop::DeSelected()
{
}


