#include "Rook.h"

Chess::Rook::Rook(PieceColor color, glm::vec2 screenPosition, const unsigned int& arrayIndex)
	: BasePiece(ROOK, color, screenPosition, arrayIndex)
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

std::vector<glm::vec2> Chess::Rook::GetAvailableMoves(glm::vec2 currentPosition)
{
	return std::vector<glm::vec2>();
}
