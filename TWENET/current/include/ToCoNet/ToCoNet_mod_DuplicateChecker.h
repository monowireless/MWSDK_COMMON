/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/**
 * @defgroup grp_ToCoNet_DuplicateChecker ToCoNet 重複チェッカ
 *
 * シーケンス番号で運用されるパケットの重複を確認する
 */

#ifndef TOCONET_MOD_DUPLICATECHECKER_H_
#define TOCONET_MOD_DUPLICATECHECKER_H_

#include <jendefs.h>

#ifdef __cplusplus
extern "C" {
#endif // C++

extern volatile uint32 u32TickCount_ms;

/** @ingroup grp_ToCoNet_DuplicateChecker
 * - 重複チェックでチェックする対象のカウント数
 * - 2^n で 64 が安全
 */
#define TOCONET_DUPCHK_MAX_COUNTS 64

/** @ingroup grp_ToCoNet_DuplicateChecker
 * 重複チェックのビットマップをクリアするまでの時間。
 * タイムアウトまでに対象ノードからパケットが受信されない場合、ビットマップをクリアする。
 * （タイムアウト前と同じ番号のパケットが来ても受理する）
 */
extern uint16 TOCONET_DUPCHK_TIMEOUT_ms;

/** @ingroup grp_ToCoNet_DuplicateChecker
 * 管理する時間単位(最大のタイムアウト時間を制限)
 * 5 なら、2^5=32[ms] で１カウント、最大127まで使用するため、
 * タイムアウトは 32*127 で 4000ms 程度である。
 */
extern uint8 TOCONET_DUPCHK_TICK_SCALE;

/** @ingroup grp_ToCoNet_DuplicateChecker
 * 重複チェックのノード探索
 */
typedef struct {
	uint8 au8BmDup[TOCONET_DUPCHK_MAX_COUNTS/8]; //!< Seq番号に対応する64bit分のビットマップ
	uint8 au8TickDupPkt[TOCONET_DUPCHK_MAX_COUNTS/8]; //!< ４つに分割したブロック内で最初に受信したシステム時刻 1カウント=32ms
} tsToCoNet_DupChk_Element;

struct _sToCoNet_DupChk_Context;

/** @ingroup grp_ToCoNet_DuplicateChecker
 * 探索結果情報
 */
typedef struct _sToCoNet_DupChk_Context {
	uint32 *au32Nodes;
	tsToCoNet_DupChk_Element *asDupChk;
	uint8 *au8TimeStamp; // 一番最近受信したタイムスタンプ（現時刻との差）
	uint8 u8MaxNodes;
	uint8 u8HashMask;

	// methods
	bool_t (*pf_bAdd)(struct _sToCoNet_DupChk_Context *, uint32, uint8);
	void (*pf_vClean)(struct _sToCoNet_DupChk_Context *);
} tsToCoNet_DupChk_Context;

/** @ingroup grp_ToCoNet_DuplicateChecker
 * データの宣言 (nameで識別できるように変数定義を行う)
 * @param name 識別名
 * @param count ノード数
 */
#define TOCONET_DUPCHK_DECLARE_CONETXT(name, count) \
	static tsToCoNet_DupChk_Context _TOCONET_DUPCHK_sContext_##name; \
	static uint32 _TOCONET_DUPCHK_au32Nodes_##name[count]; \
	static tsToCoNet_DupChk_Element _TOCONET_DUPCHK_asEle_##name[count]; \
	static uint8 _TOCONET_DUPCHK_au8TimeStamp_##name[count]; \
	static const uint8 _TOCONET_DUPCHK_u8Count_##name = count

//static uint8 _TOCONET_DUPCHK_au8Hash_##name[count];


/** @ingroup grp_ToCoNet_DuplicateChecker
 * 事前にTOCONET_DUPCHK_DECLARE_CONETXT()によりデータを宣言しておくこと
 *
 * @param
 */
#define ToCoNet_DupChk_psInit(name) \
		_ToCoNet_DupChk_psInit( \
			&_TOCONET_DUPCHK_sContext_##name, \
			_TOCONET_DUPCHK_au32Nodes_##name, \
			_TOCONET_DUPCHK_asEle_##name, \
			_TOCONET_DUPCHK_au8TimeStamp_##name, \
			_TOCONET_DUPCHK_u8Count_##name)


/** @ingroup grp_ToCoNet_DuplicateChecker
 * uint32 から 8bit の XOR によるハッシュを作成する。
 *
 * - ハッシュ生成ベンチマーク (16Mhz, JN5164)\n
 *   配列探索時間に対して十分短ければ有効である。
 *
 *   - CRC8(関数呼び出し)      - 4.5uSec\n
 *     for (i = 0; i < 1000*1000; i+=1000) u8res = u8CCITT(&i, 4);\n
 *   - XOR                     - 0.8uSec\n
 *     for (i = 0; i < 1000*1000; i+=1000) u8res = u8HashGen(i);\n
 *   - XOR(char型ポインタ変換) - 1.2uSec\n
 *     for (i = 0; i < 1000*1000; i+=1000) u8res = u8HashGen(&i);\n
 *   - 配列探索                - 0.6uSec/要素\n
 *     for (i = 0; i < 256; i++) if (ary[i] == u32Key) break;\n
 *
 * @param pDupPkt 管理構造体
 */
static inline uint8 u8HashGen(uint32 u32d) {
	uint32 t = u32d;
	t ^= (u32d >> 16);
	t ^= (t >> 8);

	return t & 0xff;
}

void ToCoNet_DupChk_vInit_E(tsToCoNet_DupChk_Element *pDupPkt);
tsToCoNet_DupChk_Context* _ToCoNet_DupChk_psInit(
	tsToCoNet_DupChk_Context *pContext,
	uint32 *pu32Addrs,
	tsToCoNet_DupChk_Element *psEle,
	uint8 *pu8TimeStamp,
	uint8 u8count);
bool_t ToCoNet_DupChk_bAdd_E(tsToCoNet_DupChk_Element *pDupPkt, uint8 u8Seq);
bool_t ToCoNet_DupChk_bAdd(tsToCoNet_DupChk_Context *pC, uint32 u32addr, uint8 u8Seq);
uint8 ToCoNet_DupChk_u8Clean_E(tsToCoNet_DupChk_Element *pC);
void ToCoNet_DupChk_vClean(tsToCoNet_DupChk_Context *pC);

#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_MOD_DUPLICATECHECKER_H_ */
