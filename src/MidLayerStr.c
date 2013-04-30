#include "MidLayerStr.h"

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