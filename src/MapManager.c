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
	struct BattleResult BattleResult = {0};
	char cKey = 0x00;

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
	strcpy(sPlayer.pName, "Shiren");
	sPlayer.iHp  = 20;
	sPlayer.iPow = 5;
	sPlayer.iDef = 5;
	sPlayer.iColPos = 0;
	
	//make_enemy();
	struct Character sEnemy;
	sEnemy.iType = 1;
	strcpy(sEnemy.pName, "Mamuru");
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
		int iCnt = 0;
		
		ML_GetKey(&cKey);
		while ( !(0x40 < cKey && cKey < 0x45) )
		{
			ML_GetKey(&cKey);
		}
		
		if (NULL == Map[sPlayer.iColPos+1].pChar)
		{
			Map[sPlayer.iColPos+1].pChar = Map[sPlayer.iColPos].pChar;
			Map[sPlayer.iColPos].pChar = NULL;
			sPlayer.iColPos++;
		}
		else
		{
			iResult = fight(&sPlayer, &sEnemy, &BattleResult);
		}
		DrawMap(Map, 10);
		MLDISP_DispResult(&BattleResult);
		ML_GetKey(&cKey);

		// 敵フェイズ
		if ( NULL == Map[sEnemy.iColPos-1].pChar )
		{
			Map[sEnemy.iColPos-1].pChar = Map[sEnemy.iColPos].pChar;
			Map[sEnemy.iColPos].pChar = NULL;
			sEnemy.iColPos--;
		}
		else
		{
			if ( !((BATTLE_RESULT_DESTROYED-1) < iResult) && // iResult suggests fight is not finished.
				 0 < sEnemy.iHp      // Enemy Alive. 
			   )
			{
				iResult = fight(&sEnemy, &sPlayer, &BattleResult);
			}
			else
			{
			}
		}

		if ( (BATTLE_RESULT_DESTROYED-1) < iResult )
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
		MLDISP_DispResult(&BattleResult);

		bHadPlayerWalk = ML_FALSE;
		bHadEnemyWalk  = ML_FALSE;
	}
		
	DrawMap(Map, 10);
	MLDISP_DispResult(&BattleResult);

	return iResult;
}
