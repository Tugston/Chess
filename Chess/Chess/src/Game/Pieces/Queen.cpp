#include "Queen.h"

Chess::Queen::Queen(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(QUEEN, color, screenPosition, arrayIndex)
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

std::vector<glm::vec2> Chess::Queen::GetAvailableMoves(glm::vec2 currentPosition)
{
	return std::vector<glm::vec2>();
}
