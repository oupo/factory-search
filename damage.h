#pragma once
#include "factory-data.h"
#include <random>
int calcComputedSpeed(PokeStruct *poke);
int calcDamage(PokeStruct *userPoke, PokeStruct *foePoke, Waza waza, RandomValue &random);