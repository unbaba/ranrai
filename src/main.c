#include "MapManager.h"

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

int main(const char *a_pArg[])
{
	int iRet = ERROR_NONE;

	iRet = ranrai_main();

	return iRet;
}
