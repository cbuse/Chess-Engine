// Zugzwang Chess Engine - stage 2

#include "pawn.h"

BITBOARD Pawn::pawnAttacks(int pos, bool isWhite) {
	BITBOARD res = 0LL;

	if (isWhite) {
		if (pos % 8 == 0) {
			res = 1ULL << (pos + 9);
		} else if (pos % 8 == 7) {
			res = 1ULL << (pos + 7);
		} else {
			res = (1ULL << (pos + 9)) | (1ULL << (pos + 7));
		}
	} else {
		if (pos % 8 == 0) {
			res = 1ULL << (pos - 7);
		} else if (pos % 8 == 7) {
			res = 1ULL << (pos - 9);
		} else {
			res = (1ULL << (pos - 9)) | (1ULL << (pos - 7));
		}
	}

	return res;
}

BITBOARD Pawn::pawnMove(int pos, bool isWhite) {
	BITBOARD res = 0ULL;
	if (isWhite) {
		res |= 1ULL << (pos + 8);
		if (8 <= pos && pos <= 15) {
			res |= 1ULL << (pos + 16);
		}
	} else {
		res |= 1ULL << (pos - 8);
		if (48 <= pos && pos <= 55) {
			res |= 1ULL << (pos - 16);
		}
	}

	return res;
}
