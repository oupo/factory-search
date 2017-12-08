"use strict";

// ref: http://www.smogon.com/dp/articles/damage_formula

function calcComputedSpeed(poke, weather, trickRoom) {
	var speed = poke.speed;
	if (poke.item == "こだわりスカーフ") {
		speed = int(speed * 1.5);
	}
	if (poke.item == "くろいてっきゅう") {
		speed = int(speed * 0.5);
	}
	if (poke.ability == "すいすい" && weather == "雨") {
		speed = int(speed * 2);
	}
	if (trickRoom) {
		return -speed;
	}
	return speed;
}

function calcDamage(userPoke, foePoke, waza, weather, options) {
	if (!options) options = {};
	if (waza.effectCode == 87) {
		return userPoke.level; // ナイトヘッドとちちゅうなげ
	}
	if (waza.basePower <= 1) return 0;
	if (!(options.max || options.ignoreAccuracy) && waza.accuracy < 100 && waza.effectCode != 17) return 0;
	if (!(options.max || options.ignoreAccuracy) && (foePoke.item == "ひかりのこな" || foePoke.item == "のんきのおこう")) return 0;
	if (isToZero(userPoke, foePoke, waza)) return 0;
	var ch = 1; // 急所はなし
	var r = 85; // 最小乱数

	if (options.max) {
		ch = 2;
		r = 100;
		if (userPoke.ability == "スナイパー") ch = 3;
	}
	var type1 = typeChart(waza.type, foePoke.type1);
	var type2 = foePoke.type1 == foePoke.type2 ? 1 : typeChart(waza.type, foePoke.type2);

	// Same Type Attack Bonus
	var stab = 1;
	if (waza.type == userPoke.type1 || waza.type == userPoke.type2) {
		if (userPoke.ability == "てきおうりょく") {
			stab = 2;
		} else {
			stab = 1.5;
		}
	}

	// ignore やけど、光の壁、リフレクター、ダブルバトル、雨、晴れ、もらい火
	var mod1 = 1;
	if (userPoke.ability == "こんじょう" && options.hasStatusCondition) {
		mod1 *= 1.5;
	}
	if (options.hasStatusCondition && waza.name == "からげんき") {
		mod1 *= 2;
	}
	if (weather == "雨" && waza.type == "みず") {
		mod1 *= 1.5;
	}

	var mod2 = calcMod2(userPoke);
	var mod3 = calcMod3(userPoke, foePoke, waza, (type1*type2) > 1);

	var basePower = calcBasePower(userPoke, foePoke, waza);
	var atk = calcAttack(userPoke, foePoke, waza, options);
	var def = calcDefence(userPoke, foePoke, waza);
	var dmg = calcDamage0(userPoke.level, basePower, atk, def, mod1, ch, mod2, r, stab, type1, type2, mod3);

	if (!options.max && (foePoke.item == "きあいのタスキ" || foePoke.item == "きあいのハチマキ")) {
		dmg = Math.min(dmg, foePoke.hp - 1);
	}
	return dmg;
}

function isToZero(userPoke, foePoke, waza) {
	if (waza.type == "みず" && (foePoke.ability == "ちょすい" || foePoke.ability == "かんそうはだ"))
		return true;
	if (waza.type == "じめん" && foePoke.ability == "ふゆう")
		return true;
	if (waza.type == "ほのお" && foePoke.ability == "もらいび")
		return true;
	if (waza.type == "でんき" && (foePoke.ability == "ちくでん" || foePoke.ability == "でんきエンジン"))
		return true;
	// ignore ぼうおん
	return false;
}

function calcMod2(userPoke) {
	// ignore メトロノーム、さきどり
	if (userPoke.item == "いのちのたま") {
		return 1.3;
	}
	return 1;
}

function calcMod3(userPoke, foePoke, waza, effective) {
	// 一つ掛けるたびに小数点以下切り下げしないといけないので配列を返す

	// ignore いろめがね(効果今ひとつの技が倍になる), ホズのみ(ノーマルタイプ半減)
	var mod3 = [];
	if (foePoke.ability == "ハードロック" && effective) {
		mod3.push(3/4);
	}
	if (userPoke.item == "たつじんのおび" && effective) {
		mod3.push(1.2);
	}
	// 半減実
	if (BERRY_NAMES.indexOf(foePoke.item) == TYPENAMES.indexOf(waza.type) && effective) {
		mod3.push(0.5);
	}
	return mod3;
}

function calcBasePower(userPoke, foePoke, waza, effective) {
	// ignore 威力が固定でない技
	var basePower = waza.basePower;

	// ignore てだすけ(HH), じゅうでん(CHG), どろあそび(MS), みずあそび(WS)

	if (userPoke.item == "ちからのハチマキ" && waza.isPhysical) {
		basePower = int(basePower * 1.1);
	}
	
	if (userPoke.item == "ものしりメガネ" && !waza.isPhysical) {
		basePower = int(basePower * 1.1);
	}

	// ignore プレート, おこう
	// ignore こんごうだま, しらたま, はっきんだま
	
	// ignore とうそうしん, すてみ, てつのこぶし, もうか, しんりょく, げきりゅう, むしのしらせ, テクニシャン

	if (foePoke.ability == "あついしぼう" && (waza.type == "ほのお" || waza.type == "こおり")) {
		basePower = int(basePower * 0.5);
	}
	if (foePoke.ability == "たいねつ" && waza.type == "ほのお") {
		basePower = int(basePower * 0.5);
	}
	if (foePoke.ability == "かんそうはだ" && waza.type == "ほのお") {
		basePower = int(basePower * 1.25);
	}
	return basePower;
}

function calcAttack(userPoke, foePoke, waza, options) {
	var atk = waza.isPhysical ? userPoke.atk : userPoke.spAtk;

	// 能力変化 (こうげきのみ)
	var SM_multiplier = [2/8, 2/7, 2/6, 2/5, 2/4, 2/3, 2/2, 3/2, 4/2, 5/2, 6/2, 7/2, 8/2];
	if (waza.isPhysical && options.atkMod != null) {
		atk = int(atk * SM_multiplier[options.atkMod + 6]);
	}

	// ignore ヨガパワー ちからもち フラワーギフト こんじょう はりきり スロースタート プラス マイナス ソーラーパワー

	// ignore でんきだま ふといほね こころのしずく しんかいのキバ
	if (waza.isPhysical && userPoke.item == "こだわりハチマキ") {
		atk = int(atk * 1.5);
	}
	if (waza.isPhysical && userPoke.item == "ふといホネ") {
		atk = int(atk * 2);
	}
	if (!waza.isPhysical && userPoke.item == "こだわりメガネ") {
		atk = int(atk * 1.5);
	}
	return atk;
}

function calcDefence(userPoke, foePoke, waza) {
	var def = waza.isPhysical ?  foePoke.def :  foePoke.spDef;
	
	if (waza.name == "だいばくはつ") {
		def *= 0.5;
	}

	// ignore 能力変化
	// ignore じばく だいばくはつ
	// ignore メタルパウダー ふしぎなうろこ こころのしずく しんかいのウロコ
	// ignore 砂嵐下での岩タイプの特防アップ フラワーギフト

	return def;
}

var TYPENAMES = ["ノーマル", "かくとう", "ひこう", "どく", "じめん", "いわ", "むし", "ゴースト", "はがね", "ほのお", "みず", "くさ", "でんき", "エスパー", "こおり", "ドラゴン", "あく"];

var BERRY_NAMES = ["ホズのみ", "ヨプのみ", "バコウのみ", "ビアーのみ", "シュカのみ", "ヨロギのみ", "タンガのみ", "カシブのみ", "リリバのみ", "オッカのみ", "イトケのみ", "リンドのみ", "ソクノのみ", "ウタンのみ", "ヤチェのみ", "ハバンのみ", "ナモのみ"];

var TYPE_CHART = [
	[1,   1,   1,   1,   1, 0.5,   1,   0, 0.5,   1,   1,   1,   1,   1,   1,   1,   1],
	[2,   1, 0.5, 0.5,   1,   2, 0.5,   0,   2,   1,   1,   1,   1, 0.5,   2,   1,   2],
	[1,   2,   1,   1,   1, 0.5,   2,   1, 0.5,   1,   1,   2, 0.5,   1,   1,   1,   1],
	[1,   1,   1, 0.5, 0.5, 0.5,   1, 0.5,   0,   1,   1,   2,   1,   1,   1,   1,   1],
	[1,   1,   0,   2,   1,   2, 0.5,   1,   2,   2,   1, 0.5,   2,   1,   1,   1,   1],
	[1, 0.5,   2,   1, 0.5,   1,   2,   1, 0.5,   2,   1,   1,   1,   1,   2,   1,   1],
	[1, 0.5, 0.5, 0.5,   1,   1,   1, 0.5, 0.5, 0.5,   1,   2,   1,   2,   1,   1,   2],
	[0,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   2,   1,   1, 0.5],
	[1,   1,   1,   1,   1,   2,   1,   1, 0.5, 0.5, 0.5,   1, 0.5,   1,   2,   1,   1],
	[1,   1,   1,   1,   1, 0.5,   2,   1,   2, 0.5, 0.5,   2,   1,   1,   2, 0.5,   1],
	[1,   1,   1,   1,   2,   2,   1,   1,   1,   2, 0.5, 0.5,   1,   1,   1, 0.5,   1],
	[1,   1, 0.5, 0.5,   2,   2, 0.5,   1, 0.5, 0.5,   2, 0.5,   1,   1,   1, 0.5,   1],
	[1,   1,   2,   1,   0,   1,   1,   1,   1,   1,   2, 0.5, 0.5,   1,   1, 0.5,   1],
	[1,   2,   1,   2,   1,   1,   1,   1, 0.5,   1,   1,   1,   1, 0.5,   1,   1,   0],
	[1,   1,   2,   1,   2,   1,   1,   1, 0.5, 0.5, 0.5,   2,   1,   1, 0.5,   2,   1],
	[1,   1,   1,   1,   1,   1,   1,   1, 0.5,   1,   1,   1,   1,   1,   1,   2,   1],
	[1, 0.5,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   2,   1,   1, 0.5],
];

function typeChart(wazaType, foeType) {
	return TYPE_CHART[TYPENAMES.indexOf(wazaType)][TYPENAMES.indexOf(foeType)];
}

var int = Math.floor;

function calcDamage0(level, basePower, atk, def, mod1, ch, mod2, r, stab, type1, type2, mod3) {
	var x1 = int((int(level * 2 / 5) + 2) * basePower * atk / 50);
	var x2 = int(int(x1 / def) * mod1) + 2;
	var x3 = int(int(x2 * ch * mod2) * r / 100);
	var x4 = int(int(int(x3 * stab) * type1) * type2);
	mod3.forEach (function (m) {
		x4 = int(x4 * m);
	});
	return x4;
}

function damageTest() {
	console.log(calcDamage0(47, 60, 140, 77, 1, 1, 1, 85, 1.5, 2, 1, []));
	console.log(calcDamage({level: 47, atk: 140, type1: "ノーマル", type2: "ひこう"}, {def: 77, type1: "くさ", type2: "どく"}, {basePower: 60, isPhysical: true, type: "ひこう"}));
}
