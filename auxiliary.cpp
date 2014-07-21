// Zugzwang Chess Engine - stage 2

#include "auxiliary.h"

std::string Auxiliary::convertToBitString(BITBOARD value) {
	std::string str(64, '0');

	for (int i = 0; i < 64; i++)
	{
		if ((1ll << i) & value)
			str[i] = '1';
	}

	return str;
}

std::vector<int> Auxiliary::getOneBits(BITBOARD x) {
	std::vector<int> res;
	for (int i = 0; i < 64; ++i) {
		if ((1ULL << i) & x) {
			res.push_back(i);
		}
	}
	return res;
}
