var Rank = {};

function RankObject(is_open_level, rank, range, iv) {
	this.is_open_level = is_open_level;
	this.rank = rank;
	this.range = range;
	this.iv = iv;
}

RankObject.prototype.toString = function () {
	return "Rank("+this.is_open_level+", "+this.rank+")";
};

var ENTRY_RANGES = [
	null,
	[  1, 150],
	[151, 250],
	[251, 350],
	[351, 486],
	[487, 622],
	[623, 758],
	[759, 894],
	[351, 950],
];

Rank.objects = {};
Rank.objects[false] = [];
Rank.objects[true] = [];

// Lv50
for (var i = 1; i <= 7; i ++) {
	Rank.objects[false][i] = new RankObject(false, i, ENTRY_RANGES[i], 4 * (i - 1));
}
Rank.objects[false][8] = new RankObject(false, 8, [351, 950], 31);

// Open
for (var i = 1; i <= 4; i ++) {
	Rank.objects[true][i] = new RankObject(true, i, ENTRY_RANGES[3 + i], 4 * (i - 1));
}
for (var i = 5; i <= 7; i ++) {
	Rank.objects[true][i] = new RankObject(true, i, [623, 950], 4 * (i - 1));
}
Rank.objects[true][8] = new RankObject(true, 8, [351, 950], 31);

// オープンレベルのランク5からランク7までのトレーナーの手持ちはスターターと違って範囲が広くなる
Rank.objectsOpenTrainer = Rank.objects[true].slice(0);
for (var i = 5; i <= 7; i ++) {
	Rank.objectsOpenTrainer[i] = new RankObject(true, i+"(トレーナー)", [351, 950], 4 * (i - 1));
}

Rank.silverNejiki = {};
Rank.silverNejiki[false] = {};
Rank.silverNejiki[true] = {};
Rank.silverNejiki[false][false] = Rank.objects[false][4]; // Lv50, Pt
Rank.silverNejiki[false][true]  = Rank.objects[false][4]; // Lv50, HGSS
Rank.silverNejiki[true][false] = new RankObject(true, "銀ネジキPt", [759, 950], 12); // Open, PT
Rank.silverNejiki[true][true] = new RankObject(true, "銀ネジキHGSS", [351, 486], 12); // Open, HGSS

Rank.goldNejiki = {};
Rank.goldNejiki[false] = {};
Rank.goldNejiki[true] = {};
Rank.goldNejiki[false][false] = new RankObject(false, "金ネジキPt", [351, 486], 31); // Lv50, Pt
Rank.goldNejiki[false][true] = new RankObject(false, "金ネジキHGSS", [759, 950], 31); // Lv50, HGSS
Rank.goldNejiki[true][false] = Rank.goldNejiki[true][true] =
	new RankObject(true, "金ネジキ", [759, 950], 31); // Open, both

function starter_rank(is_open_level, rank) {
	return Rank.objects[is_open_level][Math.min(rank, 8)];
}

function trainer_id_to_rank(is_open_level, is_hgss, id) {
	switch (id) {
	case silver_nejiki_id:
		return Rank.silverNejiki[is_open_level][is_hgss];
	case gold_nejiki_id:
		return Rank.goldNejiki[is_open_level][is_hgss];
	}
	var r = trainer_id_to_rank_basis(id);
	if (is_open_level) {
		return Rank.objectsOpenTrainer[r];
	}
	return Rank.objects[is_open_level][r];
}

function trainer_id_to_rank_basis(id) {
	if (id <= 99) return 1;
	if (id <= 119) return 2;
	if (id <= 139) return 3;
	if (id <= 159) return 4;
	if (id <= 179) return 5;
	if (id <= 199) return 6;
	if (id <= 219) return 7;
	if (id <= 299) return 8;
}

function rand_entry(lcg, rank) {
	var start = rank.range[0], end = rank.range[1];
	var count = end - start + 1;
	return factory_data[end - lcg.rand() % count];
}