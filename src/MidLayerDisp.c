#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "MidLayerDisp.h"

#include "common_header.h"

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
		printf("%s", a_pStr);

		bRet = ML_TRUE;
	}
	else
	{
cons_MSG("", "a_pStr is NULL!!");
	}

	return bRet;
}

#define QUIT_CHAR 0x04 /* CTRL-D を押すと終了する．*/

// 端末設定保存用大域変数
struct termios CookedTermIos; // cooked モード用
struct termios RawTermIos; // raw モード用

ML_BOOL ML_GetKey(char *a_pRet)
{
	ML_BOOL bRet = ML_FALSE;
	int c;

	// 初期状態の端末設定 (cooked モード) を取得・保存する．
	tcgetattr(STDIN_FILENO, &CookedTermIos);

	// raw モードの端末設定を作成・保存する．
	RawTermIos = CookedTermIos;
	cfmakeraw(&RawTermIos);

	// 端末を raw モードに設定する．
	tcsetattr(STDIN_FILENO, 0, &RawTermIos);

	// １文字入力されるごとに標準出力にエコーする．
	while((c = getchar()) != QUIT_CHAR)
	{
		if ( isprint(c) )
		{
			// 印字可能文字の場合：そのまま返す．
			*a_pRet = c;
			bRet = ML_TRUE;
			break;
		}
		else
		{
			// 制御文字の場合：16進で出力．
			if ( 0x1B != c )
			{
				*a_pRet = c;
				bRet = ML_TRUE;
				break;
			}
			else // 制御文字がエスケープシーケンスだった場合は
				 // 1文字目が 「ESC」(=0x18)
				 // 2文字目が 「[」(=0x5B)なので読み飛ばし
			{
				getchar();
				*a_pRet = getchar();
				//printf("<%02X>", c);
				bRet = ML_TRUE;
				break;
			}
		}
	}

	// 端末設定を元に戻す．
	tcsetattr(STDIN_FILENO, 0, &CookedTermIos);
	//putchar('\n');

	return bRet;
}

/*
	結果を表示します
*/
extern ML_BOOL MLDISP_DispResult
(struct BattleResult *a_sResult
)
{
	ML_BOOL bRet = ML_FALSE;
printf("iResult is '%i' \n", a_sResult->iResult);

	if ( (NULL != a_sResult->sSrc)
		 && (NULL != a_sResult->sDst)
	   )
	{
		bRet = ML_TRUE;
	}

	if ( bRet
		 && (BATTLE_RESULT_DESTROYED == a_sResult->iResult) 
	   )
	{
		printf("==================================================\n");
		printf("%s Attacked %s, %i Damage. \n", a_sResult->sSrc->pName, a_sResult->sDst->pName, a_sResult->iDamage);
		printf("%s Destroyed %s !! \n", a_sResult->sSrc->pName, a_sResult->sDst->pName);
		printf("==================================================\n");
	}
	else if ( bRet
			  && ( BATTLE_RESULT_NO_JUDGE == a_sResult->iResult )
			)
	{
		printf("==================================================\n");
		printf("%s Attacked %s, %i Damage. \n", a_sResult->sSrc->pName, a_sResult->sDst->pName, a_sResult->iDamage);
		printf("==================================================\n");
	}
	else
	{
	}

	return bRet;
}
