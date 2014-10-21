var factory_data = null;
var pokemon_data = null;
var pokemon_name2id = null;
var trainer_names = null;
var waza_data = null;
var waza_fromname = null;
var natures = "がんばりや さみしがり ゆうかん いじっぱり やんちゃ ずぶとい すなお のんき わんぱく のうてんき おくびょう せっかち まじめ ようき むじゃき ひかえめ おっとり れいせい てれや うっかりや おだやか おとなしい なまいき しんちょう きまぐれ".split(" ");
var rank_entries_count = [null, 150, 100, 100, 136, 136, 136, 136];
var rank_entries_start;
var rank_entries_end;

// rankという変数だけでポケモンがどの決定方法かを判別できるようにするため
// 以下の特殊なランクを設定する
var RANK_8_LV50                  =  8, // ランク8 (Lv50 最初の6匹)
    RANK_8_OPEN                  =  9, // ランク8 (オープン 最初の6匹)
    RANK_8_ENEMY                 = 10, // ランク8 (相手)
    RANK_OPEN_8                  = 11, // オープンレベル最初の6匹 8周目
    RANK_OPEN_SILVER_NEJIKI_PT   = 12, // オープン銀ネジキ(pt)
    RANK_OPEN_SILVER_NEJIKI_HGSS = 13, // オープン銀ネジキ(hgss)
    RANK_LV50_GOLD_NEJIKI_PT     = 14, // Lv50金ネジキ(pt)
    RANK_LV50_GOLD_NEJIKI_HGSS   = 15, // Lv50金ネジキ(hgss)
    RANK_OPEN_GOLD_NEJIKI        = 16; // オープン金ネジキ

(function() {
	function set(i, j) {
		// ランクjの最初から準伝説の最後までの範囲に設定
		var start = rank_entries_start[j];
		var count = 951 - start;
		rank_entries_start[i] = start;
		rank_entries_count[i] = count;
	}
	function copy(to, from) {
		rank_entries_count[to] = rank_entries_count[from];
		rank_entries_start[to] = rank_entries_start[from];
	}
	function set_start() {
		// ランク1～7までのrank_entries_startを設定
		rank_entries_start = [null];
		var n = 1;
		for (var i = 1; i <= 7; i ++) {
			rank_entries_start[i] = n;
			n += rank_entries_count[i];
		}
	}
	function set_end() {
		// すべてのランクのrank_entries_endを設定
		rank_entries_end = [null];
		for (var i = 1; i < rank_entries_count.length; i ++) {
			var count = rank_entries_count[i];
			var start = rank_entries_start[i];
			rank_entries_end[i] = start + count - 1;
		}
	}
	
	set_start();
	set(RANK_8_LV50, 4);
	set(RANK_8_OPEN, 6);
	set(RANK_8_ENEMY, 4);
	set(RANK_OPEN_8, 4);
	set(RANK_OPEN_SILVER_NEJIKI_PT, 7);
	copy(RANK_OPEN_SILVER_NEJIKI_HGSS, 4);
	copy(RANK_LV50_GOLD_NEJIKI_PT, 4);
	set(RANK_LV50_GOLD_NEJIKI_HGSS, 7);
	set(RANK_OPEN_GOLD_NEJIKI, 7);
	set_end();
})();

function initialize_factory(callback) {
	var factory_data_text;
	var pokedex_csv_text;
	var trainer_names_text;
	var waza_csv_text;
	$.ajax({dataType: "text", url: "factory_data.txt", success: function(data) {
		factory_data_text = data;
		boot();
	}});
	$.ajax({dataType: "text", url: "pokedex.csv", success: function(data) {
		pokedex_csv_text = data;
		boot();
	}});
	$.ajax({dataType: "text", url: "trainer.txt", success: function(data) {
		trainer_names_text = data;
		boot();
	}});
	$.ajax({dataType: "text", url: "waza.csv", success: function(data) {
		waza_csv_text = data;
		boot();
	}});
	function boot() {
		if (!factory_data_text || !pokedex_csv_text || !trainer_names_text || !waza_csv_text) return;
		initialize_pokemon_data(pokedex_csv_text);
		initialize_factory_entries(factory_data_text);
		initialize_trainer_names(trainer_names_text);
		initialize_waza(waza_csv_text);
		callback();
	}
}

function initialize_pokemon_data(pokedex_csv_text) {
	var lines = pokedex_csv_text.split("\n");
	if (lines[lines.length - 1] === "") lines.pop();
	var boundaries = {"♂のみ": -1, "♀のみ": 255, "1:7": 30, "1:3": 63, "1:1": 126, "3:1": 190, "ふめい": null};
	pokemon_data = new Array(lines.length);
	pokemon_name2id = {};
	for (var i = 0; i < lines.length; i ++) {
		var row = lines[i].split(",");
		var name = row[0];
		var stats = row.slice(1, 1 + 6).map(Number);
		var ability1 = row[7];
		var ability2 = row[8] || ability1;
		var gender_boundary = boundaries[row[9]];
		var type = row[10].split("/");
		pokemon_name2id[name] = i;
		pokemon_data[i] = {
			id: i,
			name: name,
			stats: stats,
			abilities: [ability1, ability2],
			gender_boundary: gender_boundary,
			type1: type[0],
			type2: type[1] || type[0],
		};
	}
}

function initialize_factory_entries(factory_data_text) {
	var lines = factory_data_text.split("\n");
	if (lines[lines.length - 1] === "") lines.pop();
	factory_data = new Array(1 + lines.length);
	for (var i = 0; i < lines.length; i ++) {
		var row = lines[i].split("|");
		var pokemon = get_pokemon_entry(row[1]);
		factory_data[1 + i] = {
			id: 1 + i,
			name:   row[1],
			pokemon: pokemon,
			nature: natures.indexOf(row[2]),
			item:   row[3],
			move:   row[4].split(","),
			effort: omit_effort_text(row[5])
		};
	}
}

var silver_nejiki_id;
var gold_nejiki_id;

function initialize_trainer_names(trainer_names_text) {
	var lines = trainer_names = trainer_names_text.split("\n");
	if (lines[lines.length - 1] === "") lines.pop();
	// 300番目,301番目のトレーナーをそれぞれ銀ネジキ、金ネジキとして扱う
	silver_nejiki_id = lines.length;
	lines.push("ファクトリーヘッドのネジキ");
	gold_nejiki_id = lines.length;
	lines.push("ファクトリーヘッドのネジキ");
}

function initialize_waza(waza_csv_text) {
	var lines = waza_csv_text.split("\n");
	lines.shift(); // ヘッダ行を除去
	if (lines[lines.length - 1] === "") lines.pop();
	waza_data = new Array(1 + lines.length);
	waza_fromname = {};
	for (var i = 0; i < lines.length; i ++) {
		var row = lines[i].split(",");
		var name = row[1];
		var basePower = Number(row[2]);
		var accuracy = Number(row[3]);
		var type = row[5];
		var isPhysical = row[6] == "物理";
		waza_data[i + 1] = {
			id: i + 1,
			name: name,
			basePower: basePower,
			accuracy: accuracy,
			type: type,
			isPhysical: isPhysical,
		};
		waza_fromname[name] = waza_data[i + 1];
	}
}


function get_trainers_candidate(seed_head, shuu, back_seeds, is_hgss) {
	var i = 13;
	var seed = step_seed(seed_head, -i);
	var result = [];
	back_seeds = back_seeds.concat(0); // Array#concatは破壊的ではないので引数の配列を変更していない
	while (i < 50) {
		// 50個前のseedまで決めうちで遡るってのはダサいけどきちんとループの終了条件を確かめるのは面倒なので
		var r = get_trainers_by_seed(shuu, seed, is_hgss);
		if (ary_include(back_seeds, i - r.consumption)) {
			result.push({start: -i,
			             seed: seed,
			             c: r.c,
			             trainer_ids: r.trainer_ids,
			             trainer_ids2: r.trainer_ids2});
		}
		i ++;
		seed = prev_seed(seed);
	}
	return result;
}

// 相手の3匹決定後の乱数消費量を取得
function get_enemy_consuption(shuu, nth, enemy_rank) {
	var c;
	if (shuu >= 5) {
		c = 24;
	} else if(enemy_rank === 1) {
		c = 6;
	} else {
		c = 12;
	}
	if (nth === 1) {
		c += shuu === 1 ? 18 : 36;
	}
	return c;
}

function trainer_id_to_name(i) {
	return /の([^の]+)$/.exec(trainer_names[i])[1];
}

function read_int_string(s, default_value) {
	if (/^\s*$/.test(s) && default_value !== undefined) {
		return default_value;
	}
	if (!/^\s*(?:-?\d+|0x[0-9a-f]+)\s*$/i.test(s)) {
		return null;
	}
	return Number(s);
}

function entry_id_to_rank(id) {
	for (var i = 1; i <= 7; i ++) {
		if (id <= rank_entries_end[i]) return i;
	}
	return 8;
}

function get_individual_by_rank(rank) {
	if (rank <= 7) {
		return (rank - 1) * 4;
	}
	switch(rank) {
	case RANK_8_LV50:
	case RANK_8_OPEN:
	case RANK_8_ENEMY:
	case RANK_OPEN_8:
		return 31;
	case RANK_OPEN_SILVER_NEJIKI_PT:
	case RANK_OPEN_SILVER_NEJIKI_HGSS:
		return 12;
	case RANK_LV50_GOLD_NEJIKI_PT:
	case RANK_LV50_GOLD_NEJIKI_HGSS:
	case RANK_OPEN_GOLD_NEJIKI:
		return 31;
	default:
		UNREACHABLE();
	}
}

var nature_table = new Array(25);
(function() {
	var fix = [0, 1, 4, 2, 3]; // HABSCD順をHABCDS順に
	for (var i = 0; i < 25; i ++) {
		var row = [10, 10, 10, 10, 10];
		row[fix[i / 5 | 0]] ++;
		row[fix[i % 5]] --;
		nature_table[i] = row;
	}
})();

var status_names = "HP,攻撃,防御,特攻,特防,すば".split(",");

function omit_effort_text(text) {
	// ex. omit_effort_text("HP/攻撃")
	//  => "HA"
	var list = text.split("/");
	var result = "";
	for (var i = 0; i < list.length; i ++) {
		result += "HABCDS".charAt(status_names.indexOf(list[i]));
	}
	return result;
}

function effort_text_to_array(text) {
	// ex. effort_text_to_array("HA")
	//  => [255, 255, 0, 0, 0, 0]
	var t = "HABCDS";
	var efforts = [0, 0, 0, 0, 0, 0];
	for (var i = 0; i < text.length; i ++) {
		efforts[t.indexOf(text.charAt(i))] = text.length === 3 ? 170 : 255;
	}
	return efforts;
}

// damage.jsに渡すためのポケモンデータを作る
function gen_poke(rank, is_open_level, entry, ability_index) {
	var status = get_status(rank, is_open_level, entry);
	var pokemon = entry.pokemon;
	return {
		name: pokemon.name,
		level: is_open_level ? 100 : 50,
		hp: status[0],
		atk: status[1],
		def: status[2],
		spAtk: status[3],
		spDef: status[4],
		speed: status[5],
		item: entry.item,
		ability: pokemon.abilities[ability_index],
		type1: pokemon.type1,
		type2: pokemon.type2,
		waza: entry.move,
		entry: entry
	};
}

function gen_enemy_pokes(rank, is_open_level, seed, my) {
	var myEntries = my.map(function(x) { return x.entry });
	var result = get_3_entries_info(fix_rank_enemy(rank, is_open_level), seed, 0, myEntries, null);
	return result.infos.map(function (info) {
		return gen_poke(rank, is_open_level, info.entry, info.pid % 2);
	});
}

function get_status(rank, is_open_level, entry) {
	// ex. get_status(1, false, find_factory_entry(1, "フシギダネ"))
	// => [105, 48, 54, 111, 101, 50]
	// 注) rankはfix_rankされていない値
	var lv = is_open_level ? 100 : 50;
	var individual = get_individual_by_rank(rank);
	var stats = entry.pokemon.stats;
	var efforts = effort_text_to_array(entry.effort);
	var hp = calc_status_hp(lv, stats[0], individual, efforts[0]);
	if (stats[0] === 1) hp = 1; // ヌケニン
	var result = [hp];
	var nature_revisions = nature_table[entry.nature];
	for (var i = 0; i < 5; i ++) {
		result.push(calc_status(lv, stats[i + 1], individual, efforts[i + 1], nature_revisions[i]));
	}
	return result;
}

function calc_status_hp(lv, base, individual, effort) {
	return Math.floor((base * 2 + Math.floor(effort / 4) + individual) * lv / 100) + lv + 10;
}

function calc_status(lv, base, individual, effort, nature_revision) {
	return Math.floor((Math.floor((base * 2 + Math.floor(effort / 4) + individual) * lv / 100) + 5) * nature_revision / 10);
}

function get_pokemon_entry(name) {
	return pokemon_data[pokemon_name2id[name]];
}

function pid2gender(pid, pokemon) {
	var boundary = pokemon.gender_boundary;
	if (boundary !== null) {
		return (pid & 0xff) > boundary ? "♂" : "♀";
	} else {
		return "不明";
	}
}

function get_trainers_by_seed(shuu, seed, is_hgss) {
	// トレーナーA
	var r = get_trainers_by_seed0(shuu, seed, 7, [], true);
	// トレーナーB
	//   2周目と6周目は6人分しか決定しない
	//   プラチナでは7人目も6人目までと同じ決定方法
	var r2 = get_trainers_by_seed0(shuu, step_seed(seed, r.c),
	                               (shuu === 2 || shuu === 6) ? 6 : 7, r.trainer_ids,
	                               is_hgss);
	return {trainer_ids: r.trainer_ids,
	        trainer_ids2: r2.trainer_ids,
	        c: r.c,
	        consumption: r.c + r2.c};
}

function get_trainers_by_seed0(shuu, seed, num, visited, is_last_trainer_next_rank) {
	var result = [];
	var c = 0;
	var i = 1;
	while (i <= num) {
		if ((shuu === 3 || shuu === 7) && i === 7 && visited.length === 0) {
			result.push(shuu === 3 ? silver_nejiki_id : gold_nejiki_id);
			break;
		}
		var e = seed_to_trainer_id(shuu, is_last_trainer_next_rank && i === 7, seed);
		seed = next_seed(seed);
		c ++;
		if (ary_include(result, e) || ary_include(visited, e)) continue;
		result.push(e);
		i ++;
	}
	return {trainer_ids: result, c: c};
}

function seed_to_trainer_id(shuu, is_last, seed) {
	var r = seed >>> 16;
	if (shuu >= 8) {
		return r % 99 + 200;
	} else if (is_last) {
		return r % 19 + 100 + (shuu - 1) * 20;
	} else if (shuu === 1) {
		return r % 99;
	} else {
		return r % 39 + 80 + (shuu - 2) * 20;
	}
}

function trainer_id_to_rank(id, is_open_level, is_hgss) {
	if (id < 0) UNREACHABLE();
	if (id <= 99) return 1;
	if (id <= 119) return 2;
	if (id <= 139) return 3;
	if (id <= 159) return 4;
	if (id <= 179) return 5;
	if (id <= 199) return 6;
	if (id <= 219) return 7;
	if (id <= 299) return RANK_8_ENEMY;
	if (id === silver_nejiki_id) {
		if (is_open_level) {
			return is_hgss ? RANK_OPEN_SILVER_NEJIKI_HGSS : RANK_OPEN_SILVER_NEJIKI_PT;
		} else {
			return 4;
		}
	}
	if (id === gold_nejiki_id) {
		if (is_open_level) {
			return RANK_OPEN_GOLD_NEJIKI;
		} else {
			return is_hgss ? RANK_LV50_GOLD_NEJIKI_HGSS : RANK_LV50_GOLD_NEJIKI_PT;
		}
	}
	if (id >= dummy_trainer_start_id) {
		var rank = (id - dummy_trainer_start_id) + 1;
		if (rank === 8) rank = RANK_8_ENEMY;
		return rank;
	}
	UNREACHABLE();
}

function seed_to_entry(rank, seed) {
	return factory_data[seed_to_entry_id(rank, seed)];
}

function seed_to_entry_id(rank, seed) {
	var count = rank_entries_count[rank];
	var end   = rank_entries_end[rank];
	return end - (seed >>> 16) % count;
}

function get_3_entries_info(rank, seed, consumption, visited_entries, entries_in_hand) {
	return get_entries_info(rank, null, seed, consumption, 3, visited_entries, entries_in_hand, 0);
}

function get_6_entries_info(shuu, is_open_level, seed, consumption, num_bonus) {
	var rank = fix_rank(shuu, is_open_level);
	var bonus_rank = fix_rank(shuu + 1, is_open_level);
	return get_entries_info(rank, bonus_rank, seed, consumption, 6, [], null, num_bonus);
}

function get_entries_info(rank, bonus_rank, seed, consumption, num, visited_entries, entries_in_hand, num_bonus) {
	var entries = [];
	var infos = [];
	var i = 0;
	var seed_start = seed;
	var c = consumption;
	while (i < num) {
		var is_bonus = num - i <= num_bonus;
		var entry_rank = is_bonus ? bonus_rank : rank;
		var entry = seed_to_entry(entry_rank, seed);
		seed = next_seed(seed); c ++;
		if (entries_collision(entry, entries, visited_entries)) continue;
		entries.push(entry);
		infos.push({entry: entries[i],
		            parent_id: null,
		            secret_id: null,
		            pid: null,
		            is_bonus: is_bonus});
		i ++;
	}
	
	for (var i = 0; i < num; i ++) {
		var parent_id = seed >>> 16;
		seed = next_seed(seed); c ++;
		var secret_id = seed >>> 16;
		seed = next_seed(seed); c ++;
		var nature = entries[i].nature;
		do {
			var pid_low = seed >>> 16;
			seed = next_seed(seed); c ++;
			var pid_high = seed >>> 16;
			seed = next_seed(seed); c ++;
			var pid = (pid_high << 16 | pid_low) >>> 0;
		} while (pid % 25 !== nature || is_shiny_pid(parent_id, secret_id, pid));
		infos[i].parent_id = parent_id;
		infos[i].secret_id = secret_id;
		infos[i].pid = pid;
	}
	
	var range = {first: consumption, last: c - 1};
	
	var raw_entries = entries;
	if (num === 6) {
		entries = entries.slice();
		var swap1 = (seed >>> 16) % 6;
		seed = next_seed(seed); c ++;
		var swap2 = (seed >>> 16) % 6;
		seed = next_seed(seed); c ++;
		
		ary_swap(infos, 4, swap1);
		ary_swap(infos, 5, swap2);
		
		ary_swap(entries, 4, swap1);
		ary_swap(entries, 5, swap2);
	}
	
	return {entries: entries,
	        raw_entries: raw_entries,
	        infos: infos,
	        seed: seed,
	        seed_start: seed_start,
	        rand_range: range,
	        next_consumption: c,
	        entries_in_hand: entries_in_hand,
	        visited_entries: visited_entries,
	        rank: rank,
	        bonus_rank: bonus_rank,
	        num_bonus: num_bonus};
}

function entries_collision(entry, entries, visited_entries) {
	return entries_collision0(entry, entries) || entries_collision0(entry, visited_entries);
}

function entries_collision0(entry, entries) {
	for (var i = 0; i < entries.length; i ++) {
		if (entry.pokemon === entries[i].pokemon) return true;
		if (entry.item === entries[i].item) return true;
	}
	return false;
}

function find_collided_entry(entry, entries, visited_entries) {
	return find_collided_entry0(entry, entries) || find_collided_entry0(entry, visited_entries);
}

function find_collided_entry0(entry, entries) {
	for (var i = 0; i < entries.length; i ++) {
		if (entry.pokemon === entries[i].pokemon) return entry;
		if (entry.item === entries[i].item) return entry;
	}
	return null;
}

function is_shiny_pid(parent_id, secret_id, pid) {
	return ((parent_id ^ secret_id ^ (pid >>> 16) ^ (pid & 0xffff)) & ~7) === 0;
}

function fix_rank(rank, is_open_level) {
	var fixed_rank = rank;
	if (is_open_level) {
		fixed_rank += 3;
	}
	if (is_open_level && rank >= 8) { // オープン8周目
		fixed_rank = RANK_OPEN_8;
	} else if (fixed_rank >= 8) {
		fixed_rank = is_open_level ? RANK_8_OPEN : RANK_8_LV50;
	}
	return fixed_rank;
}

function fix_rank_enemy(rank, is_open_level) {
	if (is_open_level && rank <= 7) {
		var fixed_rank = rank + 3;
		if (fixed_rank >= 8) {
			fixed_rank = RANK_8_ENEMY;
		}
		return fixed_rank;
	} else {
		return rank;
	}
}

function limit_rank(rank) {
	return rank > 8 ? 8 : rank;
}

function unfix_rank(rank, is_open_level) {
	if (is_open_level) {
		return rank - 3;
	} else {
		return rank;
	}
}

function ary_eq(a, b) {
	if (a === b) return true;
	if (a.length !== b.length) return false;
	var l = a.length;
	for (var i = 0; i < l; i ++) {
		if (a[i] !== b[i]) return false;
	}
	return true;
}

function ary_swap(array, i, j) {
	var tmp = array[i];
	array[i] = array[j];
	array[j] = tmp;
}

function format_hex(n, prec) {
	var s = n.toString(16);
	return "0x" + (str_repeat("0", prec - s.length) + s);
}

function format_dec(n, prec, c) {
	var s = String(n);
	return (str_repeat(c || "0", prec - s.length) + s);
}

function str_repeat(s, n) {
	var r = "";
	for (var i = 0; i < n; i ++) {
		r += s;
	}
	return r;
}

function UNREACHABLE() {
	throw new Error("unreachable");
}

function step_seed(seed, n) {
	var fn = next_seed;
	if (n < 0) {
		fn = prev_seed;
		n = -n;
	}
	for (var i = 0; i < n; i ++) {
		seed = fn(seed);
	}
	return seed >>> 0;
}

function next_seed(seed) {
	return mul(seed, 0x41c64e6d) + 0x6073 >>> 0;
}

function prev_seed(seed) {
	return mul(seed, 0xeeb9eb65) + 0xa3561a1 >>> 0;
}

function mul(a, b) {
	var a1 = a >>> 16, a2 = a & 0xffff;
	var b1 = b >>> 16, b2 = b & 0xffff;
	return (((a1 * b2 + a2 * b1) << 16) + a2 * b2) >>> 0;
}
