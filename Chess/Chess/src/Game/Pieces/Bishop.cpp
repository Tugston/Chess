#include "Bishop.h"

#include<iostream>

Chess::Bishop::Bishop(PieceColor color, glm::vec2 offset) : BasePiece(BISHOP, color, offset)
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
