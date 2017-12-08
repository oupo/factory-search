#pragma once
#include "factory-data.h"
#include <random>

struct State {
	PokeStruct myPokes[3];
	PokeStruct foePokes[3];
	int curr;
	int foeCurr;
	int turn;
	int trickRoom;
};

int calcComputedSpeed(PokeStruct *poke, State *state);
int calcDamage(PokeStruct *userPoke, PokeStruct *foePoke, Waza waza, mt19937 &rnd);