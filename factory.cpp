#include <iostream>
#include <array>
#include <vector>
#include <sstream>
#include <iomanip>
#include "lcg.h"
#include "factory-data.h"

using namespace std;

template <typename T>
std::vector<T> operator+(const std::vector<T> &A, const std::vector<T> &B)
{
	std::vector<T> AB;
	AB.reserve(A.size() + B.size());                // preallocate memory
	AB.insert(AB.end(), A.begin(), A.end());        // add A;
	AB.insert(AB.end(), B.begin(), B.end());        // add B;
	return AB;
}

template <typename T>
std::vector<T> &operator+=(std::vector<T> &A, const std::vector<T> &B)
{
	A.reserve(A.size() + B.size());                // preallocate memory without erase original data
	A.insert(A.end(), B.begin(), B.end());         // add B;
	return A;                                        // here A could be named AB
}

template<typename Container>
bool exist_in(const Container& c, const typename Container::value_type& v) {
	return (c.end() != std::find(c.begin(), c.end(), v));
}

int calc_status_hp(int lv, int base, int individual, int effort) {
	return ((base * 2 + (effort / 4) + individual) * lv / 100) + lv + 10;
}

int calc_status(int lv, int base, int individual, int effort, int nature_revision) {
	return ((((base * 2 + (effort / 4) + individual) * lv / 100) + 5) * nature_revision / 10);
}

struct RankObject {
    bool is_open_level;
    int rank;
    int range_min;
    int range_max;
    int iv;
};

array<int, 6> get_status(const RankObject &rank, Entry entry) {
	int lv = rank.is_open_level ? 100 : 50;
    int iv = rank.iv;
	PokemonSpec poke = entry->pokemon();
	int hp = calc_status_hp(lv, poke->stats[0], iv, entry->effort[0]);
	if (poke->stats[0] == 1) hp = 1; // ヌケニン
	array<int, 6> result;
	result[0] = hp;
	for (int i = 0; i < 5; i ++) {
		result[i + 1] = calc_status(lv, poke->stats[i + 1], iv, entry->effort[i + 1], NATURE_TABLE[entry->nature][i]);
	}
	return result;
}

bool entries_collision0(Entry entry, const vector<Entry> entries) {
	for (int i = 0; i < entries.size(); i++) {
		if (entry->pokemonNo == entries[i]->pokemonNo) return true;
		if (entry->itemNo == entries[i]->itemNo) return true;
	}
	return false;
}

bool entries_collision(Entry entry, const vector<Entry> entries, const vector<Entry> visited_entries) {
	return entries_collision0(entry, entries) || entries_collision0(entry, visited_entries);
}

bool is_shiny_pid(int parent_id, int secret_id, uint32_t pid) {
	return ((parent_id ^ secret_id ^ (pid >> 16) ^ (pid & 0xffff)) & ~7) == 0;
}

int ENTRY_RANGES[][2] ={
	{0, 0},
	{1, 150},
	{151, 250},
	{251, 350},
	{351, 486},
	{487, 622},
	{623, 758},
	{759, 894},
	{351, 950},
};

RankObject RANKS_LV50[] = {
	RankObject{},
	RankObject{ false, 1, ENTRY_RANGES[1][0], ENTRY_RANGES[1][1], 0 },
	RankObject{ false, 2, ENTRY_RANGES[2][0], ENTRY_RANGES[2][1], 4 },
	RankObject{ false, 3, ENTRY_RANGES[3][0], ENTRY_RANGES[3][1], 8 },
	RankObject{ false, 4, ENTRY_RANGES[4][0], ENTRY_RANGES[4][1], 12 },
	RankObject{ false, 5, ENTRY_RANGES[5][0], ENTRY_RANGES[5][1], 16 },
	RankObject{ false, 6, ENTRY_RANGES[6][0], ENTRY_RANGES[6][1], 20 },
	RankObject{ false, 7, ENTRY_RANGES[7][0], ENTRY_RANGES[7][1], 24 },
	RankObject{ false, 8, 351, 950, 31 },
};

RankObject RANKS_OPEN[] = {
	RankObject{},
	RankObject{ true, 1, ENTRY_RANGES[4][0], ENTRY_RANGES[4][1], 0 },
	RankObject{ true, 2, ENTRY_RANGES[5][0], ENTRY_RANGES[5][1], 4 },
	RankObject{ true, 3, ENTRY_RANGES[6][0], ENTRY_RANGES[6][1], 8 },
	RankObject{ true, 4, ENTRY_RANGES[7][0], ENTRY_RANGES[7][1], 12 },
	RankObject{ true, 5, 623, 950, 16 },
	RankObject{ true, 6, 623, 950, 20 },
	RankObject{ true, 7, 623, 950, 24 },
	RankObject{ true, 8, 351, 950, 31 },
};

RankObject RANKS_OPEN_TRAINER[] = {
	RankObject{},
	RankObject{ true, 1, ENTRY_RANGES[4][0], ENTRY_RANGES[4][1], 0 },
	RankObject{ true, 2, ENTRY_RANGES[5][0], ENTRY_RANGES[5][1], 4 },
	RankObject{ true, 3, ENTRY_RANGES[6][0], ENTRY_RANGES[6][1], 8 },
	RankObject{ true, 4, ENTRY_RANGES[7][0], ENTRY_RANGES[7][1], 12 },
	RankObject{ true, 5, 351, 950, 16 },
	RankObject{ true, 6, 351, 950, 20 },
	RankObject{ true, 7, 351, 950, 24 },
	RankObject{ true, 8, 351, 950, 31 },
};

RankObject RANK_MIDBOSS_LV50_PT = RankObject{ false, 4, ENTRY_RANGES[7][0], ENTRY_RANGES[7][1], 12 };
RankObject RANK_MIDBOSS_LV50_HGSS = RankObject{ false, 4, ENTRY_RANGES[7][0], ENTRY_RANGES[7][1], 12 };
RankObject RANK_MIDBOSS_OPEN_PT = RankObject{ true, 4, 759, 950, 12 };
RankObject RANK_MIDBOSS_OPEN_HGSS = RankObject{ true, 4, 351, 486, 12 };

RankObject RANK_FNLBOSS_LV50_PT = RankObject{ false, 8, 351, 486, 31 };
RankObject RANK_FNLBOSS_LV50_HGSS = RankObject{ false, 8, 759, 950, 31 };
RankObject RANK_FNLBOSS_OPEN_PT = RankObject{ true, 8, 759, 950, 31 };
RankObject RANK_FNLBOSS_OPEN_HGSS = RankObject{ true, 8, 759, 950, 31 };

const int MIDBOSS_ID = 300;
const int FNLBOSS_ID = 301;

RankObject starter_rank(bool is_open_level, int rank) {
	return (is_open_level ? RANKS_OPEN : RANKS_LV50)[min(rank, 8)];
}

RankObject trainer_rank(bool is_open_level, int rank) {
	int r = min(rank, 8);
	if (is_open_level) {
		return RANKS_OPEN_TRAINER[r];
	} else {
		return RANKS_LV50[r];
	}
}

int trainer_id_to_rank_basis(int id) {
	if (id <= 99) return 1;
	if (id <= 119) return 2;
	if (id <= 139) return 3;
	if (id <= 159) return 4;
	if (id <= 179) return 5;
	if (id <= 199) return 6;
	if (id <= 219) return 7;
	if (id <= 299) return 8;
	return 0;
}

RankObject trainer_id_to_rank(bool is_open_level, bool is_hgss, int id) {
	switch (id) {
	case MIDBOSS_ID:
		if (is_open_level) {
			if (is_hgss) return RANK_MIDBOSS_OPEN_HGSS;
			else return RANK_MIDBOSS_OPEN_PT;
		} else {
			if (is_hgss) return RANK_MIDBOSS_LV50_HGSS;
			else return RANK_MIDBOSS_LV50_PT;
		}
	case FNLBOSS_ID:
		if (is_open_level) {
			if (is_hgss) return RANK_FNLBOSS_OPEN_HGSS;
			else return RANK_FNLBOSS_OPEN_PT;
		}
		else {
			if (is_hgss) return RANK_FNLBOSS_LV50_HGSS;
			else return RANK_FNLBOSS_LV50_PT;
		}
	}
	int r = trainer_id_to_rank_basis(id);
	return trainer_rank(is_open_level, r);
}

Entry rand_entry(LCG &lcg, RankObject &rank) {
	int start = rank.range_min, end = rank.range_max;
	int count = end - start + 1;
	return &ENTRIES[end - lcg.rand() % count];
}

vector<Entry> rand_entries(LCG &lcg, RankObject &rank, int num, vector<Entry> seen) {
	vector<Entry> entries;
	while (entries.size() < num) {
		Entry entry = rand_entry(lcg, rank);
		if (!entries_collision(entry, entries, seen)) {
			entries.push_back(entry);
		}
	}
	return entries;
}

vector<Entry> rand_starter_entries(LCG &lcg, bool is_open_level, int shuu, int num_bonus) {
	vector<Entry> entries = rand_entries(lcg, starter_rank(is_open_level, shuu), 6 - num_bonus, {});
	vector<Entry> bonusEntries = rand_entries(lcg, starter_rank(is_open_level, shuu + 1), num_bonus, entries);
	entries += bonusEntries;
	return entries;
}

vector<uint32_t> rand_pid(LCG &lcg, vector<Entry> entries) {
	vector<uint32_t> pids;
	for (Entry entry : entries) {
		int nature = entry->nature;
		int tid = lcg.rand();
		int sid = lcg.rand();
		uint32_t pid;
		do {
			int pid_low = lcg.rand();
			int pid_high = lcg.rand();
			pid = pid_high << 16 | pid_low;
		} while (pid % 25 != nature || is_shiny_pid(tid, sid, pid));
		pids.push_back(pid);
	}
	return pids;
}

int rand_trainer_id(LCG &lcg, int shuu, int is_last) {
	int r = lcg.rand();
	if (shuu >= 8) {
		return r % 99 + 200;
	}
	else if (is_last) {
		return r % 19 + 100 + (shuu - 1) * 20;
	}
	else if (shuu == 1) {
		return r % 99;
	}
	else {
		return r % 39 + 80 + (shuu - 2) * 20;
	}
}

vector<int> rand_trainers0(LCG &lcg, int shuu, int num, vector<int> seen, bool is_last_trainer_next_rank) {
	vector<int> trainers;
	int i = 1;
	while (i <= num) {
		if ((shuu == 3 || shuu == 7) && i == 7 && seen.size() == 0) {
			trainers.push_back(shuu == 3 ? MIDBOSS_ID : FNLBOSS_ID);
			break;
		}
		int e = rand_trainer_id(lcg, shuu, is_last_trainer_next_rank && i == 7);
		if (exist_in(trainers, e) || exist_in(seen, e)) continue;
		trainers.push_back(e);
		i++;
	}
	return trainers;
}

vector<int> rand_trainers(LCG &lcg, int shuu, bool is_hgss) {
	// トレーナーA
	vector<int> trainers = rand_trainers0(lcg, shuu, 7, {}, true);
	// トレーナーB
	//   2周目と6周目は6人分しか決定しない
	//   プラチナでは7人目も6人目までと同じ決定方法
	vector<int> trainers2 = rand_trainers0(lcg, shuu,
		(shuu == 2 || shuu == 6) ? 6 : 7, trainers,
		is_hgss);
	return trainers;
}



vector<Entry> rand_enemy_poke(LCG &lcg, RankObject &rank, vector<Entry> seen) {
	vector<Entry> entries = rand_entries(lcg, rank, 3, seen);
	vector<uint32_t> pids = rand_pid(lcg, entries);
	return entries;
}

void rand_gap(LCG &lcg, int shuu, int nth, RankObject &enemy_rank) {
	int c;
	if (shuu >= 5) {
		c = 24;
	}
	else if (enemy_rank.rank == 1) {
		c = 6;
	}
	else {
		c = 12;
	}
	if (nth == 1) {
		c += shuu == 1 ? 18 : 36;
	}
	lcg.step(c);
}

string tohex(uint32_t x) {
	ostringstream ostr;
	ostr << hex << setfill('0') << setw(8) << x;
	return ostr.str();
}

int main() {
	LCG lcg(0);
	int shuu = 1;
	bool is_hgss = false;
	bool is_open = false;
	int num_bonus = 0;
	vector<int> trainers = rand_trainers(lcg, shuu, is_hgss);
	vector<Entry> starterEntries = rand_starter_entries(lcg, is_open, shuu, num_bonus);
	vector<uint32_t> pids = rand_pid(lcg, starterEntries);
	lcg.step(2);

	vector<Entry> playerEntries = { starterEntries[0], starterEntries[1], starterEntries[2] };
	
	for (int i = 0; i < 6; i++) {
		cout << starterEntries[i]->pokemon()->name << endl;
	}

	vector<Entry> prev;
	for (int i = 1; i <= 7; i ++) {
		cout << i << "戦目:" << endl;
		RankObject rank = trainer_id_to_rank(is_open, is_hgss, trainers[i - 1]);
		vector<Entry> seen = i == 1 ? starterEntries : playerEntries + prev;
		vector<Entry> entries = rand_enemy_poke(lcg, rank, seen);
		rand_gap(lcg, shuu, i, rank);
		prev = entries;
		for (int i = 0; i < 3; i++) {
			cout << entries[i]->pokemon()->name << endl;
		}
	}
	return 0;
}