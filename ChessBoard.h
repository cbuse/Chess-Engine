// Zugzwang Chess Engine - stage 2

#ifndef _FUNC_H
#define _FUNC_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <cmath>
#include <utility>
#include <map>
#include <cstring>
#include "constants.h"
#include "pawn.h"
#include "auxiliary.h"
	typedef struct{
		int score;
		std::pair<int, int> positions;
	}score_max;
// Class that implements the chess board's current state and functions for
// generating moves and checking move validity.
class ChessBoard {
private:
	static board* table;
	static std::string ALPHA_NUMERIC_POSITIONS[64];
	static int initial_position,
			   final_position,
			   white_en_passant,
			   black_en_passant,
			   final_pos_opponent;
	static bool queen_flag,
				castling_possible;
	
	// Checks if king is in danger of capture.
	// Returns the position of the "attacker" or -1 if the king is not in chess.
	static int kingInDanger(bool isWhite);
	
	// Updates the fields on the pieces according to the current board state.
	static void updateBoard(void);
	
	// Move a piece across the board from initial_pos to final_pos.
	// Update the bitboards for the occupied squares, white pieces, black pieces,
	// and all the piece types, depending on the piece that moves and whether the
	// move is a capture.
	static void movePiece(int initial_pos, int final_pos);
	
	// Tries to move a piece so that the king will not be in chess.
	// Returns true is the piece was moved with success, false otherwise. 
	static bool try_moving_piece(int start, int end, bool isWhite);
	
	// Does castling for the engine, if possible.
	static bool try_castling(bool isWhite);
	
	// Tries to get the king out of chess, if he is in danger.
	static int kingIsSafe(bool isWhite);
	
	// Checks if there is a valid move for the pawn on the pos position.
	// If there are more than one, chooses randomly, moves the pawn and updates
	// final_position.
	// Returns true if a valid move exists, false if not.
	static bool generateValidPawnMove(int pos, bool isWhite);
	
	// Checks if the pawn on the pos position can attack a neighboring piece.
	// A pawn captures diagonally, one square forward and to the left or right.
	// If there are more than one possible captures, chooses randomly, moves the
	// pawn and updates final_position.
	// Returns true if a capture is possible, false if not.
	static bool generateValidPawnAttack(int pos, bool isWhite);
	
	// Chooses a random available pawn to move on the board.
	// Returns true if there is a pawn that can be moved, false otherwise.
	static bool randomPositionPawn(bool isWhite);
	
	// Generate a vector of available move positions for the king.
	static std::vector<int> generateValidKingMove(int pos, bool isWhite);
	
	// Generate a vector of available move positions for the knight.
	static std::vector<int> generateValidKnightMove(int pos, bool isWhite);
	
	// Generate a vector of available move positions for the bishop.
	static std::vector<int> generateValidBishopMove(int pos, bool isWhite);
	
	// Generate a vector of available move positions for the rook.
	static std::vector<int> generateValidRookMove(int pos, bool isWhite);
	
	// Generate a vector of available move positions for the queen.
	static std::vector<int> generateValidQueenMove(int pos, bool isWhite);
	
	
	// Helper function for checking if a piece type has any available moves.
	// Arguments are: a vector with the positions of the pieces of that type on
	// the board; a pointer to a function that generates possible moves for
	// that piece type; a bool for checking the color of the engine.
	//
	// Returns 1 if a valid move lead to victory, 0 if a simple valid move was
	// found, -1 if no valid move was found.
	static int random_piece_helper(std::vector<int> piece_positions,
								  std::vector<int> (* generate)(int, bool),
								  bool isWhite);
	
	// Interprets the en passant move from the opponent.
	static void en_passant_recognition(bool isWhite);
	
	// Interprets the castling move from the opponent.
	static bool castling_recognition(int start, int end, bool isWhite);
	
	// Checks if the engine has given checkmate to it's opponent.
	static bool winner(bool isWhite);
	
public:
	// Frees allocated memory for table.
	static void deleteTable();
	
	// Moves opponent's piece on the board.
	static void updateOpponentMove(char* positions, bool isWhite);
	
	// Initialises all the bitboards, the board of piece codes, the flags and
	// the class variables.
	static void initializeBitboard(void);
	
	// Chooses a random available piece to move on the board.
	// Returns 0 if there is a piece that can simply be moved, 1 if a piece was 
	// moved and the engine won, -1 if the engine cannot make a valid move (the
	// engine lost), 2 if the engine is in stalemate.
	static int randomPiece(bool isWhite);
	
	// Returns the algebraic notation corresponding to final_position.
	static std::string finalPosFunc(void);
	
	// Returns the algebraic notation corresponding to initial_position.
	static std::string initialPosFunc(void);
	
	static int eval_heuristic(bool isWhite);
	static score_max negaMax(int alpha, int beta, int depth, bool isWhite);
	//static int negaMax(int alpha, int beta, int depth, bool isWhite);
};

#endif

