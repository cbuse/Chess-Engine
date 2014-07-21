// Zugzwang Chess Engine - stage 2

#include "ChessBoard.h"

// Keeps track of the board's current start.
board* ChessBoard::table = NULL;

// Positions from and to which a piece moves.
int ChessBoard::initial_position = 0,
	ChessBoard::final_position = 0;
	
// Tells whether the queen's next moves will be generated 
bool ChessBoard::queen_flag = false,
// Tells if castling is possible
	 ChessBoard::castling_possible = true;
// Flags to check for en passant moves from the opponent
int ChessBoard::final_pos_opponent = 0,
	ChessBoard::white_en_passant = 0, 
	ChessBoard::black_en_passant = 0;

// Algebraic notation for each square of the board.
std::string ChessBoard::ALPHA_NUMERIC_POSITIONS[64] =
	{"a1","b1","c1","d1","e1","f1","g1","h1",
	"a2","b2","c2","d2","e2","f2","g2","h2",
	"a3","b3","c3","d3","e3","f3","g3","h3",
	"a4","b4","c4","d4","e4","f4","g4","h4",
	"a5","b5","c5","d5","e5","f5","g5","h5",
	"a6","b6","c6","d6","e6","f6","g6","h6",
	"a7","b7","c7","d7","e7","f7","g7","h7",
	"a8","b8","c8","d8","e8","f8","g8","h8"};
