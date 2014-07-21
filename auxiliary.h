// Zugzwang Chess Engine - stage 2

#ifndef AUXILIARY_H_
#define AUXILIARY_H_

#include <string>
#include <vector>
#include "constants.h"

// Auxiliary functions.
namespace Auxiliary {
	// Returns a string consisting of the bits of the argument "value", sorted from
	// least significant to most significant.
	// For debugging purposes.
	std::string convertToBitString(BITBOARD value);
	
	// Returns a vector consisting of the set bits of the argumeny "x".
	std::vector<int> getOneBits(BITBOARD x);
};

#endif
