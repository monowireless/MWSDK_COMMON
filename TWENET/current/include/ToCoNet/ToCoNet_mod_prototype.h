/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * ToCoNet_mod_prototye.h
 *
 *  Created on: 2013/01/10
 *      Author: seigo13
 */

/*!
    @brief モジュールのAPI宣言および登録マクロ
    @author seigo13
*/

#ifndef TOCONET_MOD_PROTOTYE_H_
#define TOCONET_MOD_PROTOTYE_H_

#ifdef __cplusplus
extern "C" {
#endif // C++

// 内部モジュールID
#define ToCoNet_MOD_MAC 0 //!< ネットワークモジュール
#define ToCoNet_MOD_TX 1 //!< ネットワークモジュール
#define ToCoNet_MOD_NWK 2 //!< ネットワークモジュール
#define ToCoNet_MOD_NBSCAN 3 //!< 近隣探索（探索側）
#define ToCoNet_MOD_NBSCAN_SLAVE 4 //!< 近隣探索（被探索側）
#define ToCoNet_MOD_CH_MANAGER 5 //!< チャネル管理
#define ToCoNet_MOD_NWK_MSG_POOL 6 //!< メッセージプール

// Module: EnergyScan
#if defined(ToCoNet_USE_MOD_ENERGYSCAN) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_ENERGYSCAN() ToCoNet_vReg_mod_EnergyScan()
void ToCoNet_vReg_mod_EnergyScan(); //!< モジュールの登録関数

/** @ingroup grp_ToCoNet_EnergySacn
 * エナジースキャンを開始する
 * - Nwk層での利用は不可
 * - 関数呼び出し後は速やかに無線送受信が不可能になるため、無線送受信が無い事を確認の上使用する
 * - 完了後 E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE イベントを発生
 * @param u32ChMask 測定するチャネルマスク(ch16のみなら 1UL<<16 を指定、ch16,17なら 1UL<<16|1UL<<17 を指定)
 * @param u8PreScale 測定時間。およそ(2^u8PreScale)*16[ms]
 * @return TRUEなら要求がMAC層に伝達された
 */
bool_t ToCoNet_EnergyScan_bStart(uint32 u32ChMask, uint8 u8PreScale);
#else
#define ToCoNet_REG_MOD_ENERGYSCAN()
#define ToCoNet_vReg_mod_EnergyScan() (error)
#define ToCoNet_EnergyScan_bStart(...) (error)
#endif

#if defined(ToCoNet_USE_MOD_NBSCAN) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_NBSCAN() ToCoNet_vReg_mod_NbScan()
void ToCoNet_vReg_mod_NbScan(); //!< モジュールの登録関数

/** @ingroup grp_ToCoNet_NbScan
 *
 * 近隣探索を開始する。本探索では、各チャネルにスキャン要求パケットを送信し、
 * NeighbourScan Slave モジュールをロードされたノードからパケット返答を受信
 * することで近隣の探索を行う。
 *
 * 最大探索数は１６で、LQI の大きなものからリストされる。
 *
 * @param u32ChMask  探索対象チャネル
 * @param u16Dur_ms  各チャネルの探索時間 (ms)
 */
uint8 ToCoNet_NbScan_bStart(uint32 u32ChMask, uint16 u16Dur_ms);

/** @ingroup grp_ToCoNet_NbScan
 *
 * 近隣探索を開始する。本探索では、各チャネルにスキャン要求パケットを送信し、
 * NeighbourScan Slave モジュールをロードされたノードからパケット返答を受信
 * することで近隣の探索を行う。
 * （モジュール内から使用するためのコールバック関数指定版）
 *
 * @param u32ChMask  探索対象チャネル
 * @param u16Dur_ms  各チャネルの探索時間 (ms)
 * @param u8RoleMask ネットワーク役割 (Role) に合致するノードのみ探索する
 * @param fp_CallBack 探索完了時に呼びだすコールバック関数
 */
uint8 ToCoNet_NbScan_bStartToFindRole(uint32 u32ChMask, uint16 u16Dur_ms, uint8 u8RoleMask, void *fp_CallBack);

/** @ingroup grp_ToCoNet_NbScan
 *
 * 既にアドレスが判っているノードが稼働しているチャネルを探索する。
 * 各チャネルにパケット送信を試み、パケット送信に成功したチャネルを返す。
 *
 * @param u32ChMask  探索対象チャネル
 * @param u32Addr    探索するアドレス
 */
uint8 ToCoNet_NbScan_bStartToFindAddr(uint32 u32ChMask, uint32 u32Addr);

/** @ingroup grp_ToCoNet_NbScan
 *
 * 既にアドレスが判っているノードが稼働しているチャネルを探索する。
 * 各チャネルにパケット送信を試み、パケット送信に成功したチャネルを返す。
 * （モジュール内から使用するためのコールバック関数指定版）
 *
 * @param u32ChMask  探索対象チャネル
 * @param u32Addr    探索するアドレス
 * @param fp_CallBack 探索完了時に呼びだすコールバック関数
 */
uint8 ToCoNet_NbScan_bStartToFindAddrCb(uint32 u32ChMask, uint32 u32Addr, void *fp_CallBack);
#else
#define ToCoNet_REG_MOD_NBSCAN()
#define ToCoNet_vReg_mod_NbScan() (error)
#define ToCoNet_NbScan_bStart(...) (error)
#define ToCoNet_NbScan_bStartToFindRole(...) (error)
#define ToCoNet_NbScan_bStartToFindAddr(...) (error)
#define ToCoNet_NbScan_bStartToFindAddrCb(...) (error)
#endif

// Module: NBSCAN_SLAVE (Neighbour scan) 近隣探索で探索応答する
#if defined(ToCoNet_USE_MOD_NBSCAN_SLAVE) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_NBSCAN_SLAVE() ToCoNet_vReg_mod_NbScan_Slave()
void ToCoNet_vReg_mod_NbScan_Slave(); //!< モジュールの登録関数
#else
#define ToCoNet_REG_MOD_NBSCAN_SLAVE()
#define ToCoNet_vReg_mod_NbScan_Slave() (error)
#endif

// Module: MTRAND (Mersenne twister random algorithm) 高品質な乱数生成アルゴリズム
//   本アルゴリズムは BSD License に基づきます。ToCoNet_USE_MOD_MTRAND を定義して生成
//   したバイナリには本アルゴリズムが含まれるためライセンス条件に従う必要があります。(randMt.c参照)
#if defined(ToCoNet_USE_MOD_RAND_MT) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_USE_MOD_RAND_MT() ToCoNet_vReg_mod_Rand_MT()
void ToCoNet_vReg_mod_Rand_MT(); //!< モジュールの登録関数
#else
#define ToCoNet_REG_MOD_MTRAND()
#endif

// Module: XorShift 法に基づく乱数生成
#if defined(ToCoNet_USE_MOD_RAND_XOR_SHIFT) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_RAND_XOR_SHIFT() ToCoNet_vReg_mod_Rand_Xor_Shift()
void ToCoNet_vReg_mod_Rand_Xor_Shift(); //!< モジュールの登録関数
#else
#define ToCoNet_REG_MOD_RAND_XOR_SHIFT()
#endif

// Module: Nwk LayerTree
#include "ToCoNet_mod_Nwk_LayerTree.h"
#if defined(ToCoNet_USE_MOD_NWK_LAYERTREE) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_NWK_LAYERTREE() ToCoNet_vReg_mod_Nwk_LayerTree()
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * LayerTree のモジュール登録
 */
void ToCoNet_vReg_mod_Nwk_LayerTree();
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * LayerTree の設定関数。
 * @param _psConf 設定情報を格納した構造体
 * @return ネットワークコンテキスト構造体（コンテキストは内部的に確保される）
 */
tsToCoNet_Nwk_Context* ToCoNet_NwkLyTr_psConfig(tsToCoNet_NwkLyTr_Config *_psConf);

/*
 * 依存モジュールの定義チェック
 */
#ifndef ToCoNet_LIB_BUILD
#ifndef ToCoNet_USE_MOD_DUPCHK
# error "needs ToCoNet_USE_MOD_DUPCHK defined for ToCoNet_USE_MOD_NWK_LAYERTREE."
#endif
#ifndef ToCoNet_USE_MOD_NBSCAN
# error "needs ToCoNet_USE_MOD_NBSCAN defined for ToCoNet_USE_MOD_NWK_LAYERTREE."
#endif
#ifndef ToCoNet_USE_MOD_NBSCAN_SLAVE
# error "needs ToCoNet_USE_MOD_NBSCAN_SLAVE defined for ToCoNet_USE_MOD_NWK_LAYERTREE."
#endif
#ifndef ToCoNet_USE_MOD_NBSCAN_SLAVE
# error "needs ToCoNet_USE_MOD_NBSCAN_SLAVE defined for ToCoNet_USE_MOD_NWK_LAYERTREE."
#endif
#endif
#else
#define ToCoNet_REG_MOD_NWK_LAYERTREE()
#define ToCoNet_NwkLyTr_psConfig(...) (error)
#endif

#if defined(ToCoNet_USE_MOD_NWK_LAYERTREE_MININODES) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_NWK_LAYERTREE_MININODES() ToCoNet_vReg_mod_Nwk_LayerTree_MiniNodes()
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree_MiniNodes
 * LayerTree のモジュール登録
 */
void ToCoNet_vReg_mod_Nwk_LayerTree_MiniNodes();
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree_MiniNodes
 * LayerTree の設定関数。
 * @param _psConf 設定情報を格納した構造体
 * @return ネットワークコンテキスト構造体（コンテキストは内部的に確保される）
 */
tsToCoNet_Nwk_Context* ToCoNet_NwkLyTr_psConfig_MiniNodes(tsToCoNet_NwkLyTr_Config *_psConf);
#else
#define ToCoNet_REG_MOD_NWK_LAYERTREE_MININODES()
#define ToCoNet_NwkLyTr_psConfig_MiniNodes(...) (error)
#endif

// Module: Channel Manager
#include "ToCoNet_mod_Channel_Mgr.h"
#if defined(ToCoNet_USE_MOD_CHANNEL_MGR) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_CHANNEL_MGR() ToCoNet_vReg_mod_Channel_Mgr()
void ToCoNet_vReg_mod_Channel_Mgr(); //!< モジュールの登録関数
/** @ingroup grp_ToCoNet_mod_Channel_Mgr
 * チャネルマネージャの設定を行う。
 * - NWK層の開始前に行っておく。
 * @param _psConf 設定情報を格納した構造体。
 * @return チャネルマネージャのコンテキスト構造体(tsToCoNet_ChMgr_Context*型のポインタ)
 */
tsToCoNet_ChMgr_Context* ToCoNet_ChMgr_psConfig(tsToCoNet_ChMgr_Config *_psConf);
#else
#define ToCoNet_REG_MOD_CHANNEL_MGR()
#define ToCoNet_ChMgr_psConfig(...) (error)
#endif

// Module: Channel Manager
#include "ToCoNet_mod_Nwk_MessagePool.h"
#if defined(ToCoNet_USE_MOD_NWK_MESSAGE_POOL) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_NWK_MESSAGE_POOL() ToCoNet_vReg_mod_Nwk_MessagePool()
void ToCoNet_vReg_mod_Nwk_MessagePool(); //!< モジュールの登録関数

/** @ingroup grp_ToCoNet_mod_MessagePool
 * メッセージプールにメッセージを設定する（親機のみ）
 * - 設定後、速やかに各中継機へメッセージを配送する
 * - リフレッシュはしないので、失敗等を考え定期的に実行することを推奨する
 * - ただし、下り方向の全ブロードキャストでありネットワーク負荷は大きいため、注意が必要である
 * @param u8Slot スロット番号
 * @param u16Lifs_s 寿命[秒] (未使用)
 * @param u8len メッセージ長
 * @param puDat データ
 * @return TRUEなら成功
 */
bool_t ToCoNet_MsgPl_bSetMessage(uint8 u8Slot, uint16 u16Lifs_s, uint8 u8len, uint8 *puDat);

/** @ingroup grp_ToCoNet_mod_MessagePool
 * メッセージプールの情報を要求する（EndDevice中継しない子機のみ）
 * - 要求が成功すれば E_EVENT_TOCONET_NWK_MESSAGE_POOL イベントが発生。
 * - 無線送信の失敗などで実行されなければ何も起きない。タイムアウトなどで対処すること。
 *
 * @param u8Slot 要求するスロット番号
 * @return 要求が受け付けられれば TRUE
 */
bool_t ToCoNet_MsgPl_bRequest(uint8 u8Slot);

/** @ingroup grp_ToCoNet_mod_MessagePool
 * メッセージプールの情報を要求し、データを親機に送る（EndDevice中継しない子機のみ）
 *
 * - 要求が成功すれば E_EVENT_TOCONET_NWK_MESSAGE_POOL イベントが発生。
 * - 無線送信の失敗などで実行されなければ何も起きない。タイムアウトなどで対処すること。
 * - データ送信完了のイベントは発生しない。
 *
 * @param u8Slot 要求するスロット番号
 * @param psTx 送信したいデータ (auData, u8Seq, u8Cmd, u8CbId, u8Len が有効)
 * @return 要求が受け付けられれば TRUE
 */
bool_t ToCoNet_MsgPl_bRequest_w_Payload_to_Parenet(uint8 u8Slot, tsTxDataApp *psTx);
#else
#define ToCoNet_REG_MOD_NWK_MESSAGE_POOL()
#define ToCoNet_vReg_mod_Nwk_MessagePool(...) (error)
#define ToCoNet_MsgPl_bSetMessage(...) (error)
#define ToCoNet_MsgPl_bRequest(...) (error)
#define ToCoNet_MsgPl_bRequest_w_Payload_to_Parenet(...) (error)
#endif


// Module: TX RX QUEUE の定義。メモリサイズに応じて。
#if defined(ToCoNet_USE_MOD_TXRXQUEUE_SMALL)
#define ToCoNet_REG_MOD_TXRXQUEUE() ToCoNet_vReg_mod_TxRxQueueSmall()
void ToCoNet_vReg_mod_TxRxQueueSmall(); //!< モジュールの登録関数
#elif defined(ToCoNet_USE_MOD_TXRXQUEUE_MID)
#define ToCoNet_REG_MOD_TXRXQUEUE() ToCoNet_vReg_mod_TxRxQueueMid()
void ToCoNet_vReg_mod_TxRxQueueMid(); //!< モジュールの登録関数
#elif defined(ToCoNet_USE_MOD_TXRXQUEUE_BIG) // default, big queue
#define ToCoNet_REG_MOD_TXRXQUEUE() ToCoNet_vReg_mod_TxRxQueueBig()
void ToCoNet_vReg_mod_TxRxQueueBig(); //!< モジュールの登録関数
#else
#define ToCoNet_REG_MOD_TXRXQUEUE() ToCoNet_vReg_mod_TxRxQueueMid()
void ToCoNet_vReg_mod_TxRxQueueMid(); //!< モジュールの登録関数
#endif

// Module: TX
#if !defined(ToCoNet_USE_MOD_TX_CUSTOM) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_TX() ToCoNet_vReg_mod_Tx_Standard(); //!< モジュールの登録関数
void ToCoNet_vReg_mod_Tx_Standard();
#else
// place custom handler
#endif

// 重複チェッカ
#if defined(ToCoNet_USE_MOD_DUPCHK) || defined(ToCoNet_LIB_BUILD)
#define ToCoNet_REG_MOD_DUPCHK() ToCoNet_vReg_mod_DupChk()
void ToCoNet_vReg_mod_DupChk(); //!< モジュールの登録関数
#else
#define ToCoNet_REG_MOD_DUPCHK()
#endif

/**
 * モジュール登録マクロ
 */
#define ToCoNet_REG_MOD_ALL() \
			ToCoNet_REG_MOD_ENERGYSCAN(); \
			ToCoNet_REG_MOD_NBSCAN(); \
			ToCoNet_REG_MOD_NBSCAN_SLAVE(); \
			ToCoNet_REG_MOD_MTRAND(); \
			ToCoNet_REG_MOD_RAND_XOR_SHIFT() ; \
			ToCoNet_REG_MOD_TXRXQUEUE(); \
			ToCoNet_REG_MOD_NWK_LAYERTREE(); \
			ToCoNet_REG_MOD_CHANNEL_MGR(); \
			ToCoNet_REG_MOD_NWK_MESSAGE_POOL(); \
			ToCoNet_REG_MOD_TX() \
			ToCoNet_REG_MOD_DUPCHK() \
			;

#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_MOD_PROTOTYE_H_ */
