// Zugzwang Chess Engine - stage 2

// Methods: kingInDanger, 
//			movePiece,
//			updateBoard.

#include "ChessBoard.h"

using namespace Pawn;
using namespace Auxiliary;

// Checks if king is in danger of capture.
// Returns the position of the "attacker" or -1 if the king is not in chess.
int ChessBoard::kingInDanger(bool isWhite) {
	std::vector<int> v;
	size_t i;
	if (isWhite) {
		v = getOneBits(table->blackPieces);
		for (i = 0; i < v.size(); ++i) {
			if (table->pieces[v[i]].nextAttacks & table->whiteKing) {
				return v[i];
			}
		}
	}
	else {
		v = getOneBits(table->whitePieces);
		for (i = 0; i < v.size(); ++i) {
			if (table->pieces[v[i]].nextAttacks & table->blackKing) {
				return v[i];
			}
		}
	}
	return -1;
}

// Move a piece across the board from initial_pos to final_pos.
// Update the bitboards for the occupied squares, white pieces, black pieces,
// and all the piece types, depending on the piece that moves and whether the
// move is a capture.
void ChessBoard::movePiece(int initial_pos, int final_pos) {

	// Set the bit for the initial position to 0.
	table->occupied &= ~(1ULL << (initial_pos));

	// Swap squares.
	char aux = table->pieces[initial_pos].name;
	table->pieces[initial_pos].name = table->pieces[final_pos].name;
	table->pieces[final_pos].name = aux;

	// Check for capture. If there was a piece on the final position (which is
	// now on the final position) the move is a capture, and the bitboards
	// corresponding to the captures piece are updated.
	if (table->pieces[initial_pos].name != EMPTY_CODE) {
		if (table->pieces[initial_pos].name < 'a') {
			switch (table->pieces[initial_pos].name){
			case 'P':
				table->whitePawns &= ~(1ULL << final_pos);
				break;
			case 'R':
				table->whiteRooks &= ~(1ULL << final_pos);
				break;
			case 'N':
				table->whiteKnights &= ~(1ULL << final_pos);
				break;
			case 'B':
				table->whiteBishops &= ~(1ULL << final_pos);
				break;
			case 'Q':
				table->whiteQueens &= ~(1ULL << final_pos);
				break;
			}
			table->whitePieces &= ~(1ULL << final_pos);
			table->whiteLostPieces.push_back(table->pieces[initial_pos].name);
		}else {
			switch(table->pieces[initial_pos].name) {
			case 'p':
				table->blackPawns &= ~(1ULL << final_pos);
				break;
			case 'r':
				table->blackRooks &= ~(1ULL << final_pos);
				break;
			case 'n':
				table->blackKnights &= ~(1ULL << final_pos);
				break;
			case 'b':
				table->blackBishops &= ~(1ULL << final_pos);
				break;
			case 'q':
				table->blackQueens &= ~(1ULL << final_pos);
				break;
			}
			table->blackPieces &= ~(1ULL << final_pos);
			table->blackLostPieces.push_back(table->pieces[initial_pos].name);
		}
		table->pieces[initial_pos].name = EMPTY_CODE;
		table->pieces[initial_pos].nextMoves = 0ULL;
		table->pieces[initial_pos].nextAttacks = 0ULL;
	}

	// If the move is not a capture, then update the bitboard for occupied
	// squares.
	//else {
		table->occupied |= 1ULL << (final_pos);
	//}

	// Update the bitboards corresponding to the moving piece.
	if (table->pieces[final_pos].name < 'a') {
		switch (table->pieces[final_pos].name){
		case 'P':
			table->whitePawns &= ~(1ULL << initial_pos);
			// white pawn promotion
			if (56 <= final_pos && final_pos <= 63) {
				table->whiteQueens |= 1ULL << final_pos;
				table->pieces[final_pos].name = 'Q';
			} else {
				table->whitePawns |= 1ULL << final_pos;
			}
			break;
		case 'R':
			table->whiteRooks &= ~(1ULL << initial_pos);
			table->whiteRooks |= 1ULL << final_pos;
			break;
		case 'N':
			table->whiteKnights &= ~(1ULL << initial_pos);
			table->whiteKnights |= 1ULL << final_pos;
			break;
		case 'B':
			table->whiteBishops &= ~(1ULL << initial_pos);
			table->whiteBishops |= 1ULL << final_pos;
			break;
		case 'Q':
			table->whiteQueens &= ~(1ULL << initial_pos);
			table->whiteQueens |= 1ULL << final_pos;
			break;
		case 'K':
			table->whiteKing = 1ULL << final_pos;
			break;
		}
		table->whitePieces &= ~(1ULL << initial_pos);
		table->whitePieces |= 1ULL << final_pos;
	}else {
		switch(table->pieces[final_pos].name) {
		case 'p':
			table->blackPawns &= ~(1ULL << initial_pos);
			// black pawn promotion
			if (0 <= final_pos && final_pos <= 7) {
				table->blackQueens |= 1ULL << final_pos;
				table->pieces[final_pos].name = 'q';
			} else {
				table->blackPawns |= 1ULL << final_pos;
			}
			break;
		case 'r':
			table->blackRooks &= ~(1ULL << initial_pos);
			table->blackRooks |= 1ULL << final_pos;
			break;
		case 'n':
			table->blackKnights &= ~(1ULL << initial_pos);
			table->blackKnights |= 1ULL << final_pos;
			break;
		case 'b':
			table->blackBishops &= ~(1ULL << initial_pos);
			table->blackBishops |= 1ULL << final_pos;
			break;
		case 'q':
			table->blackQueens &= ~(1ULL << initial_pos);
			table->blackQueens |= 1ULL << final_pos;
			break;
		case 'k':
			table->blackKing = 1ULL << final_pos;
			break;
		}
		table->blackPieces &= ~(1ULL << initial_pos);
		table->blackPieces |= 1ULL << final_pos;
	}

	updateBoard();
}

// Updates the fields on the pieces according to the current board state.
void ChessBoard::updateBoard(){
	for (int i = 0; i < 64; ++i) {
		switch (table->pieces[i].name) {
		case 'P':
			if (table->pieces[i + 8].name == EMPTY_CODE){
				table->pieces[i].nextMoves = pawnMove(i, true) & ~table->occupied;
			}else{
				table->pieces[i].nextMoves = 0ULL;
			}
			table->pieces[i].nextAttacks = pawnAttacks(i, true) & table->blackPieces;
			break;
		case 'p':
			if (table->pieces[i - 8].name == EMPTY_CODE){
				table->pieces[i].nextMoves = pawnMove(i, false) & ~table->occupied;
			}else{
				table->pieces[i].nextMoves = 0ULL;
			}
			table->pieces[i].nextAttacks = pawnAttacks(i, false) & table->whitePieces;
			break;
		case 'R':
            generateValidRookMove(i, true);
			break;
		case 'r':
            generateValidRookMove(i, false);
			break;
		case 'B':
			generateValidBishopMove(i, true);
			break;
		case 'b':
			generateValidBishopMove(i, false);
			break;
		case 'N':
            generateValidKnightMove(i, true);
			break;
		case 'n':
            generateValidKnightMove(i, false);
			break;
		case 'Q':
            generateValidQueenMove(i, true);
			break;
		case 'q':
            generateValidQueenMove(i, false);
			break;
		case 'K':
            generateValidKingMove(i, true);
			break;
		case 'k':
            generateValidKingMove(i, false);
			break;
		}
	}
}

