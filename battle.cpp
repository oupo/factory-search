#include <algorithm>
#include <random>
#include <unordered_map>
#include <iostream>
#include "factory-data.h"
#include "factory.h"
#include "damage.h"
#include "utils.h"

struct State {
	PokeStruct myPokes[3];
	PokeStruct foePokes[3];
	int curr;
	int foeCurr;
	int turn;
	bool operator==(const State& rhs) const;
	bool operator!=(const State& rhs) const;
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

inline bool State::operator==(const State& rhs) const {
	const State& lhs = *this;
	return equal(reinterpret_cast<const char*>(&lhs), reinterpret_cast<const char*>(&lhs) + sizeof(State), reinterpret_cast<const char*>(&rhs));
}

inline bool State::operator!=(const State& rhs) const {
	return !(this->operator==(rhs));
}

struct Hash {
	typedef std::size_t result_type;
	std::size_t operator()(const State& key) const;
};

inline std::size_t Hash::operator()(const State& key) const {
	std::string bytes(reinterpret_cast<const char*>(&key), sizeof(State));
	return std::hash<std::string>()(bytes);
}

typedef std::unordered_map<State, int, Hash> StateToInt;

struct MonteCarlo {
	State initial_state;
	StateToInt plays;
	StateToInt wins;
	mt19937 rnd;
	void run_simulation();
};

struct PokeAndPlay {
	PLAYER player;
	PokeStruct *poke;
	Play play;
};

State proceed(State state, mt19937 &rnd, Play myPlay) {
	if (get_winner(state)) return state;
	Play foePlay;
	if (myPlay.type == PLAYTYPE_SINIDASI) {
		foePlay = Play{ PLAYTYPE_DONOTHING };
	} else if (state.foePokes[state.foeCurr].hp <= 0) {
		do {
			foePlay = Play{ PLAYTYPE_SINIDASI, 0, rnd() % 3 };
		} while (state.foePokes[foePlay.indexChange].hp <= 0);
	} else {
		foePlay = Play{ PLAYTYPE_WAZA, state.foePokes[state.foeCurr].wazas[rnd() % 4], -1 };
	}
	if (myPlay.type == PLAYTYPE_CHANGE) {
		state.curr = myPlay.indexChange;
	}
	if (foePlay.type == PLAYTYPE_CHANGE) {
		state.foeCurr = foePlay.indexChange;
	}
	PokeAndPlay pps[2] = {
		PokeAndPlay{PLAYER_ME, &state.myPokes[state.curr], myPlay},
		PokeAndPlay{PLAYER_FOE, &state.foePokes[state.foeCurr], foePlay}
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
	if (myPlay.type != PLAYTYPE_SINIDASI && foePlay.type != PLAYTYPE_SINIDASI) {
		state.turn++;
	}
	return state;
}

vector<Play> legal_plays(State &state) {
	if (state.foePokes[state.foeCurr].hp <= 0) {
		return { Play{PLAYTYPE_DONOTHING} };
	} else {
		vector<Play> plays;
		for (int i = 0; i < 4; i++) {
			plays.push_back(Play{ PLAYTYPE_WAZA, state.myPokes[state.curr].wazas[i], -1 });
		}
		for (int i = 0; i < 3; i++) {
			if (i == state.curr) continue;
			plays.push_back(Play{ PLAYTYPE_CHANGE, 0, i });
		}
		return plays;
	}
}

void MonteCarlo::run_simulation() {
	vector<State> visited_states;
	State state = initial_state;
	PLAYER winner = PLAYER_NONE;
	bool expand = true;
	for (int t = 0; t < 200; t++) {
		vector<Play> legal = legal_plays(state);
		Play play = legal[rnd() % legal.size()];
		state = proceed(state, rnd, play);

		if (expand && plays.find(state) == plays.end()) {
			expand = false;
			plays[state] = 0;
			wins[state] = 0;
		}
		visited_states.push_back(state);

		winner = get_winner(state);
		if (winner) break;
	}
	for (State state : visited_states) {
		if (plays.find(state) == plays.end()) continue;
		plays[state] ++;
		if (winner == PLAYER_ME) {
			wins[state] ++;
		}
	}
}

int main() {
	MonteCarlo monte;
	monte.initial_state = State{
		{
			gen_poke(starter_rank(false, 1), &ENTRIES[1], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[2], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[3], 0),
		},
		{
			gen_poke(starter_rank(false, 1), &ENTRIES[4], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[5], 0),
			gen_poke(starter_rank(false, 1), &ENTRIES[6], 0),
		},
		0, 0, 0,
	};
	monte.run_simulation();
	return 0;
}