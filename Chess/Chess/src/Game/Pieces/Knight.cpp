#include "Knight.h"

Chess::Knight::Knight(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(KNIGHT, color, screenPosition, arrayIndex)
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

std::vector<glm::vec2> Chess::Knight::GetAvailableMoves(glm::vec2 currentPosition)
{
	return std::vector<glm::vec2>();
}
