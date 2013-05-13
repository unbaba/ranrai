#include <string.h>
#include "MapManager.h"

/*
キャラクターの現在の位置情報を元にマップを更新します
*/

static void DrawMap
(struct MapCube *a_Map,	// Map
 const int a_iMapSize			// MapSize
)
{
	char pStr[1024] = {0};
	struct MapCube *pMapCube = a_Map;

	int i = 0;
	int y = 0;
	while( NULL != pMapCube)
	{
		if ( NULL != pMapCube->pChar )
		{
			switch( pMapCube->pChar->iType )
			{
				case CHAR_TYPE_PLAYER :
						strcat(pStr, "@");
						break;
				case CHAR_TYPE_ENEMY :
						strcat(pStr, "+");
						break;
				case CHAR_TYPE_WALL :
						strcat(pStr, "*");
						break;
				default :
						strcat(pStr, "9"); // Don't Take this!
						break;
			}
		}
		else
		{
			strcat(pStr, " ");
		}
		pMapCube = pMapCube->pNext;
	}
	MLDISP_RefreshAndPrint(pStr, 1024);

	return;
}

extern signed int getWalk
(
const char a_cKey,
const struct Character a_sPlayer
)
{
	int iRet = 0;

	switch (a_cKey)
	{
		case 0x41 :
			iRet = 0;
			break;
		case 0x42 :
			iRet = 0;
			break;
		case 0x43 :
			iRet = 1;
			break;
		case 0x44 :
			iRet = -1;
			break;
		default :
			iRet = 0;
			break;
	}
	return iRet;
}

extern ML_BOOL IsWalkingToWall
(const struct MapCube *a_Map,
 const struct Character *a_sCharacter,
 const signed int a_iXMove
)
{
	ML_BOOL bRet = ML_FALSE;

	if ( a_sCharacter->iColPos + a_iXMove < 0
		 || ( (MAP_X_MAX-1) == (a_sCharacter->iColPos + a_iXMove) )
	   )
	{
		bRet = ML_TRUE;
	}
	else
	{
		bRet = ML_FALSE;
	}
	
	return bRet;
}

extern int MapManager_Start(void)
{
	ML_BOOL bJudge = ML_FALSE;
	ML_BOOL bHadPlayerWalk = ML_FALSE;
	ML_BOOL bHadEnemyWalk  = ML_FALSE;
	int iResult = 0;
	struct BattleResult BattleResult = {0};
	char cKey = 0x00;

	// Map init!
/*
	{
		int i = 0;
		int y = 0;
		for(y=0; y<MAP_Y_MAX ; y++)
		{
			for(i=0;i<10;i++)
			{
				Map[y][i].pChar = NULL;
			}
		}
	}
*/

	//make_player();
	struct Character sPlayer;
	sPlayer.iType = CHAR_TYPE_PLAYER;
	strcpy(sPlayer.pName, "Shiren");
	sPlayer.iHp  = 20;
	sPlayer.iPow = 5;
	sPlayer.iDef = 5;
	sPlayer.iColPos = 1;

	struct MapCube Map1 = {0};
	Map1.pChar = &sPlayer;
	Map1.x = 0;
	Map1.y = 0;

	//make_enemy();
	struct Character sEnemy;
	sEnemy.iType = CHAR_TYPE_ENEMY;
	strcpy(sEnemy.pName, "Mamuru");
	sEnemy.iHp  = 15;
	sEnemy.iPow =  3;
	sEnemy.iDef =  3;
	sEnemy.iColPos = 9;

	struct MapCube Map2 = {0};
	Map2.pChar = &sEnemy;
	Map2.x = 1;
	Map2.y = 0;

	struct Character sWall;
	sWall.iType = CHAR_TYPE_WALL;
	strcpy(sWall.pName, "Wall");
	sWall.iHp  = 15;
	sWall.iPow =  3;
	sWall.iDef =  3;
	sWall.iColPos = 10;

	struct MapCube Map3 = {0};
	Map3.pChar = &sWall;
	Map3.x = 2;
	Map3.y = 0;

	// Character の 初期配置
	Map1.pNext = &Map2;
	Map2.pNext = &Map3;
	Map3.pNext = NULL;

	// 決着がつくまでループ
/*
	int i = 0;
	while(1)
	{
		// お互い移動している ○→　　←■ ぶつかったら戦闘
		// プレイヤーフェイズ
		int iCnt = 0;
		
		while ( !(0x40 < cKey && cKey < 0x45) )
		{
			ML_GetKey(&cKey);
			signed int iXMove = getWalk(cKey, sPlayer);

			if ( 0 != iXMove ) // 移動せよと入力された
			{
				if ( !IsWalkingToWall(Map, sPlayer, iXMove) // 移動先が壁じゃない
					 && NULL == Map[0][sPlayer.iColPos + iXMove].pChar) // 移動先に何も居ない
				{ // 移動可能
					Map[0][sPlayer.iColPos + iXMove].pChar = Map[0][sPlayer.iColPos].pChar;
					Map[0][sPlayer.iColPos].pChar = NULL;
					sPlayer.iColPos = sPlayer.iColPos + iXMove;
					DrawMap(&Map, MAP_X_MAX);
					break;
				}
				else if ( !(NULL == Map[0][sPlayer.iColPos + iXMove].pChar) ) // 移動先に何か居る
				{
					iResult = fight(&sPlayer, &sEnemy, &BattleResult);
					DrawMap(&Map, MAP_X_MAX);
					MLDISP_DispResult(&BattleResult, ML_FALSE);
					break;
				}
				else
				{
					// nothing to do.
					DrawMap(&Map, MAP_X_MAX);
					printf("nothing to do.\n");
				}
			}
			else
			{
					DrawMap(&Map, MAP_X_MAX);
					printf("nothing to do. iXMove is %i. \n", iXMove);
			}
		}
sleep(1);
		// 敵フェイズ
		if ( NULL == Map[0][sEnemy.iColPos-1].pChar )
		{
			Map[0][sEnemy.iColPos-1].pChar = Map[0][sEnemy.iColPos].pChar;
			Map[0][sEnemy.iColPos].pChar = NULL;
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
		
		DrawMap(Map, MAP_X_MAX);
		MLDISP_DispResult(&BattleResult, ML_FALSE);

		cKey = 0x00;
		bHadPlayerWalk = ML_FALSE;
		bHadEnemyWalk  = ML_FALSE;
	}
		
*/
	DrawMap(&Map1, MAP_X_MAX);
	MLDISP_DispResult(&BattleResult, ML_TRUE);

	return iResult;
}
