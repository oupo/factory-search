#include <cstdint>

struct LCG {
	uint32_t seed;

	LCG(uint32_t s) : seed(s) {}

	int rand() {
		seed = seed * 0x41c64e6d + 0x6073;
		return seed >> 16;
	}
};