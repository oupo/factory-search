#include <iostream>
#include <set>
#include <algorithm>
#include <stack>
#include <functional>
#include "damage.h"
#include "utils.h"
#include "factory.h"

vector<Waza> get_wazas(Poke p) {
	return apply(vector<int>({ 0,1,2,3 }), [&](int i) { return p->entry->getWaza(i); });
}

int judge_seed(uint32_t seed) {
	LCG lcg(seed);
	int shuu = 3;
	bool is_open_level = false;
	bool is_hgss = false;
	int num_bonus = 2;
	vector<int> trainers = rand_trainers(lcg, shuu, is_hgss);
	vector<Entry> entries = rand_starter_entries(lcg, is_open_level, shuu, num_bonus);
	vector<Poke> starters = starter_entiries_to_poke(is_open_level, shuu, 0, entries, rand_pid(lcg, entries));
	lcg.step(2); // shuffle

	LCG l(lcg.seed);

	vector<Poke> playerPokes = { starters[0], starters[4], starters[5] };

	vector<Poke> prev;
	int i;
	for (i = 1; i <= 1; i++) {
		vector<Entry> seen = apply(i == 1 ? starters : playerPokes + prev, [](Poke p) { return p->entry; });
		RankObject rank = trainer_id_to_rank(is_open_level, is_hgss, trainers[i - 1]);
		vector<Poke> pokes = rand_enemy_poke(l, rank, seen);

		//bool win = completely_winnable(playerPokes, pokes, true);
		bool win = false;
		cout << tohex(seed) << ": " << win << endl;
		if (!win) {
			break;
		}
		prev = pokes;
		rand_gap(l, shuu, i, rank);
	}
	if (i - 1 >= 1) {
		return 2;
		//cout << tohex(seed) << endl;
	}
	return 1;
}

int search_main() {
	int count = 0;
	int count_succeed = 0;
	for (uint64_t i = 0; i < 10000; i++) {
		int judged = judge_seed(i);
		if (judged > 0) count ++;
		if (judged == 2) count_succeed ++;
	}
	cout << count_succeed << " / " << count << endl;
	return 0;
}

