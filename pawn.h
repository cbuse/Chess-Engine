// Zugzwang Chess Engine - stage 2

#ifndef PAWN_H_
#define PAWN_H_

#include "constants.h"

// Functions for opperations with pawns.
namespace Pawn {
	// Returns possible attacks for the pawn on position pos, with the color
	// given by isWhite.
	BITBOARD pawnAttacks(int pos, bool isWhite);
	
	// Returns possible moves for the pawn on position pos, with the color
	// given by isWhite.
	BITBOARD pawnMove(int pos, bool isWhite);
}

#endif
