#include <algorithm>
#include <random>
#include "factory-data.h"
#include "damage.h"

struct State {
	PokeStruct myPokes[3];
	PokeStruct foePokes[3];
	int curr;
	int foeCurr;
	int turn;
};

enum PlayType {
	PLAYTYPE_WAZA,
	PLAYTYPE_CHANGE,
	PLAYTYPE_DONOTHING,
	PLAYTYPE_SINIDASI,
};

struct Play {
	PlayType type;
	Waza waza;
	int indexChange;
};

enum PLAYER {
	PLAYER_NONE,
	PLAYER_ME,
	PLAYER_ENEMY
};

PLAYER winner(State &state) {
	if (all_of(state.myPokes, state.myPokes + 3, [](PokeStruct p) { return p.hp <= 0; })) {
		return PLAYER_ENEMY;
	}
	if (all_of(state.foePokes, state.foePokes + 3, [](PokeStruct p) { return p.hp <= 0;  })) {
		return PLAYER_ME;
	}
	return PLAYER_NONE;
}

class MonteCarlo {
	State state;
};

struct PokeAndPlay {
	PLAYER player;
	PokeStruct *poke;
	Play play;
};

State proceed(State state, mt19937 &rnd, Play myPlay) {
	if (winner(state)) return state;
	Play enemyPlay;
	if (myPlay.type == PLAYTYPE_SINIDASI) {
		enemyPlay = Play{ PLAYTYPE_DONOTHING };
	} else if (state.foePokes[state.foeCurr].hp == 0) {
		do {
			enemyPlay = Play{ PLAYTYPE_SINIDASI, 0, rnd() % 3 };
		} while (state.foePokes[enemyPlay.indexChange].hp <= 0);
	} else {
		enemyPlay = Play{ PLAYTYPE_WAZA, state.foePokes[state.foeCurr].wazas[rnd() % 4], -1 };
	}
	if (myPlay.type == PLAYTYPE_CHANGE) {
		state.curr = myPlay.indexChange;
	}
	if (enemyPlay.type == PLAYTYPE_CHANGE) {
		state.foeCurr = enemyPlay.indexChange;
	}
	PokeAndPlay pps[2] = {
		PokeAndPlay{PLAYER_ME, &state.myPokes[state.curr], myPlay},
		PokeAndPlay{PLAYER_ENEMY, &state.foePokes[state.foeCurr], enemyPlay}
	};
	if (calcComputedSpeed(pps[0].poke) < calcComputedSpeed(pps[1].poke)) {
		swap(pps[0], pps[1]);
	}
	for (int i = 0; i < 2; i++) {
		PokeStruct *poke0 = pps[i].poke;
		PokeStruct *poke1 = pps[1 - i].poke;
		Play play = pps[i].play;
		if (play.type != PLAYTYPE_WAZA) continue;
		Waza waza = play.waza;
		int damage = calcDamage(poke0, poke1, waza, rnd);
		poke1->hp -= damage;
		if (poke1->hp <= 0) return state;
	}
	return state;
}

vector<Play> legal_plays(State &state) {
	return {};
}