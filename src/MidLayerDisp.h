/*
	引数で受け取った文字列を画面更新しながら表示します
	
	戻り値:
		ML_TRUE  正常終了
		ML_FALSE 異常終了 // 今のところエラー無し
*/
extern int MLDISP_RefreshAndPrint
(const char *a_pStr,	// 文字列
 const int   a_iStrLen	// 文字列長
);
