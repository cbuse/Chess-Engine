// Zugzwang Chess Engine - stage 2

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <vector>

// The starting positions of all pieces
#define WHITE_PAWN 0x000000000000FF00ULL
#define BLACK_PAWN 0x00FF000000000000ULL
#define WHITE_ROOK 0x0000000000000081ULL
#define BLACK_ROOK 0x8100000000000000ULL
#define WHITE_KNIGHT 0x0000000000000042ULL
#define BLACK_KNIGHT 0x4200000000000000ULL
#define WHITE_BISHOP 0x0000000000000024ULL
#define BLACK_BISHOP 0x2400000000000000ULL
#define WHITE_QUEEN 0x0000000000000008ULL
#define BLACK_QUEEN 0x0800000000000000ULL
#define WHITE_KING 0x0000000000000010ULL
#define BLACK_KING 0x1000000000000000ULL

// Codes for piece types
#define WHITE_PAWN_CODE 'P'
#define BLACK_PAWN_CODE 'p'
#define WHITE_ROOK_CODE 'R'
#define BLACK_ROOK_CODE 'r'
#define WHITE_KNIGHT_CODE 'N'
#define BLACK_KNIGHT_CODE 'n'
#define WHITE_BISHOP_CODE 'B'
#define BLACK_BISHOP_CODE 'b'
#define WHITE_QUEEN_CODE 'Q'
#define BLACK_QUEEN_CODE 'q'
#define WHITE_KING_CODE 'K'
#define BLACK_KING_CODE 'k'
#define EMPTY_CODE '0'

//standard chess values for each piece
#define PAWN_VALUE 1
#define KNIGHT_VALUE 3
#define BISHOP_VALUE 3
#define ROOK_VALUE 5
#define QUEEN_VALUE 9

typedef unsigned long long BITBOARD;

// The characteristics of a piece
struct piece{
	BITBOARD nextAttacks, nextMoves; //to be implemented
	char name;
};

// The characteristics of the board
struct board{
	// Array with the codes for all pieces.
	piece pieces[64];
	BITBOARD occupied, whitePieces, blackPieces, whitePawns, blackPawns,
		 whiteRooks, blackRooks, whiteKnights, blackKnights,
		 whiteBishops, blackBishops, whiteQueens, blackQueens, whiteKing,
		 blackKing;
	std::vector<char> whiteLostPieces, blackLostPieces;
};

#endif
