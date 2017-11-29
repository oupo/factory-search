#include <iostream>
#include <array>
#include <vector>
#include "lcg.h"
#include "factory-data.h"

using namespace std;

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

bool is_shiny_pid(int parent_id, int secret_id, int pid) {
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


RankObject starter_rank(bool is_open_level, int rank) {
	return (is_open_level ? RANKS_LV50 : RANKS_OPEN)[min(rank, 8)];
}

RankObject trainer_rank(bool is_open_level, int rank) {
	int r = min(rank, 8);
	if (is_open_level) {
		return RANKS_OPEN_TRAINER[r];
	}
	return (is_open_level ? RANKS_LV50 : RANKS_OPEN)[r];
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

int main() {
	LCG lcg(0);
    RankObject rank = RankObject{false, 1, 1, 150, 0};
	vector<Entry> entries = rand_entries(lcg, rank, 6, {});
	for (Entry e : entries) {
		cout << e->pokemon()->name << endl;
	}
	return 0;
}