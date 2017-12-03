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
	if (state.turn > 10) {
		return PLAYER_FOE;
	}
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

typedef std::unordered_map<State, double, Hash> StateToDouble;

struct PokeAndPlay {
	PLAYER player;
	PokeStruct *poke;
	Play play;
};

struct RandomValue2 {
	RandomValue val[2];
	int foeMove;
};

State proceed(State state, RandomValue2 &random, Play myPlay) {
	if (get_winner(state)) return state;
	Play foePlay;
	if (myPlay.type == PLAYTYPE_SINIDASI) {
		foePlay = Play{ PLAYTYPE_DONOTHING };
	} else if (state.foePokes[state.foeCurr].hp <= 0) {
		vector<int> changeCandidates;
		for (int i = 0; i < 3; i++) {
			if (state.foePokes[i].hp > 0) changeCandidates.push_back(i);
		}
		foePlay = Play{ PLAYTYPE_SINIDASI, 0, changeCandidates[random.foeMove] };
	} else {
		foePlay = Play{ PLAYTYPE_WAZA, state.foePokes[state.foeCurr].wazas[random.foeMove], -1 };
	}
	if (myPlay.type == PLAYTYPE_CHANGE || myPlay.type == PLAYTYPE_SINIDASI) {
		state.curr = myPlay.indexChange;
	}
	if (foePlay.type == PLAYTYPE_CHANGE || foePlay.type == PLAYTYPE_SINIDASI) {
		state.foeCurr = foePlay.indexChange;
	}
	//cout << "proceed: " << myPlay.type << " " << (myPlay.waza ? myPlay.waza->name : "") << state.myPokes[state.curr].hp
	//	 << ", " << foePlay.type << " " << (foePlay.waza ? foePlay.waza->name : "") << state.foePokes[state.curr].hp << endl;
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
		int damage = calcDamage(poke0, poke1, waza, random.val[i]);
		poke1->hp -= damage;
		if (poke1->hp <= 0) return state;
	}
	if (myPlay.type != PLAYTYPE_SINIDASI && foePlay.type != PLAYTYPE_SINIDASI) {
		state.turn++;
	}
	return state;
}

void each_product(vector<int> vec, function<void(vector<int>, int)> fn) {
	int prod = 1;
	int n = vec.size();
	for (int x : vec) { prod *= x; }
	//cout << "each_prodoct: " << prod << endl;
	for (int x = 0; x < prod; x++) {
		vector<int> v(n);
		int y = x;
		for (int i = 0; i < n; i++) {
			v[i] = y % vec[i];
			y /= vec[i];
		}
		fn(v, x);
	}
}

vector<Play> legal_plays(State &state) {
	if (state.foePokes[state.foeCurr].hp <= 0) {
		return { Play{ PLAYTYPE_DONOTHING } };
	}
	if (state.myPokes[state.curr].hp <= 0) {
		vector<Play> plays;
		for (int i = 0; i < 3; i++) {
			if (state.myPokes[i].hp <= 0) continue;
			plays.push_back(Play{ PLAYTYPE_SINIDASI, 0, i });
		}
		return plays;
	}
	else {
		vector<Play> plays;
		for (int i = 0; i < 4; i++) {
			plays.push_back(Play{ PLAYTYPE_WAZA, state.myPokes[state.curr].wazas[i], -1 });
		}
		for (int i = 0; i < 3; i++) {
			if (i == state.curr) continue;
			if (state.myPokes[i].hp <= 0) continue;
			plays.push_back(Play{ PLAYTYPE_CHANGE, 0, i });
		}
		return plays;
	}
}

double calcProbability0(State &state, StateToDouble &cash) {
	if (cash.find(state) != cash.end()) {
		return cash[state];
	}
	PLAYER winner = get_winner(state);
	if (winner == PLAYER_FOE) return 0.0;
	if (winner == PLAYER_ME) return 1.0;
	//cout << state.turn << endl;
	double prob = 0.0;
	for (Play play : legal_plays(state)) {
		double pr = 0.0;
		int numFoeMove;
		if (play.type == PLAYTYPE_SINIDASI) {
			numFoeMove = 1;
		} else if (state.foePokes[state.foeCurr].hp <= 0) {
			numFoeMove = 0;
			for (int i = 0; i < 3; i++) {
				if (state.foePokes[i].hp > 0) numFoeMove++;
			}
		} else {
			numFoeMove = 4;
		}
		each_product({ 3, 2, 3, 2, numFoeMove }, [&](vector<int> v, int x) {
			int damageRand0 = v[0];
			int critical0 = v[1];
			int damageRand1 = v[2];
			int critical1 = v[3];
			int foeMove = v[4];
			//cout << damageRand0 << ", " << critical0 << ", " << damageRand1 << ", " << critical1 << ", " << foeMove	 << endl;
			RandomValue2 random = { RandomValue{ damageRand0, critical0 }, RandomValue{ damageRand1, critical1 }, foeMove };
			double p = calcProbability0(proceed(state, random, play), cash);
			pr += p / 3 / 3 * (critical0 ? 1 : 15) / 16 * (critical1 ? 1 : 15) / 16 / numFoeMove;
		});
		if (prob < pr) prob = pr;
	}
	cout << prob << endl;
	cash[state] = prob;
	return prob;
}

double calcProbability(State &state) {
	StateToDouble cash;
	return calcProbability0(state, cash);
}


int main() {
	State state = State{
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
	cout << calcProbability(state) << endl;
	return 0;
}