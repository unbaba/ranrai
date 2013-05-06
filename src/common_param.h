#ifndef __COMMON_PARAM
#else
#error "re-include MidLayerStr.h"
#endif // __COMMON_PARAM

//#define ENABLE_CONS_MSG
#ifdef ENABLE_CONS_MSG
#define cons_MSG(...) printf(__VA_ARGS__)
#else
#define cons_MSG(...)
#endif //ENABLE_CONS_MSG

// ==================================================
// ===== エラーコード定義
enum error_code
{
	ERROR_NONE
};
// ==================================================
// ==================================================

// ==================================================
// ===== boolean 定義
typedef int ML_BOOL;
#define ML_FALSE 0
#define ML_TRUE  1
// ==================================================
// ==================================================


// ==================================================
// ===== 戦闘結果
#define BATTLE_RESULT_NO_JUDGE	  0
#define BATTLE_RESULT_DESTROYED	101
#define BATTLE_RESULT_LOST		102
#define BATTLE_RESULT_DRAW		108

struct BattleResult
{
	struct Character *sSrc;
	struct Character *sDst;
	int iResult;
	int iDamage;
	ML_BOOL bDisplayed;
};

// ==================================================
// ===== キャラクターパラメータ定義
struct Character
{
	int iType;		// 属性 0:主人公 1:敵 等

	char pName[256];

	int iHp;		// HP
	int iPow;		// 力
	int iDef;		// 守備力
	
	int iColPos;	// x 方向位置情報
};
// ==================================================
// ==================================================



// ==================================================
// ===== マップ情報の定義
struct MapCube
{
	struct Character *pChar;
};
// ==================================================
// ==================================================

#define __COMMON_PARAM y
