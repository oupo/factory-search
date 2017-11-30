#include <iostream>
#include <set>
#include <algorithm>
#include <stack>
#include <functional>
#include "damage.h"
#include "utils.h"
#include "factory.h"

int foeMaxWaza(Poke my, Poke foe) {
	int dmg = 0;
	for (int i = 0; i < 4; i ++) {
		Waza waza = &WAZA[foe->entry->waza[i]];
		int d = calcDamage(foe, my, waza, true, false, 0);
		if (dmg < d) {
			dmg = d;
		}
	}
	return dmg;
}

vector<Waza> get_wazas(Poke p) {
	return apply(vector<int>({ 0,1,2,3 }), [&](int i) { return p->entry->getWaza(i); });
}

const set<int> NG_ITEMS = { ITEM_せんせいのツメ };
const set<int> NG_WAZA = { WAZA_こらえる };
const set<int> NG_WAZA2 = { WAZA_りゅうのまい, WAZA_トリックルーム, WAZA_めいそう, WAZA_のろい, WAZA_ひかりのかべ, WAZA_リフレクター, WAZA_かげぶんしん, WAZA_ちいさくなる };
const set<int> NG_WAZA3_CODE = { WAZA[WAZA_さいみんじゅつ].effectCode, WAZA[WAZA_どくばり].effectCode, WAZA[WAZA_ほのおのパンチ].effectCode, WAZA[WAZA_れいとうパンチ].effectCode, WAZA[WAZA_かみなりパンチ].effectCode, WAZA[WAZA_ふきとばし].effectCode, WAZA[WAZA_どくどく].effectCode, WAZA[WAZA_ひかりのかべ].effectCode, WAZA[WAZA_トライアタック].effectCode, WAZA[WAZA_ほのおのうず].effectCode, WAZA[WAZA_あやしいひかり].effectCode, WAZA[WAZA_こうそくいどう].effectCode, WAZA[WAZA_こわいかお].effectCode, WAZA[WAZA_リフレクター].effectCode, WAZA[WAZA_でんじは].effectCode, WAZA[WAZA_こごえるかぜ].effectCode, WAZA[WAZA_ばくれつパンチ].effectCode, WAZA[WAZA_みがわり].effectCode, WAZA[WAZA_やどりぎのタネ].effectCode, WAZA[WAZA_かなしばり].effectCode, WAZA[WAZA_アンコール].effectCode, WAZA[WAZA_うらみ].effectCode, WAZA[WAZA_でんこうせっか].effectCode, WAZA[WAZA_まきびし].effectCode, WAZA[WAZA_ほろびのうた].effectCode, WAZA[WAZA_すなあらし].effectCode, WAZA[WAZA_いばる].effectCode, WAZA[WAZA_メロメロ].effectCode, WAZA[WAZA_バトンタッチ].effectCode, WAZA[WAZA_げんしのちから].effectCode, WAZA[WAZA_みらいよち].effectCode, WAZA[WAZA_かみなり].effectCode, WAZA[WAZA_あられ].effectCode, WAZA[WAZA_いちゃもん].effectCode, WAZA[WAZA_おにび].effectCode, WAZA[WAZA_ちょうはつ].effectCode, WAZA[WAZA_トリック].effectCode, WAZA[WAZA_あくび].effectCode, WAZA[WAZA_ブレイズキック].effectCode, WAZA[WAZA_どくどくのキバ].effectCode, WAZA[WAZA_ポイズンテール].effectCode, WAZA[WAZA_りゅうのまい].effectCode, WAZA[WAZA_つぼをつく].effectCode, WAZA[WAZA_とんぼがえり].effectCode, WAZA[WAZA_どくびし].effectCode, WAZA[WAZA_フレアドライブ].effectCode, WAZA[WAZA_トリックルーム].effectCode, WAZA[WAZA_うずしお].effectCode, WAZA[WAZA_ステルスロック].effectCode, WAZA[WAZA_ほのおのキバ].effectCode, WAZA[WAZA_こおりのキバ].effectCode, WAZA[WAZA_かみなりのキバ].effectCode, WAZA[WAZA_のしかかり].effectCode, WAZA[WAZA_ステルスロック].effectCode, WAZA[WAZA_ほのおのキバ].effectCode, WAZA[WAZA_こおりのキバ].effectCode, WAZA[WAZA_かみなりのキバ].effectCode, WAZA[WAZA_のしかかり].effectCode };

struct StatusCond {
	int cond;
	int turn; // もうどくターン
};

enum {
	STATUSCOND_NONE,
	STATUSCOND_POISON,
};

struct StackElem {
	vector<int> myHps;
	vector<StatusCond>	statusConds;
	int curr;
	vector<int> foeHps;
	int foeCurr;
	Waza fixedWaza;
	bool justAfterFoeExchanged;
	bool justAfterMyExchanged;
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
	bool justAfterMyExchanged = false;
	int atkMod = 0;
	stack<StackElem> stack;

	auto stackPush = [&]() {
		stack.push(StackElem{
			myHps,
			statusConds,
			curr,
			foeHps,
			foeCurr,
			fixedWaza,
			justAfterFoeExchanged,
			justAfterMyExchanged,
			atkMod,
		});
	};
	auto stackPop = [&]() {
		StackElem popped = stack.top();
		myHps = popped.myHps;
		statusConds = popped.statusConds;
		curr =  popped.curr;
		foeHps = popped.foeHps;
		foeCurr = popped.foeCurr;
		fixedWaza = popped.fixedWaza;
		justAfterFoeExchanged = popped.justAfterFoeExchanged;
		justAfterMyExchanged = popped.justAfterMyExchanged;
		atkMod = popped.atkMod;
		stack.pop();
	};
	function<bool(bool)> action;
	function<bool()> winnable;
	action = [&](bool speedLoss) {
		Poke my = mys[curr];
		Poke foe = foes[foeCurr];
		int myDmg2;
		if (any_of(foe->entry->waza, foe->entry->waza + 4, [](int waza) { return exist_in(NG_WAZA3_CODE, WAZA[waza].effectCode); })) {
			myDmg2 = myHps[curr];
		}
		else {
			myDmg2 = foeMaxWaza(my, foe);
		}
		// 敵が先制攻撃技持ち
		vector<Waza> foeWazas = get_wazas(foe);
		auto senseiWazaIt = find_if(foeWazas.begin(), foeWazas.end(), [](Waza w) { return w->effectCode == 103; });
		Waza senseiWaza = senseiWazaIt != foeWazas.end() ? *senseiWazaIt : 0;
		int myDmg = 0;
		if (!speedLoss && senseiWaza) {
			myDmg = calcDamage(foe, my, senseiWaza, true, false, 0);
			if (myHps[curr] <= myDmg) {
				stackPush();		
				myHps[curr] -= myDmg;
				justAfterMyExchanged = false;
				bool r = winnable();
				stackPop();
				return r;
			}
		}
		if (!speedLoss && exist_in(get_wazas(my), &WAZA[WAZA_みちづれ]) && !any_of(foe->entry->waza, foe->entry->waza + 4, [](int waza) { return exist_in(NG_WAZA3_CODE, WAZA[waza].effectCode); })) {
			stackPush();
			myHps[curr] = 0;
			foeHps[foeCurr] = 0;
			justAfterFoeExchanged = false;
			justAfterMyExchanged = false;
			bool r = winnable();
			stackPop();
			if (r) {
				return r;
			}
		}
		int dmg = 0;
		Waza waza = 0;
		if (fixedWaza) {
			waza = fixedWaza;
			dmg = calcDamage(my, foe, waza, false, statusConds[curr].cond != 0, atkMod);
		}
		else {
			// 一番ダメージの与えられる技を選択
			for (Waza w : get_wazas(my)) {
				if (w->no() != WAZA_とんぼがえり) {
					int d = min(calcDamage(my, foe, w, false, statusConds[curr].cond != 0, atkMod), foeHps[foeCurr]);
					if (dmg < d) {
						dmg = d;
						waza = w;
					}
				}
			}
		}
		if (!waza) {
			return false;
		}
		stackPush();
		justAfterFoeExchanged = false;
		justAfterMyExchanged = false;
		if (speedLoss) {
			myHps[curr] -= myDmg2;
			if (myHps[curr] <= 0) {
				bool r = winnable();
				stackPop();
				return r;
			}
		}
		int newHp = max(foeHps[foeCurr] - dmg, 0);
		if (foe->ability == ABIL_ゆうばく || (waza->isPhysical && foe->ability == ABIL_ほうし)) {
			stackPop();
			return false;
		}
		// 相手のHPが残っていて積み技などを持っていた場合失敗
		if (newHp > 0 && any_of(foe->entry->waza, foe->entry->waza + 4, [](int waza) { return exist_in(NG_WAZA2, waza); })) {
			stackPop();
			return false;
		}
		myHps[curr] -= myDmg;
		if (waza->no() == WAZA_ブレイブバード) {
			myHps[curr] -= dmg / 3;
		}
		if (my->item == ITEM_こだわりハチマキ) {
			fixedWaza = waza;
		}
		foeHps[foeCurr] = newHp;
		if (newHp > 0) { // 相手のHPが残っている
			myHps[curr] -= myDmg2;
		}
		if (statusConds[curr].cond == STATUSCOND_POISON) {
			int d = my->hp * min(statusConds[curr].turn, 15) / 16;
			myHps[curr] -= d;
			statusConds[curr].turn ++;
		}
		else if (my->item == ITEM_どくどくだま) {
			statusConds[curr].cond = STATUSCOND_POISON;
			statusConds[curr].turn = 1;
		}
		bool ret = winnable();
		stackPop();
		return ret;
	};
	winnable = [&]() {
		if (stack.size() >= 15) {
			return false;
		}
		Poke my = mys[curr];
		Poke foe = foes[foeCurr];
		//cout << my->entry->pokemon()->name << " " << foe->entry->pokemon()->name << endl;
		if (all_of(myHps.begin(), myHps.end(), [](int hp) { return hp <= 0; })) {
			return false; // 負け
		}
		if (all_of(foeHps.begin(), foeHps.end(), [](int hp) { return hp <= 0; })) {
			return true; // 勝ち
		}
		// 現在HPが0→交換	
		if (myHps[curr] <= 0) {
			vector<int> change = filter<int>(vector<int>({0, 1, 2}), [&](auto x) { return myHps[x] > 0; });
			for (int i : change) {
				stackPush();
				curr = i, fixedWaza = 0, atkMod = 0;
				justAfterMyExchanged = false;
				bool r = winnable();
				stackPop();
				if (r) return r;
			}
			return false;
		}
		if (foeHps[foeCurr] <= 0) {
			vector<int> change = filter<int>(vector<int>({ 0, 1, 2 }), [&](auto x) { return foeHps[x] > 0; });
			for (int i : change) {
				stackPush();
				foeCurr = i;
				justAfterFoeExchanged = true;
				bool r = winnable();
				stackPop();
				if (!r) return false;
			}
			return true;
		}
		if (justAfterFoeExchanged && (foe->ability == ABIL_いかく || (my->ability == ABIL_いかく && foe->ability == ABIL_トレ－ス))) {
			atkMod -= 1;
		}
		// たたかう場合
		if (calcComputedSpeed(my) <= calcComputedSpeed(foe)) {
			// 素早さ負ける
			bool ret = action(true);
			if (ret) return ret;
		}
		else {
			bool ret = action(false);
			if (ret) return ret;
		}
		// 交換する場合
		if (justAfterMyExchanged) {
			return false;
		}

		// 相手が積み技などを持っていた場合
		if (any_of(foe->entry->waza, foe->entry->waza + 4, [](int waza) { return exist_in(NG_WAZA2, waza); })) {
			return false;
		}
		vector<int> change = filter<int>(vector<int>({0, 1, 2}), [&](int x) { return myHps[x] > 0 && x != curr; });
		for (int c : change) {
			stackPush();
			int d = foeMaxWaza(my, foe);
			bool hasNgWaza = any_of(foe->entry->waza, foe->entry->waza + 4, [](int waza) { return exist_in(NG_WAZA3_CODE, WAZA[waza].effectCode); });
			if (mys[c]->item == ITEM_きあいのタスキ && myHps[c] == mys[c]->hp && myHps[c] <= d && !hasNgWaza) {
				myHps[c] = 1;
			}	
			else if (hasNgWaza) {
				myHps[c] = 0;
			}
			else {
				myHps[c] -= d;
			}
			curr = c;
			fixedWaza = 0, atkMod = 0;
			justAfterMyExchanged = true;
			bool ret = winnable();
			stackPop();
			if (ret) return ret;
		}
		return false;
	};
	return winnable();
}

int judge_seed(uint32_t seed) {
	LCG lcg(seed);
	int shuu = 3;
	bool is_open_level = false;
	bool is_hgss = false;
	int num_bonus = 1;
	vector<int> trainers = rand_trainers(lcg, shuu, is_hgss);
	vector<Entry> entries = rand_starter_entries(lcg, is_open_level, shuu, num_bonus);
	int suba_idx = indexof(apply(entries, [](Entry e) { return e->no(); }), 289);
	if (suba_idx < 0) return 0;
	vector<Poke> starters = starter_entiries_to_poke(is_open_level, shuu, 0, entries, rand_pid(lcg, entries));
	Poke suba = starters[suba_idx];
	lcg.step(2); // shuffle

	LCG l(lcg.seed);
	int idx = suba_idx == 0 ? 1 : 0;
	int idx2 = 5; // ボーナスポケをとる

	vector<Poke> playerPokes = { starters[idx2], starters[suba_idx], starters[idx] };

	vector<Poke> prev;
	int i;
	for (i = 1; i <= 1; i++) {
		vector<Entry> seen = apply(i == 1 ? starters : playerPokes + prev, [](Poke p) { return p->entry; });
		RankObject rank = trainer_id_to_rank(is_open_level, is_hgss, trainers[i - 1]);
		vector<Poke> pokes = rand_enemy_poke(l, rank, seen);

		bool win = completely_winnable(playerPokes, pokes, true);
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

int main() {
	int count = 0;
	int count_succeed = 0;
	for (uint64_t i = 0; i < 10000; i++) {
		int judged = judge_seed(i);
		if (judged > 0) count ++;
		if (judged == 2) count_succeed ++;
	}
	cout << count_succeed << " / " << count << endl;
}

