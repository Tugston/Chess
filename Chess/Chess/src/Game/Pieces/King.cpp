#include "King.h"

Chess::King::King(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(KING, color, screenPosition, arrayIndex)
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

std::vector<glm::vec2> Chess::King::GetAvailableMoves(glm::vec2 currentPosition)
{
	return std::vector<glm::vec2>();
}
