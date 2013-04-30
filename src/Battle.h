#include "common_header.h"

/*
	攻撃をします
	
	引数1 攻撃側キャラクタポインタ
	引数2 防御側キャラクタポインタ
	
	戻り値
		ML_TRUE  : 決着ついた
		ML_FALSE : 決着つかず
*/
static ML_BOOL Attack
( struct Character *a_sAttacker,
  struct Character *a_sDefender
);

/*
	引数で受け取ったキャラクターが生存しているかどうかを返します。
 */
static ML_BOOL IsAlive(const struct Character *a_sCharacter);

/*
	プレイヤーと敵の構造体を渡し、戦闘を行います。
	100ターン経っても決着がつかなかった場合は 9 を返します。
	
	戻り値：
	0  : 決着つかず
	101  : プレイヤー勝利
	102  : 敵の勝利
	108  : 相打ち
	99 : 例外エラー(↑以外。Release 時に検出されてはいけません。)
*/
extern int fight
( struct Character *a_sAttacker,
  struct Character *a_sDefender
);
