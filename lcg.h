#pragma once
#include <cstdint>

struct LCG {
	uint32_t seed;

	LCG(uint32_t s) : seed(s) {}

	int rand();

	void step(uint32_t n);
};