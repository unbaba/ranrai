#include <stdio.h>
#include <string.h>

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
	
	int iColPos;	// x 方向位置情報
};

struct MapCube
{
	struct Character *pChar;
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


static int MapManager_Start(void)
{
	ML_BOOL bJudge = ML_FALSE;
	ML_BOOL bHadPlayerWalk = ML_FALSE;
	ML_BOOL bHadEnemyWalk  = ML_FALSE;
	int iResult = 0;
	struct MapCube Map[10] = {0};

	// init!
	{
		int i = 0;
		for(i=0;i<10;i++)
		{
			Map[i].pChar = NULL;
		}
	}

	//make_player();
	struct Character sPlayer;
	sPlayer.iHp  = 20;
	sPlayer.iPow = 5;
	sPlayer.iDef = 5;
	sPlayer.iColPos = 0;
	
	//make_enemy();
	struct Character sEnemy;
	sEnemy.iHp  = 15;
	sEnemy.iPow =  3;
	sEnemy.iDef =  3;
	sEnemy.iColPos = 9;

	// Character の 初期配置
	Map[sPlayer.iColPos].pChar = &sPlayer;
	Map[sEnemy.iColPos].pChar = &sEnemy;

	// 決着がつくまでループ
	int i = 0;
	while(!bJudge)
	{
		// お互い移動している ○→　　←■ ぶつかったら戦闘
		// プレイヤーフェイズ
		if (NULL == Map[sPlayer.iColPos+1].pChar)
		{
			Map[sPlayer.iColPos+1].pChar = Map[sPlayer.iColPos].pChar;
			Map[sPlayer.iColPos].pChar = NULL;
			sPlayer.iColPos++;
cons_MSG("========== Player Walk ===========\n");
cons_MSG("Player is now %i, Enemy is now %i\n", sPlayer.iColPos, sEnemy.iColPos);
cons_MSG("===================================\n");
		}
		else
		{
cons_MSG("========== Player Attacked ==========\n");
			iResult = fight(&sPlayer, &sEnemy);
		}
cons_MSG("========== Player Finish ===========\n");
cons_MSG("Result      : %i\n", iResult);
cons_MSG("Player is now %i, Enemy is now %i\n", sPlayer.iColPos, sEnemy.iColPos);
cons_MSG("Player's HP : %i\n", sPlayer.iHp);
cons_MSG("Enemy's  HP : %i\n", sEnemy.iHp);
cons_MSG("===================================\n");
		
		// 敵フェイズ
		if ( NULL == Map[sEnemy.iColPos-1].pChar )
		{
			Map[sEnemy.iColPos-1].pChar = Map[sEnemy.iColPos].pChar;
			Map[sEnemy.iColPos].pChar = NULL;
			sEnemy.iColPos--;
cons_MSG("========== Enemy Walk ===========\n");
cons_MSG("Player is now %i, Enemy is now %i\n", sPlayer.iColPos, sEnemy.iColPos);
cons_MSG("===================================\n");
		}
		else
		{
cons_MSG("========== Enemy Attacked ==========\n");
			if ( !(100 < iResult) && // iResult suggests fight is not finished.
				 0 < sEnemy.iHp      // Enemy Alive. 
			   ) 
			{
				iResult = fight(&sEnemy, &sPlayer);
			}
			else
			{
cons_MSG("no. Enemy is already broken!!\n");
			}
		}

cons_MSG("========== Enemy  Finish ===========\n");
cons_MSG("Result      : %i\n", iResult);
cons_MSG("Player is now %i, Enemy is now %i\n", sPlayer.iColPos, sEnemy.iColPos);
cons_MSG("Player's HP : %i\n", sPlayer.iHp);
cons_MSG("Enemy's  HP : %i\n", sEnemy.iHp);
cons_MSG("===================================\n");

		if ( 100 < iResult )
		{
			break;
		}
		else if ( 99 == iResult )
		{
cons_MSG("ERROR : unexpected return. fight() Result is 99!!\n");
		}
		else
		{
			// continue
			i++;
		}
		
		bHadPlayerWalk = ML_FALSE;
		bHadEnemyWalk  = ML_FALSE;
	}
		
	return iResult;
}

static enum error_code ranrai_main(void)
{
	int iResult = 9;
	

	iResult = MapManager_Start();

/*
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
*/
	return iResult;
}

/*
	引数で渡された文字列が何行になるかを返します
	渡す文字列は暫定的に 10000文字までとします
	
	必須:
		文字列の終わりに '\0' があること
		文字列の終わりに '\0' が無い場合の動作は未定義です(大抵 Segmentation Fault)

	戻り値:
		行数(n行)
*/
#define MLSTR_GET_HOW_MANY_LINES_MAX_LEN 10000

extern int MLSTR_GetHowManyLines
(const char *a_pStr,	// 文字列
 const int   a_iStrLen	// 文字列長
)
{
	int i = 0;
	int iLineCnt = 0;
	const char *pStr = a_pStr;

	for(i=0; i<MLSTR_GET_HOW_MANY_LINES_MAX_LEN; i++)
	{
		if( '\0' != *(pStr + i) )
		{
			if ( '\n' != *(pStr + i) )
			{
				// check next char.
			}
			else
			{
				iLineCnt++;
			}
		}
		else
		{
			break; // Str End.
		}
	}

	return iLineCnt;
}

/*
	引数で受け取った文字列を画面更新しながら表示します
	
	戻り値:
		ML_TRUE  正常終了
		ML_FALSE 異常終了 // 今のところエラー無し
*/
extern int MLDISP_RefreshAndPrint
(const char *a_pStr,	// 文字列
 const int   a_iStrLen	// 文字列長
)
{
	ML_BOOL bRet = ML_FALSE;

	if( NULL != a_pStr )
	{
		// 画面のクリア
		printf("\x1b[2J"); // 画面のクリア
		printf("\x1b[0;0H"); // (0, 0) へカーソルを移動

		// 目的の文字列を表示
		printf(a_pStr);

		bRet = ML_TRUE;
	}
	else
	{
cons_MSG("", "a_pStr is NULL!!");
	}

	return bRet;
}

int main(const char *a_pArg[])
{
	int iRet = ERROR_NONE;

	iRet = ranrai_main();

	return iRet;
}
