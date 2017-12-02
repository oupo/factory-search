#include <map>
#include <iostream>
#include <random>
#include "damage.h"
#include "utils.h"
#include "factory.h"

// ref: http://www.smogon.com/dp/articles/damage_formula

const double SM_multiplier[] = { 2. / 8, 2. / 7, 2. / 6, 2. / 5, 2. / 4, 2. / 3, 2. / 2, 3. / 2, 4. / 2, 5. / 2, 6. / 2, 7. / 2, 8. / 2 };

int calcComputedSpeed(PokeStruct * poke) {
	int speed = poke->speed;
	speed *= SM_multiplier[poke->speedMod + 6];
	if (poke->statusCond == STATUSCOND_PARALYSIS) {
		speed *= 0.25;
	}
	if (poke->item == ITEM_こだわりスカーフ) {
		speed *= 1.5;
	}
	if (poke->item == ITEM_くろいてっきゅう) {
		speed *= 0.5;
	}
	return speed;
}

bool isToZero(PokeStruct * userPoke, PokeStruct * foePoke, Waza waza) {
	if (waza->type == TYPE_みず && (foePoke->ability == ABIL_ちょすい || foePoke->ability == ABIL_かんそうはだ))
		return true;
	if (waza->type == TYPE_じめん && foePoke->ability == ABIL_ふゆう)
		return true;
	if (waza->type == TYPE_ほのお && foePoke->ability == ABIL_もらいび)
		return true;
	if (waza->type == TYPE_でんき && (foePoke->ability == ABIL_ちくでん || foePoke->ability == ABIL_でんきエンジン))
		return true;
	// ignore ぼうおん
	return false;
}

double calcMod2(PokeStruct * userPoke) {
	// ignore メトロノーム、さきどり
	if (userPoke->item == ITEM_いのちのたま) {
		return 1.3;
	}
	return 1;
}

map<int, int> BERRY_MAP = {
	{ ITEM_ホズのみ,  0},
	{ ITEM_ヨプのみ, 1 },
	{ ITEM_バコウのみ, 2 },
	{ ITEM_ビアーのみ, 3 },
	{ ITEM_シュカのみ, 4 },
	{ ITEM_ヨロギのみ, 5 },
	{ ITEM_タンガのみ, 6 },
	{ ITEM_カシブのみ, 7 },
	{ ITEM_リリバのみ, 8 },
	{ ITEM_オッカのみ, 10 },
	{ ITEM_イトケのみ, 11 },
	{ ITEM_リンドのみ, 12 },
	{ ITEM_ソクノのみ, 13 },
	{ ITEM_ウタンのみ, 14 },
	{ ITEM_ヤチェのみ, 15 },
	{ ITEM_ハバンのみ, 16 },
	{ ITEM_ナモのみ, 17 },
};


double calcMod3(PokeStruct *userPoke, PokeStruct * foePoke, Waza waza, bool effective) {
	// ignore いろめがね(効果今ひとつの技が倍になる), ホズのみ(ノーマルタイプ半減)
	double mod3 = 1;
	if (foePoke->ability == ABIL_ハ－ドロック && effective) {
		mod3 *= 3.0 / 4;
	}
	if (userPoke->item == ITEM_たつじんのおび && effective) {
		mod3 *= 1.2;
	}
	// 半減実
	if (BERRY_MAP.find(foePoke->item) != BERRY_MAP.end() && BERRY_MAP[foePoke->item] == waza->type && effective) {
		mod3 *= 0.5;
	}
	return mod3;
}

int calcBasePower(PokeStruct *userPoke, PokeStruct *foePoke, Waza waza) {
	// ignore 威力が固定でない技
	int basePower = waza->power;
	if (basePower == 1) {
		throw string("unsupported waza:") + waza->name;
	}

	// ignore てだすけ(HH), じゅうでん(CHG), どろあそび(MS), みずあそび(WS)

	if (userPoke->item == ITEM_ちからのハチマキ && waza->isPhysical) {
		basePower = basePower * 1.1;
	}

	if (userPoke->item == ITEM_ものしりメガネ && !waza->isPhysical) {
		basePower = basePower * 1.1;
	}

	// ignore プレート, おこう
	// ignore こんごうだま, しらたま, はっきんだま

	// ignore とうそうしん, すてみ, てつのこぶし, もうか, しんりょく, げきりゅう, むしのしらせ, テクニシャン

	if (foePoke->ability == ABIL_あついしぼう && (waza->type == TYPE_ほのお || waza->type == TYPE_こおり)) {
		basePower = basePower * 0.5;
	}
	if (foePoke->ability == ABIL_たいねつ && waza->type == TYPE_ほのお) {
		basePower = basePower * 0.5;
	}
	if (foePoke->ability == ABIL_かんそうはだ && waza->type == TYPE_ほのお) {
		basePower = basePower * 1.25;
	}
	if (userPoke->statusCond && waza->no() == WAZA_からげんき) {
		basePower *= 2;
	}
	return basePower;
}

int calcAttack(PokeStruct *userPoke, PokeStruct *foePoke, Waza waza) {
	int atk = waza->isPhysical ? userPoke->atk : userPoke->spAtk;

	if (waza->isPhysical) {
		atk = atk * SM_multiplier[userPoke->atkMod + 6];
	}
	else {
		atk = atk * SM_multiplier[userPoke->spAtkMod + 6];
	}

	// ignore ヨガパワー ちからもち フラワーギフト こんじょう はりきり スロースタート プラス マイナス ソーラーパワー

	// ignore でんきだま ふといほね こころのしずく しんかいのキバ
	if (waza->isPhysical && userPoke->item == ITEM_こだわりハチマキ) {
		atk = atk * 1.5;
	}
	if (!waza->isPhysical && userPoke->item == ITEM_こだわりメガネ) {
		atk = atk * 1.5;
	}
	if (waza->isPhysical && userPoke->ability == ABIL_こんじょう && userPoke->statusCond) {
		atk = atk * 1.5;
	}
	return atk;
}


int calcDefence(PokeStruct * userPoke, PokeStruct * foePoke, Waza waza) {
	int def = waza->isPhysical ? foePoke->def : foePoke->spDef;

	if (waza->isPhysical) {
		def = def * SM_multiplier[foePoke->defMod + 6];
	}
	else {
		def = def * SM_multiplier[foePoke->spDefMod + 6];
	}

	// ignore じばく だいばくはつ
	// ignore メタルパウダー ふしぎなうろこ こころのしずく しんかいのウロコ
	// ignore 砂嵐下での岩タイプの特防アップ フラワーギフト

	return def;
}

int calcDamage0(int level, int basePower, int atk, int def, double mod1, double ch, double mod2, int r,	double stab, double type1, double type2, double mod3) {
	int x1 = (((level * 2 / 5) + 2) * basePower * atk / 50);
	int x2 = ((x1 / def) * mod1) + 2;
	int x3 = ((x2 * ch * mod2) * r / 100);
	int x4 = (((x3 * stab) * type1) * type2);
	int x5 = x4 * mod3;
	return x5;
}

int calcDamage(PokeStruct *userPoke, PokeStruct *foePoke, Waza waza, mt19937 &rnd) {
	if (waza->effectCode == 87) {
		return userPoke->level; // ナイトヘッドとちきゅうなげ
	}
	if (waza->effectCode == 38) {
		return foePoke->maxHp;
	}
	if (waza->power == 0) return 0;
	if (isToZero(userPoke, foePoke, waza)) return 0;
	double ch = 1;
	int r = 85 + rnd() % 16;
	
	if (rnd() % 16 == 0) {
		ch = 2;
		if (userPoke->ability == ABIL_スナイパ－) ch = 3;
	}
	double type1 = TYPE_CHART[waza->type][foePoke->type1];
	double type2 = foePoke->type1 == foePoke->type2 ? 1 : TYPE_CHART[waza->type][foePoke->type2];

	// Same Type Attack Bonus
	int stab = 1;
	if (waza->type == userPoke->type1 || waza->type == userPoke->type2) {
		if (userPoke->ability == ABIL_てきおうりょく) {
			stab = 2;
		}
		else {
			stab = 1.5;
		}
	}

	// ignore やけど、光の壁、リフレクター、ダブルバトル、雨、晴れ、もらい火
	double mod1 = 1;

	double mod2 = calcMod2(userPoke);
	double mod3 = calcMod3(userPoke, foePoke, waza, (type1*type2) > 1);

	int basePower = calcBasePower(userPoke, foePoke, waza);
	int atk = calcAttack(userPoke, foePoke, waza);
	int def = calcDefence(userPoke, foePoke, waza);
	int dmg = calcDamage0(userPoke->level, basePower, atk, def, mod1, ch, mod2, r, stab, type1, type2, mod3);
	return dmg;
}

int main() {
	mt19937 rnd(0);
	PokeStruct poke1 = gen_poke(starter_rank(false, 1), &ENTRIES[1], 0);
	PokeStruct poke2 = gen_poke(starter_rank(false, 1), &ENTRIES[2], 0);
	for (int i = 0; i < 200; i++) {
		cout << calcDamage(&poke1, &poke2, poke1.entry->getWaza(0), rnd) << endl;
	}
	return 0;
}