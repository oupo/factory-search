#include "lcg.h"

const uint32_t A = 0x41c64e6d;
const uint32_t B = 0x6073;

int LCG::rand() {
	seed = seed * A + B;
	return seed >> 16;
}

void LCG::step(uint32_t n) {
	uint32_t a = A, b = B;
	while (n != 0) {
		if ((n & 1) != 0) {
			seed = seed * a + b;
		}
		b = a * b + b;
		a = a * a;
		n >>= 1;
	}
}
