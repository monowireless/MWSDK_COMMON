/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

#ifndef BTNMGR_H_
#define BTNMGR_H_

/**
 * @defgroup grp_Utils_BTM Utils - IOポーリング入力
 *
 * IOをポーリング入力の処理を行う
 *
 */

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
//#define BTM_MAX_HISTORY 5

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
	uint8 u8DeviceTimer; //!< 使用するタイマーデバイス (0xff は外部タイマー)
	uint8 u8MaxHistory; //!< 履歴数（連照数）
	uint16 u16Tick_ms; //!< チェック間隔
	uint32 bmPortMask; //!< ビットマップ
} tsBTM_Config;

typedef void (*PR_BTM_HANDLER)(uint16);

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/** @ingroup grp_Utils_BTM
 * IO入力(ポーリング)を初期化する
 * @param sConf 初期化設定
 */
PR_BTM_HANDLER prBTM_InitExternal(tsBTM_Config *sConf);

/** @ingroup grp_Utils_BTM
 * IO入力(ポーリング)を初期化する。
 * - 簡易設定のため 200Hz で５連照にて確定。
 * @param bmPortMask 入力ポートのビットマップ
 * @param u8TimerDevice 使用するタイマーデバイス
 */
void vBTM_Init(uint32 bmPortMask, uint8 u8TimerDevice);

/** @ingroup grp_Utils_BTM
 * IO入力(ポーリング)を開始する
 */
void vBTM_Enable();
/** @ingroup grp_Utils_BTM
 * IO入力(ポーリング)を中断する
 */
void vBTM_Disable();

/** @ingroup grp_Utils_BTM
 * IO入力(ポーリング)の状態を確認する。
 * @param pbmPorts ポートの状態（ビットマップ）
 * @param pbmChanged 変更状態（ビットマップ）
 * @return TRUEなら値が取得可能、FALSE なら準備中（最初の状態が確定できない）
 */
bool_t bBTM_GetState(uint32 *pbmPorts, uint32 *pbmChanged);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
#ifdef __cplusplus
}
#endif // C++

#endif /* BTMGR_H_ */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
