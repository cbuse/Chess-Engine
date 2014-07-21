// Zugzwang Chess Engine - stage 2

#ifndef _PARSER_H
#define _PARSER_H

#include <iostream>
#include <cstring>
#include "FromEngineToXboard.h"
#include "FromXboardToEngine.h"

// Class for the interpretation of Xboard commands.
class Parser{
	public:
		static void interpretCommand(void);
};

#endif
