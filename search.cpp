#include <iostream>
#include <set>
#include <algorithm>
#include <stack>
#include <functional>
#include "damage.h"
#include "utils.h"
#include "factory.h"

const set<int> NG_ITEMS = { ITEM_せんせいのツメ };
const set<int> NG_WAZA = { WAZA_こらえる };
const set<int> NG_WAZA2 = { WAZA_りゅうのまい, WAZA_トリックルーム, WAZA_めいそう, WAZA_のろい, WAZA_ひかりのかべ, WAZA_リフレクター, WAZA_かげぶんしん, WAZA_ちいさくなる };
const set<int> NG_WAZA3_CODE = { WAZA[WAZA_さいみんじゅつ].effectCode, WAZA[WAZA_どくばり].effectCode, WAZA[WAZA_ほのおのパンチ].effectCode, WAZA[WAZA_れいとうパンチ].effectCode, WAZA[WAZA_かみなりパンチ].effectCode, WAZA[WAZA_ふきとばし].effectCode, WAZA[WAZA_どくどく].effectCode, WAZA[WAZA_ひかりのかべ].effectCode, WAZA[WAZA_トライアタック].effectCode, WAZA[WAZA_ほのおのうず].effectCode, WAZA[WAZA_あやしいひかり].effectCode, WAZA[WAZA_こうそくいどう].effectCode, WAZA[WAZA_こわいかお].effectCode, WAZA[WAZA_リフレクター].effectCode, WAZA[WAZA_でんじは].effectCode, WAZA[WAZA_こごえるかぜ].effectCode, WAZA[WAZA_ばくれつパンチ].effectCode, WAZA[WAZA_みがわり].effectCode, WAZA[WAZA_やどりぎのタネ].effectCode, WAZA[WAZA_かなしばり].effectCode, WAZA[WAZA_アンコール].effectCode, WAZA[WAZA_うらみ].effectCode, WAZA[WAZA_でんこうせっか].effectCode, WAZA[WAZA_まきびし].effectCode, WAZA[WAZA_ほろびのうた].effectCode, WAZA[WAZA_すなあらし].effectCode, WAZA[WAZA_いばる].effectCode, WAZA[WAZA_メロメロ].effectCode, WAZA[WAZA_バトンタッチ].effectCode, WAZA[WAZA_げんしのちから].effectCode, WAZA[WAZA_みらいよち].effectCode, WAZA[WAZA_かみなり].effectCode, WAZA[WAZA_あられ].effectCode, WAZA[WAZA_いちゃもん].effectCode, WAZA[WAZA_おにび].effectCode, WAZA[WAZA_ちょうはつ].effectCode, WAZA[WAZA_トリック].effectCode, WAZA[WAZA_あくび].effectCode, WAZA[WAZA_ブレイズキック].effectCode, WAZA[WAZA_どくどくのキバ].effectCode, WAZA[WAZA_ポイズンテール].effectCode, WAZA[WAZA_りゅうのまい].effectCode, WAZA[WAZA_つぼをつく].effectCode, WAZA[WAZA_とんぼがえり].effectCode, WAZA[WAZA_どくびし].effectCode, WAZA[WAZA_フレアドライブ].effectCode, WAZA[WAZA_トリックルーム].effectCode, WAZA[WAZA_うずしお].effectCode, WAZA[WAZA_ステルスロック].effectCode, WAZA[WAZA_ほのおのキバ].effectCode, WAZA[WAZA_こおりのキバ].effectCode, WAZA[WAZA_かみなりのキバ].effectCode, WAZA[WAZA_のしかかり].effectCode, WAZA[WAZA_ステルスロック].effectCode, WAZA[WAZA_ほのおのキバ].effectCode, WAZA[WAZA_こおりのキバ].effectCode, WAZA[WAZA_かみなりのキバ].effectCode, WAZA[WAZA_のしかかり].effectCode };

struct StatusCond {
	int cond;
	int turn; // もうどくターン
};

struct StackElem {
	vector<int> myHps;
	vector<StatusCond>	statusConds;
	int curr;
	vector<int> foeHps;
	int foeCurr;
	Waza fixedWaza;
	bool justAfterFoeExchanged;
	int atkMod;
};

bool completely_winnable(vector<Poke> mys, vector<Poke> foes, bool allowExchange) {
	if (any_of(foes.begin(), foes.end(), [](Poke p) { return exist_in(NG_ITEMS, p->item); })) {
		return false;
	}
	for (Poke p : foes) {
		if (any_of(p->entry->waza, p->entry->waza + 4, [](int waza) { return exist_in(NG_WAZA, waza); })) {
			return false;
		}
	}
	vector<int> myHps = apply(mys, [](Poke p) { return p->hp; });
	vector<StatusCond>	statusConds = apply(mys, [](Poke p) { return StatusCond{ 0, 0 }; });
	int curr = 0;
	vector<int> foeHps = apply(foes, [](Poke p) { return p->hp; });
	int foeCurr = 0;
	Waza fixedWaza = 0;
	bool justAfterFoeExchanged = true;
	int atkMod = 0;
	stack<StackElem> st;

	auto stackPush = [&]() {
		st.push(StackElem{
			myHps,
			statusConds,
			curr,
			foeHps,
			foeCurr,
			fixedWaza,
			justAfterFoeExchanged,
			atkMod,
		});
	};
	auto stackPop = [&]() {
		StackElem popped = st.top();
		myHps = popped.myHps;
		statusConds = popped.statusConds;
		curr =  popped.curr;
		foeHps = popped.foeHps;
		foeCurr = popped.foeCurr;
		fixedWaza = popped.fixedWaza;
		justAfterFoeExchanged = popped.justAfterFoeExchanged;
		atkMod = popped.atkMod;
		st.pop();
	};
	function<void()> winnable = [&]() {
		winnable();
	};
	return true;
}

void judge_seed(uint32_t seed) {
	LCG lcg(seed);
	int shuu = 2;
	bool is_open_level = false;
	bool is_hgss = false;
	int num_bonus = 1;
	vector<int> trainers = rand_trainers(lcg, shuu, is_hgss);
	vector<Entry> entries = rand_starter_entries(lcg, is_open_level, shuu, num_bonus);
	int suba_idx = indexof(apply(entries, [](Entry e) { return e->no(); }), 289);
	if (suba_idx < 0) return;
	vector<Poke> starters = starter_entiries_to_poke(is_open_level, shuu, 0, entries, rand_pid(lcg, entries));
	Poke suba = starters[suba_idx];
	lcg.step(2); // shuffle

	LCG l(lcg.seed);
	int idx = suba == 0 ? 1 : 0;
	int idx2 = 5; // ボーナスポケをとる

	vector<Poke> playerPokes = { starters[suba_idx], starters[idx], starters[idx2] };

	vector<Poke> prev;
	int i;
	for (i = 1; i <= 1; i++) {
		vector<Entry> seen = apply(i == 1 ? starters : playerPokes + prev, [](Poke p) { return p->entry; });
		RankObject rank = trainer_id_to_rank(is_open_level, is_hgss, trainers[i - 1]);
		vector<Poke> pokes = rand_enemy_poke(l, rank, seen);

		if (!completely_winnable(playerPokes, pokes, true)) {
			break;
		}
		prev = pokes;
		rand_gap(l, shuu, i, rank);
	}
	if (i - 1 >= 1) {
		cout << tohex(seed) << endl;
	}
	
}

int main() {
	int count = 0;
	auto hoge = [&]() {
		return count ++;
	};
	for (uint64_t i = 0; i < (1ull << 32); i++) {
		judge_seed(i);
	}
}

