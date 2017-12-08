#include <algorithm>
#include <random>
#include <iostream>
#include "factory-data.h"
#include "factory.h"
#include "damage.h"
#include "utils.h"

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
	PLAYER_FOE
};

PLAYER get_winner(State &state) {
	if (all_of(state.myPokes, state.myPokes + 3, [](PokeStruct p) { return p.hp <= 0; })) {
		return PLAYER_FOE;
	}
	if (all_of(state.foePokes, state.foePokes + 3, [](PokeStruct p) { return p.hp <= 0;  })) {
		return PLAYER_ME;
	}
	return PLAYER_NONE;
}

struct PokeAndPlay {
	PLAYER player;
	PokeStruct *poke;
	Play play;
};

Play player_play(State &state) {
	if (state.myPokes[state.curr].hp <= 0) {
		int idx = find_if(state.myPokes, state.myPokes + 3, [](PokeStruct &p) { return p.hp > 0; }) - state.myPokes;
		return { Play{ PLAYTYPE_SINIDASI, 0, idx } };
	}
	if (state.foePokes[state.foeCurr].hp <= 0) {
		return { Play{ PLAYTYPE_DONOTHING } };
	}
	else {
		return Play{ PLAYTYPE_WAZA, state.myPokes[state.curr].wazas[0], -1 };
	}
}

Play foe_play(State &state, mt19937 &rnd) {
	if (state.foePokes[state.foeCurr].hp <= 0) {
		int idx = find_if(state.foePokes, state.foePokes + 3, [](PokeStruct &p) { return p.hp > 0; }) - state.foePokes;
		return { Play{ PLAYTYPE_SINIDASI, 0, idx } };
	}
	if (state.myPokes[state.curr].hp <= 0) {
		return { Play{ PLAYTYPE_DONOTHING } };
	}
	else {
		vector<Play> plays;
		return Play{ PLAYTYPE_WAZA, state.foePokes[state.foeCurr].wazas[rnd() % 4], -1 };
	}
}

State proceed(State state, mt19937 &rnd, Play myPlay) {
	if (get_winner(state)) return state;
	Play foePlay = foe_play(state, rnd);
	if (myPlay.type == PLAYTYPE_CHANGE || myPlay.type == PLAYTYPE_SINIDASI) {
		state.curr = myPlay.indexChange;
	}
	if (foePlay.type == PLAYTYPE_CHANGE || foePlay.type == PLAYTYPE_SINIDASI) {
		state.foeCurr = foePlay.indexChange;
	}
	PokeAndPlay pps[2] = {
		PokeAndPlay{PLAYER_ME, &state.myPokes[state.curr], myPlay},
		PokeAndPlay{PLAYER_FOE, &state.foePokes[state.foeCurr], foePlay}
	};
	if (calcComputedSpeed(pps[0].poke, &state) < calcComputedSpeed(pps[1].poke, &state)) {
		swap(pps[0], pps[1]);
	}
	for (int i = 0; i < 2; i++) {
		PokeStruct *poke0 = pps[i].poke;
		PokeStruct *poke1 = pps[1 - i].poke;
		Play play = pps[i].play;
		if (play.type != PLAYTYPE_WAZA) continue;
		Waza waza = play.waza;
		int damage = calcDamage(poke0, poke1, waza, rnd);
		//cout << poke0->entry->pokemon()->name << "が" << poke1->entry->pokemon()->name << "に" << waza->name << "(" << damage << ")" << endl;
		poke1->hp -= damage;
		if (poke1->hp <= 0) return state;
	}
	if (myPlay.type != PLAYTYPE_SINIDASI && foePlay.type != PLAYTYPE_SINIDASI) {
		state.turn++;
	}
	return state;
}

bool run_simulation(State initial_state, mt19937 &rnd) {
	State state = initial_state;
	PLAYER winner = PLAYER_NONE;
	for (int t = 0; t < 200; t++) {
		Play myPlay = player_play(state);
		state = proceed(state, rnd, myPlay);

		winner = get_winner(state);
		if (winner) break;
	}
	return winner == PLAYER_ME;
}

int main() {
	State initial_state = State{
		{
			gen_poke(starter_rank(false, 1), &ENTRIES[893], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[858], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[832], 0),
		},
		{
			gen_poke(starter_rank(false, 1), &ENTRIES[4], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[5], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[6], 0),
		},
		0, 0, 0,
	};
	mt19937 rnd;
	const int n = 1000;
	int count = 0;
	for (int i = 0; i < 1000; i++) {
		bool win = run_simulation(initial_state, rnd);
		cout << i << ": " << win << endl;
		if (win) count++;
	}
	cout << (double)count / n << endl;
	return 0;
}