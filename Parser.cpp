// Zugzwang Chess Engine - stage 2

#include "Parser.h"

using namespace FromXboardtoEngine;
using namespace FromEnginetoXboard;

// interpretCommand method "listens" for commands from the Xboard and calls
// functions from the FromXboardToEngine namespaces accordingly. If a command
// expects a reply, the method calls a function from the FromEngineToXboard
// namespace to send the reply.
void Parser::interpretCommand(void){

	std::string command;

	setbuf(stdout, NULL);
	setbuf(stdin, NULL);

	while (1){
		std::cin >> command;

		if (command.compare("xboard") == 0){
			xboardCommand();
		}
		else if (command.compare("protover") == 0){
			// Set the desired features to the Xboard.
			std::cout<<"feature myname=\"Zugzwang\""<< std::endl;
			std::cout<<"feature usermove=1"<< std::endl;
			std::cout<<"feature sigint=0"<< std::endl;
			std::cout<<"feature sigterm=0"<< std::endl;
			std::cout<<"feature done=1"<< std::endl;
		}
		else if (command.compare("new") == 0){
			newCommand();
			break;
		}
	}

	while (1) {
		std::cin >> command;

		if (command.compare("quit") == 0){
			quitCommand();
		    break;
		}
		else if (command.compare("new") == 0){
			newCommand();
		}
		else if (command.compare("force") == 0){
			forceCommand();
		}
		else if (command.compare("go") == 0){
			goCommand();
			parseMovetoXboard();
		}
		else if (command.compare("white") == 0){
			whiteCommand();
		}
		else if (command.compare("black") == 0){
			blackCommand();
		}
		else if (command.compare("usermove") == 0){
			// Read the opponents move, update the board, and reply with a move,
			// if in turn and not in force mode.
			std::cin >> command;
			char* cmd = strdup(command.c_str());
			moveCommand(cmd);
			if (!FORCE_MODE && ((BLACK_MODE == BLACK_MOVING) 
					&& (WHITE_MODE == WHITE_MOVING))) {
				parseMovetoXboard();
			}
		}
	}
}
