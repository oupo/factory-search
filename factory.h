#pragma once
#include <memory>
#include "lcg.h"
#include "factory-data.h"

using namespace std;

struct RankObject {
	bool is_open_level;
	int rank;
	int range_min;
	int range_max;
	int iv;
};

RankObject starter_rank(bool is_open_level, int rank);
RankObject trainer_rank(bool is_open_level, int rank);
RankObject trainer_id_to_rank(bool is_open_level, bool is_hgss, int id);
Entry rand_entry(LCG &lcg, RankObject &rank);
vector<Entry> rand_entries(LCG &lcg, RankObject &rank, int num, vector<Entry> seen);
vector<Entry> rand_starter_entries(LCG &lcg, bool is_open_level, int shuu, int num_bonus);
vector<uint32_t> rand_pid(LCG &lcg, vector<Entry> entries);
vector<int> rand_trainers(LCG &lcg, int shuu, bool is_hgss);
void rand_gap(LCG &lcg, int shuu, int nth, RankObject &enemy_rank);
PokeStruct gen_poke(RankObject &rank, Entry entry, int ability_index);
vector<Poke> starter_entiries_to_poke(bool is_open_level, int shuu, int num_bonus, vector<Entry> entries, vector<uint32_t> pids);
vector<Poke> rand_enemy_poke(LCG &lcg, RankObject &rank, vector<Entry> seen);