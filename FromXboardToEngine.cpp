// Zugzwang Chess Engine - stage 2

#include "FromXboardToEngine.h"

// Start engine.
void FromXboardtoEngine::xboardCommand(void) {
	XBOARD_MODE = true;
}

// Initialize board, white moves, set engine to play black.
// Reset and stop clocks.
void FromXboardtoEngine::newCommand(void) {
	ChessBoard::initializeBitboard();
	WHITE_MOVING = true;
	BLACK_MOVING = false;
	WHITE_MODE = false;
	BLACK_MODE = true;
	FORCE_MODE = false;
	//whiteMove.resetTimer();
	//blackMove.resetTimer();
}

// Exit program.
void FromXboardtoEngine::quitCommand(void) {
	ChessBoard::deleteTable();
	exit(0);
}

// Set the engine to play neither color. Stop clocks.
void FromXboardtoEngine::forceCommand(void) {
	FORCE_MODE = true;
	WHITE_MODE = false;
	BLACK_MODE = false;
	//whiteMove.stopTimer();
	//blackMove.stopTimer();
}

// Play color that is on the move. Associate the engine's clock with the color
// that is on move, the opponent's clock with the color that is not on move.
// Start the engine's clock.
void FromXboardtoEngine::goCommand(void) {
	FORCE_MODE = false;
	if (WHITE_MOVING) {
		WHITE_MODE = true;
		BLACK_MODE = false;
		//whiteMove.startTimer();
	} else {
		WHITE_MODE = false;
		BLACK_MODE = true;
		//blackMove.startTimer();
	}
}

// Set White on move. Set the engine to play Black. Stop clocks.
void FromXboardtoEngine::whiteCommand(void) {
	WHITE_MOVING = true;
	BLACK_MOVING = false;
	WHITE_MODE = false;
	BLACK_MODE = true;
	//whiteMove.stopTimer();
	//blackMove.stopTimer();
}

// Set Black on move. Set the engine to play White. Stop clocks.
void FromXboardtoEngine::blackCommand(void) {
	WHITE_MOVING = false;
	BLACK_MOVING = true;
	WHITE_MODE = true;
	BLACK_MODE = false;
	//whiteMove.stopTimer();
	//blackMove.stopTimer();
}

// Update board with opponent's move. Set engine on move.
void FromXboardtoEngine::moveCommand(char* command) {
	//if (!FORCE_MODE) {
		ChessBoard::updateOpponentMove(command, WHITE_MODE);
	//}
	WHITE_MOVING = !WHITE_MOVING;
	BLACK_MOVING = !BLACK_MOVING;
}
