#include <stdio.h>
#include "MidLayerDisp.h"
#include "common_param.h"

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