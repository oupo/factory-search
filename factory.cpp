#include "factory-data.h"
#include <iostream>
#include <array>
#include <vector>

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

bool entries_collision0(const EntryStruct &entry, const vector<EntryStruct> entries) {
	for (int i = 0; i < entries.size(); i++) {
		if (entry.pokemonNo == entries[i].pokemonNo) return true;
		if (entry.itemNo == entries[i].itemNo) return true;
	}
	return false;
}

bool entries_collision(const EntryStruct &entry, const vector<EntryStruct> entries, const vector<EntryStruct> visited_entries) {
	return entries_collision0(entry, entries) || entries_collision0(entry, visited_entries);
}

bool is_shiny_pid(int parent_id, int secret_id, int pid) {
	return ((parent_id ^ secret_id ^ (pid >> 16) ^ (pid & 0xffff)) & ~7) == 0;
}

pair<int, int> ENTRY_RANGES[] ={
	pair<int, int>(0, 0),
	pair<int, int>(1, 150),
	pair<int, int>(151, 250),
	pair<int, int>(251, 350),
	pair<int, int>(351, 486),
	pair<int, int>(487, 622),
	pair<int, int>(623, 758),
	pair<int, int>(759, 894),
	pair<int, int>(351, 950),
};

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

struct LCG {
	int rand() { return 0;  }
};

Entry rand_entry(LCG &lcg, RankObject &rank) {
	int start = rank.range_min, end = rank.range_max;
	int count = end - start + 1;
	return &ENTRIES[end - lcg.rand() % count];
}

int main() {
    RankObject rank = RankObject{false, 1, 1, 150, 0};
    array<int, 6> st = get_status(rank, &ENTRIES[1]);
	cout << ENTRIES[1].no() << endl;
	cout << st[0] << endl;
    return 0;
}