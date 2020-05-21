/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * ToCoNet_mod_Common.h
 *
 *  Created on: 2013/01/09
 *      Author: seigo13
 */

/**
 * @defgroup grp_ToCoNet_Nwk ToCoNet モジュール - ネットワーク層
 * ネットワーク層を抽象化するための定義群。
 */

#ifndef TOCONET_MOD_NWK_COMMON_H_
#define TOCONET_MOD_NWK_COMMON_H_

#include <jendefs.h>
#include "ToCoNet.h"

#ifdef __cplusplus
extern "C" {
#endif // C++


/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク種別、MAC層のみ (未使用)
 */
#define TOCONET_NWK_TYPE_MAC 0

/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク種別、階層付ツリー
 */
#define TOCONET_NWK_TYPE_LAYER_TREE 2

/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク状態、停止状態
 */
#define TOCONET_NWK_STATUS_INACTIVE 0x00

/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク状態、開始済み（ただし接続しているかどうかは、別のマスクで判定）
 */
#define TOCONET_NWK_STATUS_START_MASK 0x80

/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク状態、接続済み
 */
#define TOCONET_NWK_STATUS_READY_MASK 0x01

/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク役割、エンドデバイス（中継無し子機）
 */
#define TOCONET_NWK_ROLE_ENDDEVICE 0x01

/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク役割、ルーター（中継子機）
 */
#define TOCONET_NWK_ROLE_ROUTER 0x04

/** @ingroup grp_ToCoNet_Nwk
 * ネットワーク役割、親機
 */
#define TOCONET_NWK_ROLE_PARENT 0x80

/** @ingroup grp_ToCoNet_Nwk
 * ネットワークアドレス、親機
 */
#define TOCONET_NWK_ADDR_PARENT 0x80000000
/** @ingroup grp_ToCoNet_Nwk
 * ネットワークアドレス、直接電波が見える近隣への送信（ユーザアプリケーションでは利用不可）
 */
#define TOCONET_NWK_ADDR_NEIGHBOUR 0x80000001
/** @ingroup grp_ToCoNet_Nwk
 * ネットワークアドレス、直接接続している中継・親機
 */
#define TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE 0x80000002
/** @ingroup grp_ToCoNet_Nwk
 * ネットワークアドレス、ブロードキャスト（親機からの指定のみ）
 */
#define TOCONET_NWK_ADDR_BROADCAST 0x8000FFFF

/** @ingroup grp_ToCoNet_Nwk
 * MAC 層でのブロードキャストアドレス
 */
#define TOCONET_MAC_ADDR_BROADCAST 0x0000FFFF

/** @ingroup grp_ToCoNet_Nwk
 * 無効なアドレス
 */
#define TOCONET_NWK_ADDR_NULL 0xFFFFFFFF

/** @ingroup grp_ToCoNet_Nwk
 * ロングアドレスか否かを判定する
 */
#define TOCONET_NWK_ADDR_IS_LONG(a) ((a) & 0x80000000)

/** @ingroup grp_ToCoNet_Nwk
 * ショートアドレスか否かを判定する
 */
#define TOCONET_NWK_ADDR_IS_SHORT(a) (((a) & 0xFFFF0000) == 0)

/** @ingroup grp_ToCoNet_Nwk
 * ネットワークコンテキスト（各種ステータスなどを格納する構造体）
 */
typedef struct _tsToCoNet_Nwk_Context tsToCoNet_Nwk_Context;

/** @ingroup grp_ToCoNet_Nwk
 * ネットワークコンテキストの基礎情報
 */
typedef struct
{
	uint8 u8NwkTypeId; //!< ネットワーク種別 (TOCONET_NWK_TYPE_??? 参照)
	uint8 u8Role; //!< ネットワーク役割 (TOCONET_NWK_ROLE_??? 参照)
	uint8 u8State; //!< ネットワーク状態 (TOCONET_NWK_STATUS_??? 参照)
	uint8 u8Layer; //!< ネットワーク階層（親機までのホップ数, 中継機は 1 以上の値となる。親機・中継しない子機では本値は使用されない）
	uint8 u8LayerSub; //!< ネットワークサブ階層（同階層のトップまでのホップ数。同一階層内での接続が起きた場合に 1 以上の値になる）
	uint8 u8SessionId; //!< 接続ID（これが更新された場合、新たな親機を探索し直す必要がある）
} tsToCoNet_Nwk_Context_Base_Info;

/** @ingroup grp_ToCoNet_Nwk
 * ネットワークコンテキストのメソッド群 (ToCoNet_Nwk_b???() のマクロ定義を参照）
 */
typedef struct
{
	bool_t (*bInit)(tsToCoNet_Nwk_Context *);
	bool_t (*bStart)(tsToCoNet_Nwk_Context *);
	bool_t (*bStop)(tsToCoNet_Nwk_Context *);
	bool_t (*bPause)(tsToCoNet_Nwk_Context *);
	bool_t (*bResume)(tsToCoNet_Nwk_Context *);
	/**
	 * ネットワーク層より送信を行う
	 * @param pc ネットワークコンテキスト
	 * @param ptx 送信情報
	 * @param pcb 送信完了を示すコールバック関数
	 * @return TRUE:成功 FALSE:失敗
	 */
	bool_t (*bTx)(tsToCoNet_Nwk_Context *pc, tsTxDataApp *ptx, void *pcb);
} tsToCoNet_Nwk_Context_Base_Method;

/** @ingroup grp_ToCoNet_Nwk
 * ネットワークコンテキスト構造体の定義。tsToCoNet_Nwk_Context参照
 */
struct _tsToCoNet_Nwk_Context
{
	tsToCoNet_Nwk_Context_Base_Info sInfo;
	tsToCoNet_Nwk_Context_Base_Method sMethod;
};

/*
 * API DEFINITION
 */
/** @ingroup grp_ToCoNet_Nwk
 * ネットワークを初期化する。
 * - 本呼び出しに先立ち、ネットワーク用の設定関数(ToCoNet_NwkLyTr_psConfig)によりネットワークコンテキストを取得しておく。
 * @param pc tsToCoNet_Nwk_Context*:ネットワークコンテキスト
 * @return TRUE:成功 FALSE:失敗
 */
#define ToCoNet_Nwk_bInit(pc) (pc)->sMethod.bInit(pc)
/** @ingroup grp_ToCoNet_Nwk
 * ネットワークを開始する。
 * @param pc tsToCoNet_Nwk_Context*:ネットワークコンテキスト
 * @return TRUE:成功 FALSE:失敗
 */
#define ToCoNet_Nwk_bStart(pc) (pc)->sMethod.bStart(pc)
/** @ingroup grp_ToCoNet_Nwk
 * 中継しない子機のみ。ネットワークを中断する。スリープ直前などに呼び出す。
 * @param pc tsToCoNet_Nwk_Context*:ネットワークコンテキスト
 * @return TRUE:成功 FALSE:失敗
 */
#define ToCoNet_Nwk_bPause(pc) (pc)->sMethod.bPause(pc)
/** @ingroup grp_ToCoNet_Nwk
 * 中継しない子機のみ。ネットワークを回復する。スリープ復帰時に呼び出す。
 * @param pc tsToCoNet_Nwk_Context*:ネットワークコンテキスト
 * @return TRUE:成功 FALSE:失敗
 */
#define ToCoNet_Nwk_bResume(pc) (pc)->sMethod.bResume(pc)
/** @ingroup grp_ToCoNet_Nwk
 * ネットワークを中止する。
 * @param pc tsToCoNet_Nwk_Context*:ネットワークコンテキスト
 * @return TRUE:成功 FALSE:失敗
 */
#define ToCoNet_Nwk_bStop(pc) (pc)->sMethod.bStop(pc)
/** @ingroup grp_ToCoNet_Nwk
 * 送信を行う。
 * @param pc tsToCoNet_Nwk_Context*:ネットワークコンテキスト
 * @param tx tsTxDataApp*:送信情報
 * @return TRUE:成功 FALSE:失敗
 */
#define ToCoNet_Nwk_bTx(pc, tx) (pc)->sMethod.bTx(pc, tx, NULL)

/*
 * Utils
 */
/** @ingroup grp_ToCoNet_Nwk
 * 内部処理用。パケット受信時にモジュールへデータ転送する。
 * @param pRx 受信情報
 */
void ToCoNet_vRxProc_Mod(tsRxData *pRx);
/** @ingroup grp_ToCoNet_Nwk
 * 内部処理用。パケット受信時にモジュールへデータ転送する
 * @param pRx 受信情報
 */
void ToCoNet_vRxProc_App(tsRxData *pRx);

#ifdef __cplusplus
}
#endif // C++

#endif
