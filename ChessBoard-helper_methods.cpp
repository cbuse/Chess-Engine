// Zugzwang Chess Engine - stage 2

// Methods: randomPieceHelper,
//			en_passant_recognition,
//			castling_recognition.

#include "ChessBoard.h"

// Helper function for checking if a piece type has any available moves.
// Arguments are: a vector with the positions of the pieces of that type on the
// board; a pointer to a function that generates possible moves for that piece
// type; a bool for checking the color of the engine.
//
// Returns 1 if a valid move lead to victory, 0 if a simple valid move was
// found, -1 if no valid move was found.
int ChessBoard::random_piece_helper(std::vector<int> piece_positions,
								  std::vector<int> (* generate)(int, bool),
								  bool isWhite) {
	int chosen_piece, next_pos;			  
	while (piece_positions.size() != 0) {
		chosen_piece = rand() % piece_positions.size();
		std::vector<int> valid_final_moves =
							generate(piece_positions[chosen_piece], isWhite);
		
		while (valid_final_moves.size() != 0) {
			next_pos = rand() % valid_final_moves.size();
			if (try_moving_piece(piece_positions[chosen_piece],
								valid_final_moves[next_pos],
								isWhite)) {
				//if (winner(isWhite))
				//	return 1;
				return 0;
			}
			valid_final_moves.erase(valid_final_moves.begin() + next_pos);
		}
		if (table->pieces[piece_positions[chosen_piece]].name == 'K' ||
			table->pieces[piece_positions[chosen_piece]].name == 'k')
			return -1;
		piece_positions.erase(piece_positions.begin() + chosen_piece);
	}
	return -1;
}

// Interprets the en passant move from the opponent.
void ChessBoard::en_passant_recognition(bool isWhite) {
	if (isWhite) {
		if (white_en_passant != 0) {
			if (final_pos_opponent == white_en_passant - 8 &&
				table->pieces[final_pos_opponent].name == 'p') {
			
				table->whiteLostPieces.push_back('P');
				table->whitePawns &= ~(1ULL << white_en_passant);
				table->whitePieces &= ~(1ULL << white_en_passant);
				table->occupied &= ~(1ULL << (white_en_passant));
				table->pieces[white_en_passant].name = EMPTY_CODE;
				table->pieces[white_en_passant].nextMoves = 0ULL;
				table->pieces[white_en_passant].nextAttacks = 0ULL;
				white_en_passant = 0;
				updateBoard();
			}
		}
	} else {
		if (black_en_passant != 0) {
			if (final_pos_opponent == black_en_passant + 8 &&
				table->pieces[final_pos_opponent].name == 'P') {
			
				table->blackLostPieces.push_back('p');
				table->blackPawns &= ~(1ULL << black_en_passant);
				table->blackPieces &= ~(1ULL << black_en_passant);
				table->occupied &= ~(1ULL << (black_en_passant));
				table->pieces[black_en_passant].name = EMPTY_CODE;
				table->pieces[black_en_passant].nextMoves = 0ULL;
				table->pieces[black_en_passant].nextAttacks = 0ULL;
				black_en_passant = 0;
				updateBoard();
			}
		}
	}
}

// Interprets the castling move from the opponent.
bool ChessBoard::castling_recognition(int start, int end, bool isWhite){
	if (!isWhite) {
		if (start == 4 && table->pieces[end].name == 'K') {
			if (end == 6) {
				movePiece(7, 5);
				return true;
			} else if (end == 2) {
				movePiece(0, 3);
				return true;	
			}
		}
	} else {
		if (start == 60 && table->pieces[end].name == 'k') {
			if (end == 62) {
				movePiece(63, 61);
				return true;
			} else if (end == 58) {
				movePiece(56, 59);
				return true;	
			}
		}
	}
	return false;
}

// Checks if the engine has given checkmate to it's opponent.
/*bool ChessBoard::winner(bool isWhite) {
	board *backup = new board();
	*backup = *table;
	int backup_init = initial_position, backup_fin = final_position;
	bool ret = (randomPiece(!isWhite) == -1) ? true : false;
	*table = *backup;
	backup->whiteLostPieces.clear();
	backup->blackLostPieces.clear();
	delete(backup);
	initial_position = backup_init;
	final_position = backup_fin;
	return ret;
}*/
