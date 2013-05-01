#include <string.h>
#include "MapManager.h"

/*
キャラクターの現在の位置情報を元にマップを更新します
*/

static void DrawMap
(const struct MapCube *a_Map,	// Map
 const int a_iMapSize			// MapSize
)
{
	char pStr[1024] = {0};
	struct MapCube pMapCube;

	int i = 0;
	for(i=0; i<a_iMapSize; i++)
	{
		pMapCube = a_Map[i];
		if ( NULL != pMapCube.pChar )
		{
			switch( pMapCube.pChar->iType )
			{
				case 0 :
						strcat(pStr, "+");
						break;
				case 1 :
						strcat(pStr, "*");
						break;
				default :
						strcat(pStr, "9"); // Don't Take this!
						break;
			}
		}
		else
		{
			strcat(pStr, "-");
		}
	}
	
	strcat(pStr, "\n");
	MLDISP_RefreshAndPrint(pStr, 1024);

	return;
}

extern int MapManager_Start(void)
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
	sPlayer.iType = 0;
	sPlayer.iHp  = 20;
	sPlayer.iPow = 5;
	sPlayer.iDef = 5;
	sPlayer.iColPos = 0;
	
	//make_enemy();
	struct Character sEnemy;
	sEnemy.iType = 1;
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
		
		DrawMap(Map, 10);
		
		bHadPlayerWalk = ML_FALSE;
		bHadEnemyWalk  = ML_FALSE;
	}
		
	return iResult;
}
