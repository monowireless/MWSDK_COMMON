/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/**
 * @defgroup grp_ToCoNet_Event ToCoNet イベント・状態遷移処理
 *
 * ToCoNet のイベントおよび状態遷移処理を行うための基本定義、基本関数群
 *
 */

#ifndef TOCONET_EVENT_H_
#define TOCONET_EVENT_H_
#include "jendefs.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

/** @ingroup grp_ToCoNet_Event
 * アプリケーション定義のイベントの開始番号 */
#define ToCoNet_EVENT_APP_BASE 0x00008000

/** @ingroup grp_ToCoNet_Event
 * アプリケーション定義の状態の開始番号 */
#define ToCoNet_STATE_APP_BASE 0x00008000

/** @ingroup grp_ToCoNet_Event
 * ToCoNet 定義のイベント。
 * - アプリケーションへ伝達されるイベントは [AE] (イベントマシン) [AN] (ネットワークイベント) と記載する。
 */
typedef enum
{
	E_EVENT_NULL = 0,                       			//!< 内部的に使用しているがスタックコード、アプリケーションコードともに使用しないこと。
	E_EVENT_START_UP = 0x00000001,          			//!< [AE] システム起動（またはスリープ復帰）時に発生する。イベント引数は EVARG_START_UP_WAKEUP_MASK, EVARG_START_UP_WAKEUP_RAMHOLD_MASK。
    E_EVENT_RX,                             			//!< ToCoNetのRx(受信)時に発生する。
    E_EVENT_TX,                             			//!< ToCoNetのTx終了時に発生する。
    E_EVENT_TX_STATUS,                      			//!< Mac層からのTx終了時に発生する。
    E_EVENT_TICK_TIMER, 								//!< [AE] TickTimer 発生時に呼ばれる。
    E_EVENT_TICK_SECOND,                    			//!< [AE] １秒毎に発生する。
    E_ORDER_INITIALIZE,                     			//!< 初期化を促す。
    E_ORDER_KICK,                           			//!< 開始を促す。
    E_ORDER_RECONF,                         			//!< 再設定を促す。
    E_EVENT_TOCONET_START,                  			//!< 未使用
    E_EVENT_TOCONET_ON_SLEEP,               			//!< [AE] スリープ直前に呼ばれる。
    E_EVENT_TOCONET_MAC_START,              		//!< 未使用
    E_EVENT_TOCONET_PANIC,							//!< MAC 層などの問題でシステムリセットを行う場合
    E_EVENT_TOCONET_NWK_START,              		//!< [AN] ネットワーク層の開始時に呼ばれる。
    E_EVENT_TOCONET_NWK_DISCONNECT,         		//!< [AN] ネットワーク層がエラー等でネットワークから離れたときに呼ばれる。ネットワーク層は再接続を繰り返す。
    E_EVENT_TOCONET_NWK_SCAN_COMPLETE,      		//!< NbScan の Scan 完了時に呼び出される。イベント引数はスキャン結果 tsToCoNet_NbScan_Result*。
    E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE,   		//!< [AN] EnergyScan 完了時に呼び出される。イベント引数はスキャン結果 uint8 *p、p[0] は要素数、p[1] ... は 0-255 のエネルギーレベル。
    E_EVENT_TOCONET_NWK_ROUTE_PKT,          		//!< [AN] パケット中継時に呼び出される。イベント引数は中継パケットの情報 tsRoutePktInfo*。
    E_EVENT_TOCONET_NWK_CHANGE_CHANNEL,     		//!< [AN] ChannelMgr でチャネルが変更されたときに呼び出される。イベント引数は新しいチャネル (11~25)。
    E_EVENT_TOCONET_NWK_MESSAGE_POOL,       		//!< [AN] MessagePool のデータが配送された。イベント引数は tsToCoNet_MsgPl_Entity*。
    E_EVENT_TOCONET_NWK_MESSAGE_POOL_REQUEST,	//!< [AN] MessagePool のデータ要求に対して応答した。イベント引数は tsToCoNet_MsgPl_Request*。
    E_EVENT_TOCONET_NWK_PARENT_DISCONNECT,		//!< 親機が接続解除を求めてきた。
    E_EVENT_TOCONET_NWK_NB_BEACON, 	 			//!< 近隣ノードからのビーコン (Rx -> NbScan -> NWK)
    E_EVENT_TOCONET_NWK_NB_BEACON_REQUEST, 		//!< 近隣ノードへのビーコン要求 (NWK -> NbScan_Slave)
	E_EVENT_NEW_STATE = 0x000000FF              		//!< 状態遷移したときに最初に呼ばれるイベント。
}teEvent;

#define EVARG_START_UP_WAKEUP_RAMHOLD_MASK 0x40 //!< RAM を保持した状態で復帰した @ingroup grp_ToCoNet_Event
#define EVARG_START_UP_WAKEUP_MASK 0x80 //!< POR(PowerOnReset)ではなく、ウェイクアップした @ingroup grp_ToCoNet_Event

#define EVARG_TX_STATUS_SUCCESS_MASK  0x01 //!< 送信成功した @ingroup grp_ToCoNet_Event
#define EVARG_TX_STATUS_FAIL_MASK     0x80 //!< 送信失敗した @ingroup grp_ToCoNet_Event
#define EVARG_TX_STATUS_FAIL_CCA_MASK 0x10 //!< CCAにより送信失敗した @ingroup grp_ToCoNet_Event

/** @ingroup grp_ToCoNet_Event
 * 状態遷移マシンでシステム内部的に使用されている状態
 * - E_STATE_IDLE はすべてのイベント処理関数で共通に使用する開始時の状態である。
 * - それ以外のイベントは、各イベント処理関数内の仕様に従い使用される。
 */
typedef enum
{
	E_STATE_IDLE = 0,           //!< E_STATE_IDLE
	E_STATE_RUNNING,            //!< E_STATE_RUNNING
	E_STATE_FINISHED,           //!< E_STATE_FINISHED
	E_STATE_WAIT_COMMAND,       //!< E_STATE_WAIT_COMMAND
	E_STATE_PAIR,               //!< E_STATE_PAIR
	E_STATE_WAIT_PAIR,          //!< E_STATE_WAIT_PAIR
	E_STATE_WAITING_TX_COMPLETE,//!< E_STATE_WAITING_TX_COMPLETE
	E_STATE_PREPARE_TX,         //!< E_STATE_PREPARE_TX
	E_STATE_WAIT_TX,            //!< E_STATE_WAIT_TX
	E_STATE_FINISH_TX_SUCCESS,  //!< E_STATE_FINISH_TX_SUCCESS
	E_STATE_FINISH_TX_FAIL,     //!< E_STATE_FINISH_TX_FAIL
	E_STATE_SCANNING_INIT,      //!< E_STATE_SCANNING_INIT
	E_STATE_SCANNING,           //!< E_STATE_SCANNING
	E_STATE_SCANNING_FINISH,    //!< E_STATE_SCANNING_FINISH
	E_STATE_LOCATING,           //!< E_STATE_LOCATING
	E_STATE_PAUSE,              //!< E_STATE_PAUSE
	E_STATE_DISCONNECT,         //!< E_STATE_DISCONNECT
	E_STATE_RECONF,             //!< E_STATE_RECONF
	E_STATE_VOID = 0x7FFF       //!< E_STATE_VOID
} teState;

/** @ingroup grp_ToCoNet_Event
 * イベント処理関数のイベント状態を定義
 */
typedef struct _tsEvent
{
	teState eState; //!< 状態
	uint32 u32tick_new_state; //!< 現在の状態に遷移してからの経過時間 (ms)
	void (*pvPorcessEvent)(struct _tsEvent *, teEvent, uint32); //!< 状態処理関数へのポインタ
	bool_t bKeepStateOnSetAll; //!< TRUE にセットしておくと、スリープ復帰時もスリープ前の状態が維持される
	bool_t bExecNewEvent; //!< E_EVENT_NEW が実行済みの場合 TRUE
	void *pvExtraData; //! イベント状態に追加情報を付加したい
} tsEvent;

/** @ingroup grp_ToCoNet_Event
 * イベントリスナー登録イベント（現時点で必要性が無いため、使用されていない）
 * - 登録イベントは uint32 のビットマップの一部を使用し、このビットマップが 1 になったイベント処理関数にイベントを渡す。
 */
typedef struct {
	uint32 E_EVENT_NULL;
	uint32 E_EVENT_START_UP;
    uint32 E_EVENT_RX;
    uint32 E_EVENT_TX;
    uint32 E_EVENT_TX_STATUS;
    uint32 E_EVENT_TICK_TIMER;
    uint32 E_EVENT_TICK_SECOND;
} tsToCoNetEventVars;
extern tsToCoNetEventVars sEvVars; //!< イベントリスナー登録データ @ingroup grp_ToCoNet_Event
#define ToCoNet_Event_Listen(ev, mask) ((ev) |= (mask)) //!< イベントを受け付ける判定 @ingroup grp_ToCoNet_Event
#define ToCoNet_Event_Ignore(ev, mask) ((ev) &= (~(mask))) //!< イベントを受け付けない判定 @ingroup grp_ToCoNet_Event

/** @ingroup grp_ToCoNet_Event
 * アプリケーション用のAPIラッパ。イベント処理関数をシステムに登録する
 * @param pv イベント処理関数の関数ポインタ
 * @return イベント処理関数のハンドル(ToCoNet_Event_ProcessH()など、内部データのアクセスが線形検索ではなく配列参照による低い呼び出しコストでイベント処理させる)
 */
#define ToCoNet_Event_Register_State_Machine(pv) PRSEV_u8Register(pv)

/** @ingroup grp_ToCoNet_Event
 * アプリケーション用のAPIラッパ。指定したイベント処理関数にイベントを実行させる。
 * @param ev teEvent:イベント
 * @param ag uint32:イベント引数
 * @param pv イベント処理関数の関数ポインタ
 */
#define ToCoNet_Event_Process(ev,ag,pv)	_PRSEV_vProcessEvSingle(ev,ag,pv)

/** @ingroup grp_ToCoNet_Event
 * アプリケーション用のAPIラッパ。指定したイベント処理関数（ハンドルで指定）にイベントを実行させる。
 * @param ev teEvent:イベント
 * @param ag uint32:イベント引数
 * @param hn uint8:イベント処理関数のハンドル
 */
#define ToCoNet_Event_ProcessH(ev,ag,hn) _PRSEV_vProcessEvSingleH(ev,ag,hn)

/** @ingroup grp_ToCoNet_Event
 * 状態を遷移させる。状態が遷移されるとイベント処理関数の処理後に、もう一度、E_EVENT_NEW_STATEイベ
 * ントにてイベント処理関数が呼び出される。
 * @param pev tsEvent*:イベント管理構造体
 * @param e teState:状態
 */
#define ToCoNet_Event_SetState(pev,s) PRSEV_vSetState(pev,s)

/** @ingroup grp_ToCoNet_Event
 * 状態が遷移した時刻からの経過時間を返す。
 * @param pev tsEvent*:イベント管理構造体
 * @return uint32:経過時刻[ms]
 */
#define ToCoNet_Event_u32TickFrNewState(pev) (u32TickCount_ms - (pev)->u32tick_new_state)

/** @ingroup grp_ToCoNet_Event
 * 状態が遷移した時刻からの経過時間を返す。
 * @param pv イベント処理関数の関数ポインタ
 * @return teState:現在の状態
 */
#define ToCoNet_Event_eGetState(pv) PRSEV_eGetState(pv)

/** @ingroup grp_ToCoNet_Event
 * 状態が遷移した時刻からの経過時間を返す。
 * @param hn uint8:イベント処理関数のハンドル
 * @return teState:現在の状態
 */
#define ToCoNet_Event_eGetStateH(hn) PRSEV_eGetStateH(hn)

/** @ingroup grp_ToCoNet_Event
 * イベント処理関数の状態をスリープ復帰後も維持するフラグをセットする
 * @param pev tsEvent*:イベント管理構造体
 */
#define ToCoNet_Event_vKeepStateOnRamHoldSleep(pev) (pev->bKeepStateOnSetAll = TRUE)

/**
 * イベント処理関数の最大登録数
 */
#define PRSEV_NUM_EVENT_HANDLER 8

/* 以下、スタックAPI、解説は *.c コード中に含める */
void PRSEV_vInit();
uint8 PRSEV_u8Register(void (*pvHandler));
uint8 PRSEV_u8RegisterEx(void (*pvHandler), void* pvExtra);
bool_t PRSEV_bUnregister(void (*pvHandler));
void PRSEV_vSetStateAll(teState eNewState, bool_t bConsiderKeepFlag);
void PRSEV_vSetState(tsEvent*, teState eNewState);
teState PRSEV_eGetState(void *pvFunc);
teState PRSEV_eGetStateH(uint8 u8hnd);
tsEvent* PRSEV_peGetContext(void *pvFunc);
tsEvent* PRSEV_peGetContextH(uint8 u8Hnd);

#define PRSEV_vProcessEv(ev,arg) _PRSEV_vProcessEv(sEvVars.ev, arg)
#define PRSEV_vProcessEvSingle(ev,arg,pv) _PRSEV_vProcessEvSingle(sEvVars.ev, arg, pv)
void _PRSEV_vProcessEv(uint32 eEvent, uint32 u32evarg);
void _PRSEV_vProcessEvSingle(uint32 eEvent, uint32 u32evarg, void* pvFunc);
void _PRSEV_vProcessEvSingleH(uint32 eEvent, uint32 u32evarg, uint8 u8Hnd);

bool_t PRSEV_bPostEvent(uint32 eEvent, uint32 u32evarg, void* pvFunc);
#define PRSEV_u32TickFrNewState(pev) (u32TickCount_ms - (pev)->u32tick_new_state)
#define PRSEV_vResetTickFrNewState(pev) ((pev)->u32tick_new_state = u32TickCount_ms)

/*
 * State/Event handling procedures
 *   (long switch clause is very hard to see...)
 */
typedef struct {
	uint32 u32State;
	void (*pvFunc)(tsEvent *, teEvent, uint32);
} tsToCoNet_Event_StateHandler;
void ToCoNet_Event_StateExec(const tsToCoNet_Event_StateHandler *, tsEvent *, teEvent, uint32);
// macros for ToCoNet_Event_StateExec
#define PRSEV_HANDLER_NAME(n) vHnd_##n
#define PRSEV_HANDLER(n,...) PRSEV_HANDLER_NAME(n)(__VA_ARGS__)
#define PRSEV_HANDLER_DEF(n,...) static void PRSEV_HANDLER_NAME(n)(__VA_ARGS__)
#define PRSEV_HANDLER_TBL_DEF(n) { n, PRSEV_HANDLER_NAME(n) }
#define PRSEV_HANDLER_TBL_TRM { E_STATE_VOID, NULL }

/** @ingroup grp_ToCoNet_Event
 * E_EVENT_TOCONET_PANIC のパラメータ
 */
typedef struct {
	bool_t bCancelReset; //!< TRUE を格納するとリセットをキャンセルする
	uint8 u8ReasonCode; //!< 要因コード
	uint32 u32ReasonInfo; //!< 要因の補助情報
	string strReason; //!< 要因の文字列情報
} tsPanicEventInfo;


// PANIC 要因
#define TOCONET_PANIC_TX_FAIL_COUNT 0x11
#define TOCONET_PANIC_LAYERTREE_SCAN_FAIL_COUNT 0x21
#define TOCONET_PANIC_LAYERTREE_SCAN_TIMEOUT 0x22
#define TOCONET_PANIC_LAYERTREE_LOCATE_TIMEOUT 0x23

#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_EVENT_H_ */
