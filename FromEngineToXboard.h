// Zugzwang Chess Engine - stage 2

#ifndef _TOXBOARD_H
#define _TOXBOARD_H

#include <stdio.h>
#include <string>
#include <iostream>
#include "Modes.h"
#include "ChessBoard.h"

using namespace Modes;

// The namespace includes functions that process and send commands to the Xboard.
namespace FromEnginetoXboard{
	void parseMove(std::string move);
	void resignCommand(void);
	void parseMovetoXboard();
	void checkmateCommand();
	void stalemateCommand();
};
#endif
