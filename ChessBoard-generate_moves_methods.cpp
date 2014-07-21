// Zugzwang Chess Engine - stage 2

// Methods: try_moving_piece,
//			try_castling,
//			kingIsSafe,
//			generateValidPawnMove, 
//			generateValidPawnAttack,
// 			randomPositionPawn, 
//			generateValidQueenMove, 
//			generateValidKingMove,
//			generateValidKingMove, 
//			generateValidBishopMove,
//			generateValidRookMove.

#include "ChessBoard.h"
#include <climits>

using namespace Auxiliary;
using namespace std;
// Tries to move a piece so that the king will not be in chess.
// Returns true is the piece was moved with success, false otherwise. 
bool ChessBoard::try_moving_piece(int start, int end, bool isWhite) {
    board* backup = new board();
    *backup = *table;

    movePiece(start, end);
    if(kingInDanger(isWhite) == -1) {
    	initial_position = start;
    	final_position = end;
    	backup->whiteLostPieces.clear();
		backup->blackLostPieces.clear();
        delete(backup);        
        return true;
    }
    *table = *backup;
    backup->whiteLostPieces.clear();
	backup->blackLostPieces.clear();
    delete(backup); 
    return false;
}

// Does castling for the engine, if possible.
bool ChessBoard::try_castling(bool isWhite) {
	if (castling_possible) {
		if (isWhite) {
			if (table->pieces[1].name == EMPTY_CODE &&
				table->pieces[2].name == EMPTY_CODE &&
				table->pieces[3].name == EMPTY_CODE) {
				if (try_moving_piece(4, 2, isWhite)) {
					movePiece(0, 3);
					castling_possible = false;
					return true;
				}
			}
			if (table->pieces[5].name == EMPTY_CODE &&
				table->pieces[6].name == EMPTY_CODE) {
				if (try_moving_piece(4, 6, isWhite)) {
					movePiece(7, 5);
					castling_possible = false;
					return true;
				}
			}
		} else {
			if (table->pieces[57].name == EMPTY_CODE &&
				table->pieces[58].name == EMPTY_CODE &&
				table->pieces[59].name == EMPTY_CODE) {
				if (try_moving_piece(60, 58, isWhite)) {
					movePiece(56, 59);
					castling_possible = false;
					return true;
				}
			}
			if (table->pieces[61].name == EMPTY_CODE &&
				table->pieces[62].name == EMPTY_CODE) {
				if (try_moving_piece(60, 62, isWhite)) {
					movePiece(63, 61);
					castling_possible = false;
					return true;
				}
			}
		}
	}
	return false;
}

// Tries to get the king out of chess, if he is in danger.
int ChessBoard::kingIsSafe(bool isWhite) {
	int attacker = kingInDanger(isWhite);
	if (attacker != -1) {
		// Looks for another piece to remove the "attacker".
		std::vector<int> v;
		size_t i;
		if (isWhite) {
			v = getOneBits(table->whitePieces);
		}
		else {
			v = getOneBits(table->blackPieces);
		}
		for (i = 0; i < v.size(); ++i) {
			if (table->pieces[v[i]].nextAttacks & (1ULL << attacker)) {
				if (try_moving_piece(v[i], attacker, isWhite)) {
					return 1;
				}
			}
		}

		//Tries to move the king somehow.
		
		std::vector<int> king_position, valid_final_moves;
		if(isWhite)
			king_position = getOneBits(table->whiteKing);
		else
			king_position = getOneBits(table->blackKing);

		valid_final_moves = generateValidKingMove(king_position[0], isWhite);

		while (valid_final_moves.size() != 0) {
			int final_king = rand() % valid_final_moves.size();
			if(try_moving_piece(king_position[0],
								valid_final_moves[final_king],
								isWhite)) {
				castling_possible = false;
				return 1;
			}
			valid_final_moves.erase(valid_final_moves.begin() + final_king);
		}

		// Tries to find another piece to get in the way of the attacker with
		// another piece.
		if ((table->pieces[attacker].name != 'n' &&
			table->pieces[attacker].name != 'p') ||
			(table->pieces[attacker].name != 'N' &&
			table->pieces[attacker].name != 'P')) {

			int file_k = king_position[0] % 8,
				rank_k = king_position[0] / 8,
				file_a = attacker % 8,
				rank_a = attacker / 8;
			int dif_file = file_a - file_k,
				dif_rank = rank_a - rank_k;
			
			int step;
			BITBOARD mask = 0ULL;
			
			if (file_k == file_a) {
				step = (rank_k > rank_a) ? 8 : -8;
			} else if (rank_k == rank_a){
				step = (file_k > file_a) ? 1 : -1;
			} else {
				if (dif_rank > 0 && dif_file > 0)
					step = 7;
				else if (dif_rank < 0 && dif_file < 0)
					step = -9;
				else if (dif_rank > 0 && dif_file < 0)
					step = -7;
				else
					step = 9;
			}
			
			// Builds a mask with the path of the attacker to the king.
			i = attacker + step;
			while (i != king_position[0]){ //&& i >= 0 && i < 64) {
				mask |= 1ULL << i;
				i += step;
			}

			std::vector<int> aux;

			// Gather all pieces.
			if (isWhite) {
				v = getOneBits(table->whitePawns);
				aux = getOneBits(table->whiteBishops);
				v.insert(v.end(), aux.begin(), aux.end());
				aux = getOneBits(table->whiteRooks);
				v.insert(v.end(), aux.begin(), aux.end());
				aux = getOneBits(table->whiteKnights);
				v.insert(v.end(), aux.begin(), aux.end());
				aux = getOneBits(table->whiteQueens);
				v.insert(v.end(), aux.begin(), aux.end());
			}
			else {
				v = getOneBits(table->blackPawns);
				aux = getOneBits(table->blackBishops);
				v.insert(v.end(), aux.begin(), aux.end());
				aux = getOneBits(table->blackRooks);
				v.insert(v.end(), aux.begin(), aux.end());
				aux = getOneBits(table->blackKnights);
				v.insert(v.end(), aux.begin(), aux.end());
				aux = getOneBits(table->blackQueens);
				v.insert(v.end(), aux.begin(), aux.end());
			}

			// Finds the intersection of a piece's path with the attacker's and
			// tries to move the piece.
			for (i = 0; i < v.size(); ++i) {
				std::vector<int> intersect = 
							getOneBits(table->pieces[v[i]].nextMoves &mask);
				if (intersect.size() != 0) {
					if(try_moving_piece(v[i], intersect[0], isWhite)) { 
						return 1;
					}
				}
			}
		}
		return -1;
	}
	return 0;
}

// Checks if there is a valid move for the pawn on the pos position.
// If there are more than one, chooses randomly, moves the pawn and updates
// final_position.
// Returns true if a valid move exists, false if not.
bool ChessBoard::generateValidPawnMove(int pos, bool isWhite){
	bool b1 = false;
	bool b2 = false;

	if (isWhite){
		// If playing white, check ahead 8 positions in array of codes, and
		// check ahead 16 positions if pawn is in start position (second row)
		// for existing pieces.
		if	(table->pieces[pos + 8].name == EMPTY_CODE) {
			b2 = true;
			if ((15 >= pos) && (pos >= 8) &&
				(table->pieces[pos + 16].name == EMPTY_CODE)){
				b1 = true;
			}
		}
		if (b2){
			if (b1) {
				if(rand() % 2 == 0){
					if (try_moving_piece(pos, pos + 8, isWhite)){
						return true;
					}
				}
				else{
					if (try_moving_piece(pos, pos + 16, isWhite)) {
						white_en_passant = final_position;
						return true;
					}
				}
			}
			else {
				if (try_moving_piece(pos, pos + 8, isWhite)) {
					return true;
				}
			}
		}
	}
	else{
		// If playing black, check behind 8 positions in array of codes, and
		// check behind 16 positions if pawn is in start position (second to
		// last row) for existing pieces.
		if (table->pieces[pos - 8].name == EMPTY_CODE) {
			b2 = true;
			if ((55 >= pos) && (pos >= 48) &&
				(table->pieces[pos - 16].name == EMPTY_CODE)){
				b1 = true;
			}
		}
		if (b2){
			if (b1) {
				if(rand() % 2 == 0){
					if (try_moving_piece(pos, pos - 8, isWhite)) {
						return true;
					}
				}
				else{
					if (try_moving_piece(pos, pos - 16, isWhite)) {
						black_en_passant = final_position;
						return true;
					}
				}
			}
			else {
				if (try_moving_piece(pos, pos - 8, isWhite)) {
					return true;
				}
			}
		}
	}
	return false;
}

// Checks if the pawn on the pos position can attack a neighboring piece.
// A pawn captures diagonally, one square forward and to the left or right.
// If there are more than one possible captures, chooses randomly, moves the
// pawn and updates final_position.
// Returns true if a capture is possible, false if not.
bool ChessBoard::generateValidPawnAttack(int pos, bool isWhite){
	bool b1 = false;
	bool b2 = false;

	if (isWhite){
		// If playing white and pawn is on the margin of the board, checks ahead
		// in the array of codes for existing pieces, depending on which margin
		// the pawn is on.
		if (pos % 8 == 0){
			if (table->pieces[pos + 9].name == BLACK_PAWN_CODE |
				table->pieces[pos + 9].name == BLACK_ROOK_CODE |
				table->pieces[pos + 9].name == BLACK_KNIGHT_CODE |
				table->pieces[pos + 9].name == BLACK_BISHOP_CODE |
				table->pieces[pos + 9].name == BLACK_QUEEN_CODE ){

				if (try_moving_piece(pos, pos + 9, isWhite)) {
					return true;
				}
			}
			return false;
		}
		else if (pos % 8 == 7){
			if (table->pieces[pos + 7].name == BLACK_PAWN_CODE |
				table->pieces[pos + 7].name == BLACK_ROOK_CODE |
				table->pieces[pos + 7].name == BLACK_KNIGHT_CODE |
				table->pieces[pos + 7].name == BLACK_BISHOP_CODE |
				table->pieces[pos + 7].name == BLACK_QUEEN_CODE ){

				if (try_moving_piece(pos, pos + 7, isWhite)) {
					return true;
				}
			}
			return false;
		}
		else{
			// If playing white and pawn is not on the margin of the board,
			// checks ahead for existing pieces in the array of codes.
			if (table->pieces[pos + 7].name == BLACK_PAWN_CODE |
				table->pieces[pos + 7].name == BLACK_ROOK_CODE |
				table->pieces[pos + 7].name == BLACK_KNIGHT_CODE |
				table->pieces[pos + 7].name == BLACK_BISHOP_CODE |
				table->pieces[pos + 7].name == BLACK_QUEEN_CODE ) {
				b1 = true;
			}
			if (table->pieces[pos + 9].name == BLACK_PAWN_CODE |
				table->pieces[pos + 9].name == BLACK_ROOK_CODE |
				table->pieces[pos + 9].name == BLACK_KNIGHT_CODE |
				table->pieces[pos + 9].name == BLACK_BISHOP_CODE |
				table->pieces[pos + 9].name == BLACK_QUEEN_CODE ){
				b2 = true;
			}
			if (b1 && b2){
				if (rand() % 2 == 0){
					if (try_moving_piece(pos, pos + 9, isWhite)) {
						return true;
					}
				}
				else{
					if (try_moving_piece(pos, pos + 7, isWhite)) {
						return true;
					}
				}
			}
			else if (b1){
				if (try_moving_piece(pos, pos + 7, isWhite)) {
					return true;
				}
			}
			else if (b2){
				if (try_moving_piece(pos, pos + 9, isWhite)) {
					return true;
				}
			}
			return false;
		}
	}
	else{
		// If playing black and pawn is on the margin of the board, checks
		// behind in the array of codes for existing pieces, depending on
		// which margin the pawn is on.
		if (pos % 8 == 0){
			if (table->pieces[pos - 7].name == WHITE_PAWN_CODE |
				table->pieces[pos - 7].name == WHITE_ROOK_CODE |
				table->pieces[pos - 7].name == WHITE_KNIGHT_CODE |
				table->pieces[pos - 7].name == WHITE_BISHOP_CODE |
				table->pieces[pos - 7].name == WHITE_QUEEN_CODE ){

				if (try_moving_piece(pos, pos - 7, isWhite)) {
					return true;
				}
			}
			return false;
		}
		else if (pos % 8 == 7) {
			if (table->pieces[pos - 9].name == WHITE_PAWN_CODE |
				table->pieces[pos - 9].name == WHITE_ROOK_CODE |
				table->pieces[pos - 9].name == WHITE_KNIGHT_CODE |
				table->pieces[pos - 9].name == WHITE_BISHOP_CODE |
				table->pieces[pos - 9].name == WHITE_QUEEN_CODE ){

				if (try_moving_piece(pos, pos - 9, isWhite)) {
					return true;
				}
			}
			return false;
		}
		else{
			// If playing white and pawn is not on the margin of the board,
			// checks behind for existing pieces in the array of codes.
			if (table->pieces[pos - 7].name == WHITE_PAWN_CODE |
				table->pieces[pos - 7].name == WHITE_ROOK_CODE |
				table->pieces[pos - 7].name == WHITE_KNIGHT_CODE |
				table->pieces[pos - 7].name == WHITE_BISHOP_CODE |
				table->pieces[pos - 7].name == WHITE_QUEEN_CODE ) {
				b1 = true;
			}
			if (table->pieces[pos - 9].name == WHITE_PAWN_CODE |
				table->pieces[pos - 9].name == WHITE_ROOK_CODE |
				table->pieces[pos - 9].name == WHITE_KNIGHT_CODE |
				table->pieces[pos - 9].name == WHITE_BISHOP_CODE |
				table->pieces[pos - 9].name == WHITE_QUEEN_CODE ){
				b2 = true;
			}
			if (b1 && b2){
				if(rand() % 2 == 0){
					if (try_moving_piece(pos, pos - 9, isWhite)) {
						return true;
					}
				}
				else{
					if (try_moving_piece(pos, pos - 7, isWhite)) {
						return true;
					}
				}
			}
			else if (b1){
				if (try_moving_piece(pos, pos - 7, isWhite)) {
					return true;
				}
			}
			else if (b2){
				if (try_moving_piece(pos, pos - 9, isWhite)) {
					return true;
				}
			}
			return false;
		}
	}
}


// Chooses a random available pawn to move on the board.
// Returns true if there is a pawn that can be moved, false otherwise.
bool ChessBoard::randomPositionPawn(bool isWhite){
	std::vector<int> v;
	int p;

	// Keeps track of all available pawns, depending on the color of the engine
	// in the vector v.
	if (isWhite)
		v = getOneBits(table->whitePawns);
	else
		v = getOneBits(table->blackPawns);

	// Checks pawns for possible captures and moves. When one is found,
	// update initial_position.
	while (v.size() > 0) {
		p = rand() % v.size();
		if (generateValidPawnAttack(v[p], isWhite)){
			return true;
		}
		if (generateValidPawnMove(v[p], isWhite)){
			return true;
		}
		v.erase(v.begin() + p);
	}
	return false;
}

// Generate a vector of available move positions for the queen.
std::vector<int> ChessBoard::generateValidQueenMove(int pos, bool isWhite) {
	std::vector<int> available_positions;
	
	queen_flag = true;
	// Use moves from the rook and bishop.
    available_positions = generateValidBishopMove(pos, isWhite);
	std::vector<int> aux_vector = generateValidRookMove(pos, isWhite);
    available_positions.insert(available_positions.end(),
    						   aux_vector.begin(),
    						   aux_vector.end());
    queen_flag = false;
 
    return available_positions;
}
int ChessBoard::eval_heuristic(bool isWhite){

	int rank = 0, mobility = 0;
	BITBOARD opposite_pieces;
	
	if(isWhite)
		opposite_pieces = table->blackPieces;
	else opposite_pieces = table->whitePieces;
	
	std::vector<int> positions = getOneBits(opposite_pieces);
	
	for(int i = 0; i< positions.size(); ++i){
			if(table->pieces[positions[i]].name == 'Q' ||
				table->pieces[positions[i]].name == 'q'){
				rank += QUEEN_VALUE;
				mobility += getOneBits(table->pieces[positions[i]].nextMoves | table->pieces[positions[i]].nextAttacks).size();
			}
			else if(table->pieces[positions[i]].name == 'K' ||
				table->pieces[positions[i]].name == 'K'){
				rank += KNIGHT_VALUE;
				mobility += getOneBits(table->pieces[positions[i]].nextMoves | table->pieces[positions[i]].nextAttacks).size();
			}
			else if(table->pieces[positions[i]].name == 'B' ||
				table->pieces[positions[i]].name == 'b'){
				rank += BISHOP_VALUE;
				mobility += getOneBits(table->pieces[positions[i]].nextMoves | table->pieces[positions[i]].nextAttacks).size();
			}
			else if(table->pieces[positions[i]].name == 'R' ||
				table->pieces[positions[i]].name == 'r'){
				rank += ROOK_VALUE;
				mobility += getOneBits(table->pieces[positions[i]].nextMoves | table->pieces[positions[i]].nextAttacks).size();
			}
			else if(table->pieces[positions[i]].name == 'P' ||
				table->pieces[positions[i]].name == 'p'){
				rank += PAWN_VALUE;
				mobility += getOneBits(table->pieces[positions[i]].nextMoves | table->pieces[positions[i]].nextAttacks).size();
			}
		
		
	}
	
	return rank + mobility/10;
}

	
score_max ChessBoard::negaMax(int alpha, int beta, int depth, bool isWhite){
	std::map<int, std::vector<int> > legal_moves;
	std::vector<int> v, temp;
	std::vector<int> aux;
	std::cout<<"\nINTRARAAM!!!!!!!!!!!!!!!!!!!!"<<std::endl;

	score_max max;
	max.score = INT_MAX; 
	score_max Score;
	
	
	if (isWhite) {
			v = getOneBits(table->whitePieces);
		}
	else {
			v = getOneBits(table->blackPieces);
		}
	for (int i = 0; i < v.size(); ++i) {
		 
		 temp = getOneBits(table->pieces[v[i]].nextAttacks | table->pieces[v[i]].nextMoves);
		 aux.insert(aux.end(), temp.begin(), temp.end());
		 legal_moves.insert(make_pair(v[i], aux)); 	
		 std::cout<<"\nPentru "<< table->pieces[v[i]].name << std::endl;
		 
		 for(int j = 0; j< v.size(); j++){
				std::cout<<"\n"<<v[j]<<std::endl;
			}
	}
		
	if(legal_moves.size() == 0){
		//daca regele e in pericol 
		int p = kingIsSafe(isWhite);
		score_max s;
		s.score = p;
		s.positions = std::make_pair(-1, -1);
		return s;
	}
	//for (int i = 0; i< legal_moves.size(); ++i){
		//it = legal_moves.find()
	 for (std::map<int,vector<int> >::iterator it=legal_moves.begin(); it!=legal_moves.end(); ++it){
		//std::cout << it->first << " => " << it->second << '\n';
		for (std::vector<int>::iterator ii = it->second.begin() ; ii != it->second.end(); ++ii){
			//std::cout << ' ' << *it;
		//for(int j = 0; j< it->second.size(); j++){
			board* backup = new board();
			*backup = *table;
			//TODO de facut miscarea 
			movePiece(it->first, *ii);	
			
			if(depth == 0){
				Score.score = eval_heuristic(isWhite);
				Score.positions = make_pair(it->first, *ii);
			}
		else {
			//score = -negaMax(-beta, -alpha, depth-1, isWhite);
			score_max Score = negaMax(-beta, -alpha, depth-1, isWhite);
			Score.score = -Score.score;
			
		}
		
		*table = *backup;
		delete(backup);
		
		if(isWhite && Score.score > beta)
			break;
			
		if(!isWhite && Score.score < alpha)
			break;
			
		if(isWhite && Score.score >= alpha)
			alpha = Score.score;
		if(!isWhite && Score.score <= beta)
			beta = Score.score;
	
		if(Score.score > max.score){
			max.score = Score.score;
			max.positions = Score.positions;
			/*if(depth == MAX_DEPTH){
				bestMove = legal_moves[i];
			}*/
		}
	}
	}
	return max;
}

// Generate a vector of available move positions for the knight.
std::vector<int> ChessBoard::generateValidKingMove(int pos, bool isWhite){
	int offset [] = { 9 , 8, 7, 1, -1, -7, -8, -9};
	std::vector<int> available_positions;
	int final_pos;
	table->pieces[pos].nextMoves = 0ULL;
	
	if (isWhite) {
		for (int i = 0; i < 8; ++i) {
			final_pos = pos + offset[i];
			
			if (final_pos * (63 - final_pos) >=0 &&
				abs(pos % 8 - final_pos % 8) <= 2) {
				
				if (table->pieces[final_pos].name == EMPTY_CODE ||
					table->pieces[final_pos].name > 'a') {
					
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
			}
		}
		table->pieces[pos].nextAttacks = table->pieces[pos].nextMoves &
										 table->blackPieces;
	}
	else{
		for(int i = 0; i < 8; ++i){
			final_pos = pos + offset[i];
			
			if (final_pos * (63 - final_pos) >=0 &&
				abs(pos % 8 - final_pos % 8) <= 2) {
				
				if (table->pieces[final_pos].name == EMPTY_CODE ||
					table->pieces[final_pos].name < 'a') {
					
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
			}
		}
		table->pieces[pos].nextAttacks = table->pieces[pos].nextMoves &
										 table->whitePieces;
	}
	table->pieces[pos].nextMoves &= ~table->occupied;
    return available_positions;
}

// Generate a vector of available move positions for the knight.
std::vector<int> ChessBoard::generateValidKnightMove(int pos, bool isWhite){
	int offset [] = { 10 , 17, 15, 6, -10, -17, -15, -6};
	std::vector<int> available_positions;
	int final_pos, color;
	table->pieces[pos].nextAttacks = 0ULL;
	table->pieces[pos].nextMoves = 0ULL;
	
	if (isWhite) {
		for (int i = 0; i < 8; ++i) {
			final_pos = pos + offset[i];
			if (final_pos * ( 63 - final_pos) >=0 &&
				abs(pos % 8 - final_pos % 8) <= 2) {
				if (table->pieces[final_pos].name == EMPTY_CODE ||
					table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
			}
		}
        table->pieces[pos].nextAttacks = table->pieces[pos].nextMoves &
        								 table->blackPieces;
	} else {
		for (int i = 0; i < 8; i++) {
			final_pos = pos + offset[i];
			if (final_pos * (63 - final_pos) >=0 &&
				abs(pos % 8 - final_pos % 8) <= 2) {
				if (table->pieces[final_pos].name == EMPTY_CODE ||
					table->pieces[final_pos].name < 'a') {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
			}
		}
        table->pieces[pos].nextAttacks = table->pieces[pos].nextMoves &
        								 table->whitePieces;
	}

    table->pieces[pos].nextMoves &= ~table->occupied;

    return available_positions;
}

// Generate a vector of available move positions for the bishop.
std::vector<int> ChessBoard::generateValidBishopMove(int pos, bool isWhite) {
	std::vector<int> available_positions;
	int final_pos;
    table->pieces[pos].nextAttacks = 0ULL;
    table->pieces[pos].nextMoves = 0ULL;
	
	if (isWhite) {
		for (int i = 1; i < 8 - pos % 8; ++i) {
			final_pos = pos + 9 * i;
            if (table->pieces[final_pos].name != EMPTY_CODE &&
            	table->pieces[final_pos].name < 'a')
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		for (int i = 1; i <= pos % 8; ++i) {
			final_pos = pos - 9 * i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name < 'a'))
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 7) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i <= pos % 8; ++i) {
			final_pos = pos + 7 * i;
			if (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name < 'a')
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 7  ) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}

		for (int i = 1; i < 8 - pos % 8; ++i) {
			final_pos = pos - 7 * i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name < 'a'))
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
	} else {
		for (int i = 1; i < 8 - pos % 8; ++i) {
			final_pos = pos + 9 * i;
			if (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a')
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i <= pos % 8; ++i) {
			final_pos = pos - 9 * i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a'))
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 7) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i <= pos % 8; ++i) {
			final_pos = pos + 7 * i;
			if (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a')
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 7) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i < 8 - pos % 8; ++i) {
			final_pos = pos - 7 * i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a'))
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
	}
	return available_positions;
}

// // Generate a vector of available move positions for the rook.
std::vector<int> ChessBoard::generateValidRookMove(int pos, bool isWhite){
	std::vector<int> available_positions;

	int final_pos;
	if (!queen_flag) {
        table->pieces[pos].nextAttacks = 0ULL;
        table->pieces[pos].nextMoves = 0ULL;
    }
    
	if (isWhite) {
		for (int i = 1; i < 8 - pos % 8; ++i) {
            final_pos = pos + i;
			if (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name < 'a')
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i < 8 - pos / 8; ++i) {
			final_pos = pos + 8 * i;
			if (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name < 'a')
				break;
			if (final_pos * (63 - final_pos) >=0 && final_pos / 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		for (int i = 1; i <= pos / 8; ++i) {
			final_pos = pos - 8 * i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&	
				table->pieces[final_pos].name < 'a'))
				break;
			if (final_pos * (63 - final_pos) >=0 && final_pos /8 != 7) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}

		for (int i = 1; i <= pos % 8; ++i) {
			final_pos = pos - i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name < 'a'))
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 7) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				}
				if (table->pieces[final_pos].name > 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
	} else {
		for (int i = 1; i < 8 - pos / 8; ++i) {
			final_pos = pos + 8 * i;
			if (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a')
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a'){
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i <= pos / 8; ++i) {
			final_pos = pos - 8 * i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a'))
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos / 8 != 7) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i <= 8 - pos % 8; ++i) {
			final_pos = pos + i;
			if (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a')
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 0) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a') {
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
		
		for (int i = 1; i <= pos % 8; ++i) {
			final_pos = pos - i;
			if (final_pos < 0 || (table->pieces[final_pos].name != EMPTY_CODE &&
				table->pieces[final_pos].name > 'a'))
				break;
			if (final_pos * (63 - final_pos) >= 0 && final_pos % 8 != 7) {
				if (table->pieces[final_pos].name == EMPTY_CODE) {
					available_positions.push_back(final_pos);
                    table->pieces[pos].nextMoves |= 1ULL << final_pos;
				} else
				if (table->pieces[final_pos].name < 'a'){
					available_positions.push_back(final_pos);
					table->pieces[pos].nextAttacks |= 1ULL << final_pos;
					break;
				}
			}
		}
	}
    return available_positions;
}
