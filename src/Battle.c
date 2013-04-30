#include "Battle.h"

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
)
{
	ML_BOOL bRet = ML_FALSE;

	a_sDefender->iHp = a_sDefender->iHp - a_sAttacker->iPow;
	
	// judge
	if ( 0 < a_sDefender->iHp )
	{
		// Enemy is NOT Broken.
	}
	else
	{
		bRet = ML_TRUE; // Enemy is Broken.
	}
	
	return bRet;
}

/*
	引数で受け取ったキャラクターが生存しているかどうかを返します。
 */
static ML_BOOL IsAlive(const struct Character *a_sCharacter)
{
	ML_BOOL bRet = ML_FALSE;
	
	if ( 0 < a_sCharacter->iHp)
	{
		bRet = ML_TRUE;
	}
	else
	{
		bRet = ML_FALSE;
	}
	
	return bRet;
}
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
)
{
	ML_BOOL bJudge = ML_FALSE;
	int iResult = 99;

	// <========== Start Of Turn ==========>

	// <========== Turn main Start ==========>
	bJudge = Attack(a_sAttacker, a_sDefender);

cons_MSG("bJudge : %s \n", bJudge?"True":"False");
cons_MSG("Attacker's HP : %i\n", a_sAttacker->iHp);
cons_MSG("Defender's  HP : %i\n", a_sDefender->iHp);
cons_MSG("===================================\n", "");

	// <========== Turn main   End ==========>

	// <========== End   Of Turn ==========>
	
	// Judge Result
	{
		ML_BOOL bPlayerAlive = IsAlive(a_sAttacker);
		ML_BOOL bEnemyAlive  = IsAlive(a_sDefender);

		if (bPlayerAlive)
		{
			if (bEnemyAlive)
			{
				// 引き分け
				iResult = 0;
			}
			else
			{
				// やっつけた
				iResult = 101;
			}
		}
		else
		{
			if (bEnemyAlive)
			{
				// 負けた
				iResult = 102;
			}
			else
			{
				// 相打ち
				iResult = 108;
			}
		}
	}
	
	return iResult;
}
