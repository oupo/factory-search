#pragma once
#include "factory-data.h"
int calcComputedSpeed(Poke poke);
int calcDamage(Poke userPoke, Poke foePoke, Waza waza, bool tomax, bool hasStatusCondition, int atkMod, bool considerCritical);