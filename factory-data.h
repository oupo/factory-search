#pragma once
#include <memory>

using namespace std;

struct PokemonSpecStruct;
struct WazaStruct;
struct EntryStruct;

extern const int NATURE_TABLE[25][5];
extern const char *TYPE_NAME[18];
extern const double TYPE_CHART[18][18];
extern const PokemonSpecStruct POKEMON_SPEC[494];
extern const WazaStruct WAZA[468];
extern const EntryStruct ENTRIES[951];

typedef const PokemonSpecStruct *PokemonSpec;
typedef const WazaStruct *Waza;
typedef const EntryStruct *Entry;

struct PokemonSpecStruct {
	char name[40];
	int stats[6];
	int ability1;
	int ability2;
	int type1;
	int type2;

	int no() const {
		return this - POKEMON_SPEC;
	}
};

struct WazaStruct {
	char name[40];
	int power;
	int accuracy;
	int effectCode;
	int type;
	int isPhysical;

	int no() const {
		return this - WAZA;
	}
};

struct EntryStruct {
	int pokemonNo;
	int itemNo;
	int waza[4];
	int nature;
	int effort[6];

	PokemonSpec pokemon() const {
		return &POKEMON_SPEC[this->pokemonNo];
	}

	int no() const {
		return this - ENTRIES;
	}

	Waza getWaza(int i) const {
		return &WAZA[this->waza[i]];
	}
};

enum {
	STATUSCOND_NONE,
	STATUSCOND_PARALYSIS,
};

struct PokeStruct {
	Entry entry;
	int level;
	int maxHp;
	int atk;
	int def;
	int	spAtk;
	int spDef;
	int speed;
	int item;
	int ability;
	int type1;
	int type2;
	int hp;
	Waza wazas[4];
	int atkMod;
	int defMod;
	int spAtkMod;
	int spDefMod;
	int speedMod;
	int statusCond;
};

typedef shared_ptr<PokeStruct> Poke;

enum {
TYPE_ノーマル = 0,
TYPE_かくとう,
TYPE_ひこう,
TYPE_どく,
TYPE_じめん,
TYPE_いわ,
TYPE_むし,
TYPE_ゴ－スト,
TYPE_はがね,
TYPE_？？？,
TYPE_ほのお,
TYPE_みず,
TYPE_くさ,
TYPE_でんき,
TYPE_エスパ－,
TYPE_こおり,
TYPE_ドラゴン,
TYPE_あく,
};

enum {
ITEM_マスターボール = 1,
ITEM_ハイパーボール,
ITEM_スーパーボール,
ITEM_モンスターボール,
ITEM_サファリボール,
ITEM_ネットボール,
ITEM_ダイブボール,
ITEM_ネストボール,
ITEM_リピートボール,
ITEM_タイマーボール,
ITEM_ゴージャスボール,
ITEM_プレミアボール,
ITEM_ダークボール,
ITEM_ヒールボール,
ITEM_クイックボール,
ITEM_プレシャスボール,
ITEM_キズぐすり,
ITEM_どくけし,
ITEM_やけどなおし,
ITEM_こおりなおし,
ITEM_ねむけざまし,
ITEM_まひなおし,
ITEM_かいふくのくすり,
ITEM_まんたんのくすり,
ITEM_すごいキズぐすり,
ITEM_いいキズぐすり,
ITEM_なんでもなおし,
ITEM_げんきのかけら,
ITEM_げんきのかたまり,
ITEM_おいしいみず,
ITEM_サイコソーダ,
ITEM_ミックスオレ,
ITEM_モーモーミルク,
ITEM_ちからのこな,
ITEM_ちからのねっこ,
ITEM_ばんのうごな,
ITEM_ふっかつそう,
ITEM_ピーピーエイド,
ITEM_ピーピーリカバー,
ITEM_ピーピーエイダー,
ITEM_ピーピーマックス,
ITEM_フエンせんべい,
ITEM_きのみジュース,
ITEM_せいなるはい,
ITEM_マックスアップ,
ITEM_タウリン,
ITEM_ブロムヘキシン,
ITEM_インドメタシン,
ITEM_リゾチウム,
ITEM_ふしぎなアメ,
ITEM_ポイントアップ,
ITEM_キトサン,
ITEM_ポイントマックス,
ITEM_もりのヨウカン,
ITEM_エフェクトガード,
ITEM_クリティカッター,
ITEM_プラスパワー,
ITEM_ディフェンダー,
ITEM_スピーダー,
ITEM_ヨクアタール,
ITEM_スペシャルアップ,
ITEM_スペシャルガード,
ITEM_ピッピにんぎょう,
ITEM_エネコのシッポ,
ITEM_あおいビードロ,
ITEM_きいろビードロ,
ITEM_あかいビードロ,
ITEM_くろいビードロ,
ITEM_しろいビードロ,
ITEM_あさせのしお,
ITEM_あさせのかいがら,
ITEM_あかいかけら,
ITEM_あおいかけら,
ITEM_きいろいかけら,
ITEM_みどりのかけら,
ITEM_シルバースプレー,
ITEM_ゴールドスプレー,
ITEM_あなぬけのヒモ,
ITEM_むしよけスプレー,
ITEM_たいようのいし,
ITEM_つきのいし,
ITEM_ほのおのいし,
ITEM_かみなりのいし,
ITEM_みずのいし,
ITEM_リーフのいし,
ITEM_ちいさなキノコ,
ITEM_おおきなキノコ,
ITEM_しんじゅ,
ITEM_おおきなしんじゅ,
ITEM_ほしのすな,
ITEM_ほしのかけら,
ITEM_きんのたま,
ITEM_ハートのウロコ,
ITEM_あまいミツ,
ITEM_すくすくこやし,
ITEM_じめじめこやし,
ITEM_ながながこやし,
ITEM_ねばねばこやし,
ITEM_ねっこのカセキ,
ITEM_ツメのカセキ,
ITEM_かいのカセキ,
ITEM_こうらのカセキ,
ITEM_ひみつのコハク,
ITEM_たてのカセキ,
ITEM_ずがいのカセキ,
ITEM_きちょうなホネ,
ITEM_ひかりのいし,
ITEM_やみのいし,
ITEM_めざめいし,
ITEM_まんまるいし,
ITEM_かなめいし,
ITEM_はっきんだま,

ITEM_こんごうだま = 135,
ITEM_しらたま,
ITEM_グラスメール,
ITEM_フレイムメール,
ITEM_ブルーメール,
ITEM_ブルームメール,
ITEM_トンネルメール,
ITEM_スチールメール,
ITEM_ラブラブメール,
ITEM_ブリザードメール,
ITEM_スペースメール,
ITEM_エアメール,
ITEM_モザイクメール,
ITEM_ブリックメール,
ITEM_クラボのみ,
ITEM_カゴのみ,
ITEM_モモンのみ,
ITEM_チーゴのみ,
ITEM_ナナシのみ,
ITEM_ヒメリのみ,
ITEM_オレンのみ,
ITEM_キーのみ,
ITEM_ラムのみ,
ITEM_オボンのみ,
ITEM_フィラのみ,
ITEM_ウイのみ,
ITEM_マゴのみ,
ITEM_バンジのみ,
ITEM_イアのみ,
ITEM_ズリのみ,
ITEM_ブリーのみ,
ITEM_ナナのみ,
ITEM_セシナのみ,
ITEM_パイルのみ,
ITEM_ザロクのみ,
ITEM_ネコブのみ,
ITEM_タポルのみ,
ITEM_ロメのみ,
ITEM_ウブのみ,
ITEM_マトマのみ,
ITEM_モコシのみ,
ITEM_ゴスのみ,
ITEM_ラブタのみ,
ITEM_ノメルのみ,
ITEM_ノワキのみ,
ITEM_シーヤのみ,
ITEM_カイスのみ,
ITEM_ドリのみ,
ITEM_ベリブのみ,
ITEM_オッカのみ,
ITEM_イトケのみ,
ITEM_ソクノのみ,
ITEM_リンドのみ,
ITEM_ヤチェのみ,
ITEM_ヨプのみ,
ITEM_ビアーのみ,
ITEM_シュカのみ,
ITEM_バコウのみ,
ITEM_ウタンのみ,
ITEM_タンガのみ,
ITEM_ヨロギのみ,
ITEM_カシブのみ,
ITEM_ハバンのみ,
ITEM_ナモのみ,
ITEM_リリバのみ,
ITEM_ホズのみ,
ITEM_チイラのみ,
ITEM_リュガのみ,
ITEM_カムラのみ,
ITEM_ヤタピのみ,
ITEM_ズアのみ,
ITEM_サンのみ,
ITEM_スターのみ,
ITEM_ナゾのみ,
ITEM_ミクルのみ,
ITEM_イバンのみ,
ITEM_ジャポのみ,
ITEM_レンブのみ,
ITEM_ひかりのこな,
ITEM_しろいハーブ,
ITEM_きょうせいギプス,
ITEM_がくしゅうそうち,
ITEM_せんせいのツメ,
ITEM_やすらぎのすず,
ITEM_メンタルハーブ,
ITEM_こだわりハチマキ,
ITEM_おうじゃのしるし,
ITEM_ぎんのこな,
ITEM_おまもりこばん,
ITEM_きよめのおふだ,
ITEM_こころのしずく,
ITEM_しんかいのキバ,
ITEM_しんかいのウロコ,
ITEM_けむりだま,
ITEM_かわらずのいし,
ITEM_きあいのハチマキ,
ITEM_しあわせタマゴ,
ITEM_ピントレンズ,
ITEM_メタルコート,
ITEM_たべのこし,
ITEM_りゅうのウロコ,
ITEM_でんきだま,
ITEM_やわらかいすな,
ITEM_かたいいし,
ITEM_きせきのタネ,
ITEM_くろいメガネ,
ITEM_くろおび,
ITEM_じしゃく,
ITEM_しんぴのしずく,
ITEM_するどいくちばし,
ITEM_どくバリ,
ITEM_とけないこおり,
ITEM_のろいのおふだ,
ITEM_まがったスプーン,
ITEM_もくたん,
ITEM_りゅうのキバ,
ITEM_シルクのスカーフ,
ITEM_アップグレード,
ITEM_かいがらのすず,
ITEM_うしおのおこう,
ITEM_のんきのおこう,
ITEM_ラッキーパンチ,
ITEM_メタルパウダー,
ITEM_ふといホネ,
ITEM_ながねぎ,
ITEM_あかいバンダナ,
ITEM_あおいバンダナ,
ITEM_ピンクのバンダナ,
ITEM_みどりのバンダナ,
ITEM_きいろのバンダナ,
ITEM_こうかくレンズ,
ITEM_ちからのハチマキ,
ITEM_ものしりメガネ,
ITEM_たつじんのおび,
ITEM_ひかりのねんど,
ITEM_いのちのたま,
ITEM_パワフルハーブ,
ITEM_どくどくだま,
ITEM_かえんだま,
ITEM_スピードパウダー,
ITEM_きあいのタスキ,
ITEM_フォーカスレンズ,
ITEM_メトロノーム,
ITEM_くろいてっきゅう,
ITEM_こうこうのしっぽ,
ITEM_あかいいと,
ITEM_くろいヘドロ,
ITEM_つめたいいわ,
ITEM_さらさらいわ,
ITEM_あついいわ,
ITEM_しめったいわ,
ITEM_ねばりのかぎづめ,
ITEM_こだわりスカーフ,
ITEM_くっつきバリ,
ITEM_パワーリスト,
ITEM_パワーベルト,
ITEM_パワーレンズ,
ITEM_パワーバンド,
ITEM_パワーアンクル,
ITEM_パワーウエイト,
ITEM_きれいなぬけがら,
ITEM_おおきなねっこ,
ITEM_こだわりメガネ,
ITEM_ひのたまプレート,
ITEM_しずくプレート,
ITEM_いかずちプレート,
ITEM_みどりのプレート,
ITEM_つららのプレート,
ITEM_こぶしのプレート,
ITEM_もうどくプレート,
ITEM_だいちのプレート,
ITEM_あおぞらプレート,
ITEM_ふしぎのプレート,
ITEM_たまむしプレート,
ITEM_がんせきプレート,
ITEM_もののけプレート,
ITEM_りゅうのプレート,
ITEM_こわもてプレート,
ITEM_こうてつプレート,
ITEM_あやしいおこう,
ITEM_がんせきおこう,
ITEM_まんぷくおこう,
ITEM_さざなみのおこう,
ITEM_おはなのおこう,
ITEM_こううんのおこう,
ITEM_きよめのおこう,
ITEM_プロテクター,
ITEM_エレキブースター,
ITEM_マグマブースター,
ITEM_あやしいパッチ,
ITEM_れいかいのぬの,
ITEM_するどいツメ,
ITEM_するどいキバ,
ITEM_わざマシン０１,
ITEM_わざマシン０２,
ITEM_わざマシン０３,
ITEM_わざマシン０４,
ITEM_わざマシン０５,
ITEM_わざマシン０６,
ITEM_わざマシン０７,
ITEM_わざマシン０８,
ITEM_わざマシン０９,
ITEM_わざマシン１０,
ITEM_わざマシン１１,
ITEM_わざマシン１２,
ITEM_わざマシン１３,
ITEM_わざマシン１４,
ITEM_わざマシン１５,
ITEM_わざマシン１６,
ITEM_わざマシン１７,
ITEM_わざマシン１８,
ITEM_わざマシン１９,
ITEM_わざマシン２０,
ITEM_わざマシン２１,
ITEM_わざマシン２２,
ITEM_わざマシン２３,
ITEM_わざマシン２４,
ITEM_わざマシン２５,
ITEM_わざマシン２６,
ITEM_わざマシン２７,
ITEM_わざマシン２８,
ITEM_わざマシン２９,
ITEM_わざマシン３０,
ITEM_わざマシン３１,
ITEM_わざマシン３２,
ITEM_わざマシン３３,
ITEM_わざマシン３４,
ITEM_わざマシン３５,
ITEM_わざマシン３６,
ITEM_わざマシン３７,
ITEM_わざマシン３８,
ITEM_わざマシン３９,
ITEM_わざマシン４０,
ITEM_わざマシン４１,
ITEM_わざマシン４２,
ITEM_わざマシン４３,
ITEM_わざマシン４４,
ITEM_わざマシン４５,
ITEM_わざマシン４６,
ITEM_わざマシン４７,
ITEM_わざマシン４８,
ITEM_わざマシン４９,
ITEM_わざマシン５０,
ITEM_わざマシン５１,
ITEM_わざマシン５２,
ITEM_わざマシン５３,
ITEM_わざマシン５４,
ITEM_わざマシン５５,
ITEM_わざマシン５６,
ITEM_わざマシン５７,
ITEM_わざマシン５８,
ITEM_わざマシン５９,
ITEM_わざマシン６０,
ITEM_わざマシン６１,
ITEM_わざマシン６２,
ITEM_わざマシン６３,
ITEM_わざマシン６４,
ITEM_わざマシン６５,
ITEM_わざマシン６６,
ITEM_わざマシン６７,
ITEM_わざマシン６８,
ITEM_わざマシン６９,
ITEM_わざマシン７０,
ITEM_わざマシン７１,
ITEM_わざマシン７２,
ITEM_わざマシン７３,
ITEM_わざマシン７４,
ITEM_わざマシン７５,
ITEM_わざマシン７６,
ITEM_わざマシン７７,
ITEM_わざマシン７８,
ITEM_わざマシン７９,
ITEM_わざマシン８０,
ITEM_わざマシン８１,
ITEM_わざマシン８２,
ITEM_わざマシン８３,
ITEM_わざマシン８４,
ITEM_わざマシン８５,
ITEM_わざマシン８６,
ITEM_わざマシン８７,
ITEM_わざマシン８８,
ITEM_わざマシン８９,
ITEM_わざマシン９０,
ITEM_わざマシン９１,
ITEM_わざマシン９２,
ITEM_ひでんマシン０１,
ITEM_ひでんマシン０２,
ITEM_ひでんマシン０３,
ITEM_ひでんマシン０４,
ITEM_ひでんマシン０５,
ITEM_ひでんマシン０６,
ITEM_ひでんマシン０７,
ITEM_ひでんマシン０８,
ITEM_？？？,
ITEM_たからぶくろ,
ITEM_ルールブック,
ITEM_ポケトレ,
ITEM_ポイントカード,
ITEM_ぼうけんノート,
ITEM_シールいれ,
ITEM_アクセサリーいれ,
ITEM_シールぶくろ,
ITEM_ともだちてちょう,
ITEM_はつでんしょキー,
ITEM_こだいのおまもり,
ITEM_ギンガだんのカギ,
ITEM_あかいくさり,
ITEM_タウンマップ,
ITEM_バトルサーチャー,
ITEM_コインケース,
ITEM_ボロのつりざお,
ITEM_いいつりざお,
ITEM_すごいつりざお,
ITEM_コダックじょうろ,
ITEM_ポフィンケース,
ITEM_じてんしゃ,
ITEM_ルームキー,
ITEM_オーキドのてがみ,
ITEM_みかづきのはね,
ITEM_メンバーズカード,
ITEM_てんかいのふえ,
ITEM_ふねのチケット,
ITEM_コンテストパス,
ITEM_かざんのおきいし,
ITEM_おとどけもの,
ITEM_ひきかえけん１,
ITEM_ひきかえけん２,
ITEM_ひきかえけん３,
ITEM_そうこのカギ,
ITEM_ひでんのくすり,
ITEM_バトルレコーダー,
ITEM_グラシデアのはな,
ITEM_ひみつのカギ,
ITEM_ぼんぐりケース,
ITEM_アンノーンノート,
ITEM_きのみプランター,
ITEM_ダウジングマシン,
ITEM_ブルーカード,
ITEM_おいしいシッポ,
ITEM_とうめいなスズ,
ITEM_カードキー,
ITEM_ちかのかぎ,
ITEM_ゼニガメじょうろ,
ITEM_あかいウロコ,
ITEM_おとしもの,
ITEM_リニアパス,
ITEM_きかいのぶひん,
ITEM_ぎんいろのはね,
ITEM_にじいろのはね,
ITEM_ふしぎなタマゴ,
ITEM_あかぼんぐり,
ITEM_きぼんぐり,
ITEM_あおぼんぐり,
ITEM_みどぼんぐり,
ITEM_ももぼんぐり,
ITEM_しろぼんぐり,
ITEM_くろぼんぐり,
ITEM_スピードボール,
ITEM_レベルボール,
ITEM_ルアーボール,
ITEM_ヘビーボール,
ITEM_ラブラブボール,
ITEM_フレンドボール,
ITEM_ムーンボール,
ITEM_コンペボール,
ITEM_パークボール,
ITEM_フォトアルバム,
ITEM_ＧＢプレイヤー,
ITEM_うみなりのスズ,
ITEM_いかりまんじゅう,
ITEM_データカード０１,
ITEM_データカード０２,
ITEM_データカード０３,
ITEM_データカード０４,
ITEM_データカード０５,
ITEM_データカード０６,
ITEM_データカード０７,
ITEM_データカード０８,
ITEM_データカード０９,
ITEM_データカード１０,
ITEM_データカード１１,
ITEM_データカード１２,
ITEM_データカード１３,
ITEM_データカード１４,
ITEM_データカード１５,
ITEM_データカード１６,
ITEM_データカード１７,
ITEM_データカード１８,
ITEM_データカード１９,
ITEM_データカード２０,
ITEM_データカード２１,
ITEM_データカード２２,
ITEM_データカード２３,
ITEM_データカード２４,
ITEM_データカード２５,
ITEM_データカード２６,
ITEM_データカード２７,
ITEM_もえぎいろのたま,
ITEM_ロックカプセル,
ITEM_べにいろのたま,
ITEM_あいいろのたま,
ITEM_なぞのすいしょう,
};

enum {
ABIL_あくしゅう = 1,
ABIL_あめふらし,
ABIL_かそく,
ABIL_カブトア－マ－,
ABIL_がんじょう,
ABIL_しめりけ,
ABIL_じゅうなん,
ABIL_すながくれ,
ABIL_せいでんき,
ABIL_ちくでん,
ABIL_ちょすい,
ABIL_どんかん,
ABIL_ノ－てんき,
ABIL_ふくがん,
ABIL_ふみん,
ABIL_へんしょく,
ABIL_めんえき,
ABIL_もらいび,
ABIL_りんぷん,
ABIL_マイペ－ス,
ABIL_きゅうばん,
ABIL_いかく,
ABIL_かげふみ,
ABIL_さめはだ,
ABIL_ふしぎなまもり,
ABIL_ふゆう,
ABIL_ほうし,
ABIL_シンクロ,
ABIL_クリアボディ,
ABIL_しぜんかいふく,
ABIL_ひらいしん,
ABIL_てんのめぐみ,
ABIL_すいすい,
ABIL_ようりょくそ,
ABIL_はっこう,
ABIL_トレ－ス,
ABIL_ちからもち,
ABIL_どくのトゲ,
ABIL_せいしんりょく,
ABIL_マグマのよろい,
ABIL_みずのベ－ル,
ABIL_じりょく,
ABIL_ぼうおん,
ABIL_あめうけざら,
ABIL_すなおこし,
ABIL_プレッシャ－,
ABIL_あついしぼう,
ABIL_はやおき,
ABIL_ほのおのからだ,
ABIL_にげあし,
ABIL_するどいめ,
ABIL_かいりきバサミ,
ABIL_ものひろい,
ABIL_なまけ,
ABIL_はりきり,
ABIL_メロメロボディ,
ABIL_プラス,
ABIL_マイナス,
ABIL_てんきや,
ABIL_ねんちゃく,
ABIL_だっぴ,
ABIL_こんじょう,
ABIL_ふしぎなうろこ,
ABIL_ヘドロえき,
ABIL_しんりょく,
ABIL_もうか,
ABIL_げきりゅう,
ABIL_むしのしらせ,
ABIL_いしあたま,
ABIL_ひでり,
ABIL_ありじごく,
ABIL_やるき,
ABIL_しろいけむり,
ABIL_ヨガパワ－,
ABIL_シェルア－マ－,
ABIL_エアロック,
ABIL_ちどりあし,
ABIL_でんきエンジン,
ABIL_とうそうしん,
ABIL_ふくつのこころ,
ABIL_ゆきがくれ,
ABIL_くいしんぼう,
ABIL_いかりのつぼ,
ABIL_かるわざ,
ABIL_たいねつ,
ABIL_たんじゅん,
ABIL_かんそうはだ,
ABIL_ダウンロ－ド,
ABIL_てつのこぶし,
ABIL_ポイズンヒ－ル,
ABIL_てきおうりょく,
ABIL_スキルリンク,
ABIL_うるおいボディ,
ABIL_サンパワ－,
ABIL_はやあし,
ABIL_ノ－マルスキン,
ABIL_スナイパ－,
ABIL_マジックガ－ド,
ABIL_ノ－ガ－ド,
ABIL_あとだし,
ABIL_テクニシャン,
ABIL_リ－フガ－ド,
ABIL_ぶきよう,
ABIL_かたやぶり,
ABIL_きょううん,
ABIL_ゆうばく,
ABIL_きけんよち,
ABIL_よちむ,
ABIL_てんねん,
ABIL_いろめがね,
ABIL_フィルタ－,
ABIL_スロ－スタ－ト,
ABIL_きもったま,
ABIL_よびみず,
ABIL_アイスボディ,
ABIL_ハ－ドロック,
ABIL_ゆきふらし,
ABIL_みつあつめ,
ABIL_おみとおし,
ABIL_すてみ,
ABIL_マルチタイプ,
ABIL_フラワ－ギフト,
ABIL_ナイトメア,
};

enum {
WAZA_はたく = 1,
WAZA_からてチョップ,
WAZA_おうふくビンタ,
WAZA_れんぞくパンチ,
WAZA_メガトンパンチ,
WAZA_ネコにこばん,
WAZA_ほのおのパンチ,
WAZA_れいとうパンチ,
WAZA_かみなりパンチ,
WAZA_ひっかく,
WAZA_はさむ,
WAZA_ハサミギロチン,
WAZA_かまいたち,
WAZA_つるぎのまい,
WAZA_いあいぎり,
WAZA_かぜおこし,
WAZA_つばさでうつ,
WAZA_ふきとばし,
WAZA_そらをとぶ,
WAZA_しめつける,
WAZA_たたきつける,
WAZA_つるのムチ,
WAZA_ふみつけ,
WAZA_にどげり,
WAZA_メガトンキック,
WAZA_とびげり,
WAZA_まわしげり,
WAZA_すなかけ,
WAZA_ずつき,
WAZA_つのでつく,
WAZA_みだれづき,
WAZA_つのドリル,
WAZA_たいあたり,
WAZA_のしかかり,
WAZA_まきつく,
WAZA_とっしん,
WAZA_あばれる,
WAZA_すてみタックル,
WAZA_しっぽをふる,
WAZA_どくばり,
WAZA_ダブルニードル,
WAZA_ミサイルばり,
WAZA_にらみつける,
WAZA_かみつく,
WAZA_なきごえ,
WAZA_ほえる,
WAZA_うたう,
WAZA_ちょうおんぱ,
WAZA_ソニックブーム,
WAZA_かなしばり,
WAZA_ようかいえき,
WAZA_ひのこ,
WAZA_かえんほうしゃ,
WAZA_しろいきり,
WAZA_みずでっぽう,
WAZA_ハイドロポンプ,
WAZA_なみのり,
WAZA_れいとうビーム,
WAZA_ふぶき,
WAZA_サイケこうせん,
WAZA_バブルこうせん,
WAZA_オーロラビーム,
WAZA_はかいこうせん,
WAZA_つつく,
WAZA_ドリルくちばし,
WAZA_じごくぐるま,
WAZA_けたぐり,
WAZA_カウンター,
WAZA_ちきゅうなげ,
WAZA_かいりき,
WAZA_すいとる,
WAZA_メガドレイン,
WAZA_やどりぎのタネ,
WAZA_せいちょう,
WAZA_はっぱカッター,
WAZA_ソーラービーム,
WAZA_どくのこな,
WAZA_しびれごな,
WAZA_ねむりごな,
WAZA_はなびらのまい,
WAZA_いとをはく,
WAZA_りゅうのいかり,
WAZA_ほのおのうず,
WAZA_でんきショック,
WAZA_10まんボルト,
WAZA_でんじは,
WAZA_かみなり,
WAZA_いわおとし,
WAZA_じしん,
WAZA_じわれ,
WAZA_あなをほる,
WAZA_どくどく,
WAZA_ねんりき,
WAZA_サイコキネシス,
WAZA_さいみんじゅつ,
WAZA_ヨガのポーズ,
WAZA_こうそくいどう,
WAZA_でんこうせっか,
WAZA_いかり,
WAZA_テレポート,
WAZA_ナイトヘッド,
WAZA_ものまね,
WAZA_いやなおと,
WAZA_かげぶんしん,
WAZA_じこさいせい,
WAZA_かたくなる,
WAZA_ちいさくなる,
WAZA_えんまく,
WAZA_あやしいひかり,
WAZA_からにこもる,
WAZA_まるくなる,
WAZA_バリアー,
WAZA_ひかりのかべ,
WAZA_くろいきり,
WAZA_リフレクター,
WAZA_きあいだめ,
WAZA_がまん,
WAZA_ゆびをふる,
WAZA_オウムがえし,
WAZA_じばく,
WAZA_タマゴばくだん,
WAZA_したでなめる,
WAZA_スモッグ,
WAZA_ヘドロこうげき,
WAZA_ホネこんぼう,
WAZA_だいもんじ,
WAZA_たきのぼり,
WAZA_からではさむ,
WAZA_スピードスター,
WAZA_ロケットずつき,
WAZA_とげキャノン,
WAZA_からみつく,
WAZA_ドわすれ,
WAZA_スプーンまげ,
WAZA_タマゴうみ,
WAZA_とびひざげり,
WAZA_へびにらみ,
WAZA_ゆめくい,
WAZA_どくガス,
WAZA_たまなげ,
WAZA_きゅうけつ,
WAZA_あくまのキッス,
WAZA_ゴッドバード,
WAZA_へんしん,
WAZA_あわ,
WAZA_ピヨピヨパンチ,
WAZA_キノコのほうし,
WAZA_フラッシュ,
WAZA_サイコウェーブ,
WAZA_はねる,
WAZA_とける,
WAZA_クラブハンマー,
WAZA_だいばくはつ,
WAZA_みだれひっかき,
WAZA_ホネブーメラン,
WAZA_ねむる,
WAZA_いわなだれ,
WAZA_ひっさつまえば,
WAZA_かくばる,
WAZA_テクスチャー,
WAZA_トライアタック,
WAZA_いかりのまえば,
WAZA_きりさく,
WAZA_みがわり,
WAZA_わるあがき,
WAZA_スケッチ,
WAZA_トリプルキック,
WAZA_どろぼう,
WAZA_クモのす,
WAZA_こころのめ,
WAZA_あくむ,
WAZA_かえんぐるま,
WAZA_いびき,
WAZA_のろい,
WAZA_じたばた,
WAZA_テクスチャー2,
WAZA_エアロブラスト,
WAZA_わたほうし,
WAZA_きしかいせい,
WAZA_うらみ,
WAZA_こなゆき,
WAZA_まもる,
WAZA_マッハパンチ,
WAZA_こわいかお,
WAZA_だましうち,
WAZA_てんしのキッス,
WAZA_はらだいこ,
WAZA_ヘドロばくだん,
WAZA_どろかけ,
WAZA_オクタンほう,
WAZA_まきびし,
WAZA_でんじほう,
WAZA_みやぶる,
WAZA_みちづれ,
WAZA_ほろびのうた,
WAZA_こごえるかぜ,
WAZA_みきり,
WAZA_ボーンラッシュ,
WAZA_ロックオン,
WAZA_げきりん,
WAZA_すなあらし,
WAZA_ギガドレイン,
WAZA_こらえる,
WAZA_あまえる,
WAZA_ころがる,
WAZA_みねうち,
WAZA_いばる,
WAZA_ミルクのみ,
WAZA_スパーク,
WAZA_れんぞくぎり,
WAZA_はがねのつばさ,
WAZA_くろいまなざし,
WAZA_メロメロ,
WAZA_ねごと,
WAZA_いやしのすず,
WAZA_おんがえし,
WAZA_プレゼント,
WAZA_やつあたり,
WAZA_しんぴのまもり,
WAZA_いたみわけ,
WAZA_せいなるほのお,
WAZA_マグニチュード,
WAZA_ばくれつパンチ,
WAZA_メガホーン,
WAZA_りゅうのいぶき,
WAZA_バトンタッチ,
WAZA_アンコール,
WAZA_おいうち,
WAZA_こうそくスピン,
WAZA_あまいかおり,
WAZA_アイアンテール,
WAZA_メタルクロー,
WAZA_あてみなげ,
WAZA_あさのひざし,
WAZA_こうごうせい,
WAZA_つきのひかり,
WAZA_めざめるパワー,
WAZA_クロスチョップ,
WAZA_たつまき,
WAZA_あまごい,
WAZA_にほんばれ,
WAZA_かみくだく,
WAZA_ミラーコート,
WAZA_じこあんじ,
WAZA_しんそく,
WAZA_げんしのちから,
WAZA_シャドーボール,
WAZA_みらいよち,
WAZA_いわくだき,
WAZA_うずしお,
WAZA_ふくろだたき,
WAZA_ねこだまし,
WAZA_さわぐ,
WAZA_たくわえる,
WAZA_はきだす,
WAZA_のみこむ,
WAZA_ねっぷう,
WAZA_あられ,
WAZA_いちゃもん,
WAZA_おだてる,
WAZA_おにび,
WAZA_おきみやげ,
WAZA_からげんき,
WAZA_きあいパンチ,
WAZA_きつけ,
WAZA_このゆびとまれ,
WAZA_しぜんのちから,
WAZA_じゅうでん,
WAZA_ちょうはつ,
WAZA_てだすけ,
WAZA_トリック,
WAZA_なりきり,
WAZA_ねがいごと,
WAZA_ねこのて,
WAZA_ねをはる,
WAZA_ばかぢから,
WAZA_マジックコート,
WAZA_リサイクル,
WAZA_リベンジ,
WAZA_かわらわり,
WAZA_あくび,
WAZA_はたきおとす,
WAZA_がむしゃら,
WAZA_ふんか,
WAZA_スキルスワップ,
WAZA_ふういん,
WAZA_リフレッシュ,
WAZA_おんねん,
WAZA_よこどり,
WAZA_ひみつのちから,
WAZA_ダイビング,
WAZA_つっぱり,
WAZA_ほごしょく,
WAZA_ほたるび,
WAZA_ラスターパージ,
WAZA_ミストボール,
WAZA_フェザーダンス,
WAZA_フラフラダンス,
WAZA_ブレイズキック,
WAZA_どろあそび,
WAZA_アイスボール,
WAZA_ニードルアーム,
WAZA_なまける,
WAZA_ハイパーボイス,
WAZA_どくどくのキバ,
WAZA_ブレイククロー,
WAZA_ブラストバーン,
WAZA_ハイドロカノン,
WAZA_コメットパンチ,
WAZA_おどろかす,
WAZA_ウェザーボール,
WAZA_アロマセラピー,
WAZA_うそなき,
WAZA_エアカッター,
WAZA_オーバーヒート,
WAZA_かぎわける,
WAZA_がんせきふうじ,
WAZA_ぎんいろのかぜ,
WAZA_きんぞくおん,
WAZA_くさぶえ,
WAZA_くすぐる,
WAZA_コスモパワー,
WAZA_しおふき,
WAZA_シグナルビーム,
WAZA_シャドーパンチ,
WAZA_じんつうりき,
WAZA_スカイアッパー,
WAZA_すなじごく,
WAZA_ぜったいれいど,
WAZA_だくりゅう,
WAZA_タネマシンガン,
WAZA_つばめがえし,
WAZA_つららばり,
WAZA_てっぺき,
WAZA_とおせんぼう,
WAZA_とおぼえ,
WAZA_ドラゴンクロー,
WAZA_ハードプラント,
WAZA_ビルドアップ,
WAZA_とびはねる,
WAZA_マッドショット,
WAZA_ポイズンテール,
WAZA_ほしがる,
WAZA_ボルテッカー,
WAZA_マジカルリーフ,
WAZA_みずあそび,
WAZA_めいそう,
WAZA_リーフブレード,
WAZA_りゅうのまい,
WAZA_ロックブラスト,
WAZA_でんげきは,
WAZA_みずのはどう,
WAZA_はめつのねがい,
WAZA_サイコブースト,
WAZA_はねやすめ,
WAZA_じゅうりょく,
WAZA_ミラクルアイ,
WAZA_めざましビンタ,
WAZA_アームハンマー,
WAZA_ジャイロボール,
WAZA_いやしのねがい,
WAZA_しおみず,
WAZA_しぜんのめぐみ,
WAZA_フェイント,
WAZA_ついばむ,
WAZA_おいかぜ,
WAZA_つぼをつく,
WAZA_メタルバースト,
WAZA_とんぼがえり,
WAZA_インファイト,
WAZA_しっぺがえし,
WAZA_ダメおし,
WAZA_さしおさえ,
WAZA_なげつける,
WAZA_サイコシフト,
WAZA_きりふだ,
WAZA_かいふくふうじ,
WAZA_しぼりとる,
WAZA_パワートリック,
WAZA_いえき,
WAZA_おまじない,
WAZA_さきどり,
WAZA_まねっこ,
WAZA_パワースワップ,
WAZA_ガードスワップ,
WAZA_おしおき,
WAZA_とっておき,
WAZA_なやみのタネ,
WAZA_ふいうち,
WAZA_どくびし,
WAZA_ハートスワップ,
WAZA_アクアリング,
WAZA_でんじふゆう,
WAZA_フレアドライブ,
WAZA_はっけい,
WAZA_はどうだん,
WAZA_ロックカット,
WAZA_どくづき,
WAZA_あくのはどう,
WAZA_つじぎり,
WAZA_アクアテール,
WAZA_タネばくだん,
WAZA_エアスラッシュ,
WAZA_シザークロス,
WAZA_むしのさざめき,
WAZA_りゅうのはどう,
WAZA_ドラゴンダイブ,
WAZA_パワージェム,
WAZA_ドレインパンチ,
WAZA_しんくうは,
WAZA_きあいだま,
WAZA_エナジーボール,
WAZA_ブレイブバード,
WAZA_だいちのちから,
WAZA_すりかえ,
WAZA_ギガインパクト,
WAZA_わるだくみ,
WAZA_バレットパンチ,
WAZA_ゆきなだれ,
WAZA_こおりのつぶて,
WAZA_シャドークロー,
WAZA_かみなりのキバ,
WAZA_こおりのキバ,
WAZA_ほのおのキバ,
WAZA_かげうち,
WAZA_どろばくだん,
WAZA_サイコカッター,
WAZA_しねんのずつき,
WAZA_ミラーショット,
WAZA_ラスターカノン,
WAZA_ロッククライム,
WAZA_きりばらい,
WAZA_トリックルーム,
WAZA_りゅうせいぐん,
WAZA_ほうでん,
WAZA_ふんえん,
WAZA_リーフストーム,
WAZA_パワーウィップ,
WAZA_がんせきほう,
WAZA_クロスポイズン,
WAZA_ダストシュート,
WAZA_アイアンヘッド,
WAZA_マグネットボム,
WAZA_ストーンエッジ,
WAZA_ゆうわく,
WAZA_ステルスロック,
WAZA_くさむすび,
WAZA_おしゃべり,
WAZA_さばきのつぶて,
WAZA_むしくい,
WAZA_チャージビーム,
WAZA_ウッドハンマー,
WAZA_アクアジェット,
WAZA_こうげきしれい,
WAZA_ぼうぎょしれい,
WAZA_かいふくしれい,
WAZA_もろはのずつき,
WAZA_ダブルアタック,
WAZA_ときのほうこう,
WAZA_あくうせつだん,
WAZA_みかづきのまい,
WAZA_にぎりつぶす,
WAZA_マグマストーム,
WAZA_ダークホール,
WAZA_シードフレア,
WAZA_あやしいかぜ,
WAZA_シャドーダイブ,
};
