#include <stdio.h>

#define cons_MSG(...) printf(__VA_ARGS__)

enum error_code
{
	ERROR_NONE
};

typedef int ML_BOOL;
#define ML_FALSE 0
#define ML_TRUE  1

struct Character
{
	int iHp;
	int iPow;
	int iDef;
};

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
	0  : プレイヤー勝利
	1  : 敵の勝利
	8  : 相打ち
	9  : 100ターンで決着つかず
	99 : 例外エラー(↑以外。Release 時に検出されてはいけません。)
*/
extern int fight
( struct Character *a_sPlayer,
  struct Character *a_sEnemy
)
{
	ML_BOOL bJudge = ML_FALSE;
	int iTurn = 1; // not ZERO!!
	int iResult = 99;

	while(!bJudge)
	{
cons_MSG("==================== Turn %i Start ====================\n", iTurn);
		// <========== Start Of Turn ==========>
cons_MSG("========== Player Attack ==========\n");
cons_MSG("Player's HP : %i\n", a_sPlayer->iHp);
cons_MSG("Enemy's  HP : %i\n", a_sEnemy->iHp);
cons_MSG("===================================\n", "");

		// <========== Turn main Start ==========>
		bJudge = Attack(a_sPlayer, a_sEnemy);

cons_MSG("bJudge : %s \n", bJudge?"True":"False");
		if (!bJudge)
		{
cons_MSG("========== Enemy Attack ==========\n");
cons_MSG("Player's HP : %i\n", a_sPlayer->iHp);
cons_MSG("Enemy's  HP : %i\n", a_sEnemy->iHp);
cons_MSG("===================================\n", "");

			bJudge = Attack(a_sEnemy, a_sPlayer);
		}
		else
		{
			// Enemy broken. Nothing to do.
		}
		
		// <========== Turn main   End ==========>

		// <========== End   Of Turn ==========>
cons_MSG("==================== Turn %i   End ====================\n", iTurn);

		if(bJudge)
		{
			break;
		}

		iTurn++;
		
		// Limits Buttle Turn 100 Turn
		if ( iTurn < 101 ) // NOT Started ZERO, but 1!!!
		{
			continue;
		}
		else
		{
			break;
		}
	}
	
	// Judge Result
	{
		ML_BOOL bPlayerAlive = IsAlive(a_sPlayer);
		ML_BOOL bEnemyAlive  = IsAlive(a_sEnemy);

		if (bPlayerAlive)
		{
			if (bEnemyAlive)
			{
				// 引き分け
				iResult = 9;
			}
			else
			{
				// やっつけた
				iResult = 0;
			}
		}
		else
		{
			if (bEnemyAlive)
			{
				// 負けた
				iResult = 1;
			}
			else
			{
				// 相打ち
				iResult = 8;
			}
		}
	}
	
	return iResult;
}

static enum error_code ranrai_main(void)
{
	int iResult = 9;
	
	//make_player();
	struct Character sPlayer;
	sPlayer.iHp  = 20;
	sPlayer.iPow = 5;
	sPlayer.iDef = 5;
	
	//make_enemy();
	struct Character sEnemy;
	sEnemy.iHp  = 15;
	sEnemy.iPow =  3;
	sEnemy.iDef =  3;

	iResult = fight(&sPlayer, &sEnemy);
	if ( 99 == iResult )
	{
cons_MSG("ERROR : unexpected return. fight() Result is 99!!\n");
	}

cons_MSG("========== Fight Finish ===========\n");
cons_MSG("Result      : %i\n", iResult);
cons_MSG("Player's HP : %i\n", sPlayer.iHp);
cons_MSG("Enemy's  HP : %i\n", sEnemy.iHp);
cons_MSG("===================================\n");

	return iResult;
}

int main(const char *a_pArg[])
{
	int iRet = ERROR_NONE;

	iRet = ranrai_main();

	return iRet;
}
