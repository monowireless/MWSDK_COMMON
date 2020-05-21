/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/**
 * @defgroup grp_ToCoNet_TxRxQueue ToCoNet送信・受信キュー
 *
 * ToCoNet の送受信時に使用されるキューを定義する
 * - キューのサイズは以下のモジュール定義を行う
 *   - ToCoNet_USE_MOD_TXRXQUEUE_SMALL : 最小限のサイズ (約1.5kb)\n
 *   	キュー数→送信:3,同時送信数:2,受信ユーザ:3,受信制御パケット:2
 *   - ToCoNet_USE_MOD_TXRXQUEUE_BIG : 大きなサイズ(約5kb)\n
 *   	キュー数→送信:20,同時送信数:5,受信ユーザ:10,受信制御パケット:5
 *   - 定義しない場合は約2.5kbのキューを設定する\n
 *   	キュー数→送信:6,同時送信数:3,受信ユーザ:6,受信制御パケット:3
 *
 */

#ifndef MAC_TXRXQUEUE_H_
#define MAC_TXRXQUEUE_H_

#include <jendefs.h>
#include <mac_sap.h>
#include <mac_pib.h>

#include "ByteQueue.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

/****************************************************************************
 * MACROs
 ***************************************************************************/
// Rx/Tx buffering queue
#define DECLARE_RXQUEUE(name, size) \
	static tsRxData sRxdatabuff##_##name[size]; \
	static uint8 au8Rxdatabuff_idx##_##name[size]; \
	static const uint8 u8RxBuffSiz##_##name = size

#define DECLARE_RXQUEUE_PTR(name) \
	tsRxData *sRxdatabuff##_##name; \
	uint8 *au8Rxdatabuff_idx##_##name; \
	uint8 u8RxBuffSiz##_##name

#define DECLARE_RXQUEUE_EXTERN_PTR(name) \
	extern tsRxData *sRxdatabuff##_##name; \
	extern uint8 *au8Rxdatabuff_idx##_##name; \
	extern uint8 u8RxBuffSiz##_##name

#define DECLARE_RXQUEUE_PTR_ARY(ptr, ary) \
	sRxdatabuff##_##ptr = sRxdatabuff##_##ary; \
	au8Rxdatabuff_idx##_##ptr = au8Rxdatabuff_idx##_##ary; \
	u8RxBuffSiz##_##ptr = u8RxBuffSiz##_##ary

#define RXQUEUE_vInit(q, id) { \
		/* memset(sRxdatabuff##_##q, 0, sizeof(tsRxData) * u8RxBuffSiz##_##q); */ \
		for (q.u8RxEntry = 0; q.u8RxEntry < u8RxBuffSiz##_##q; q.u8RxEntry++) { sRxdatabuff##_##q[q.u8RxEntry].bStored = FALSE; } \
		q.sRxdatabuff = sRxdatabuff##_##q; \
		q.au8Rxdatabuff_idx = au8Rxdatabuff_idx##_##q; \
		q.u8RxBuffSiz = u8RxBuffSiz##_##q; \
		q.u8RxEntry = 0; \
		QUEUE__vInit(&(q).sQRxdatabuff, q.au8Rxdatabuff_idx, q.u8RxBuffSiz); \
		Mac_bRxQueueReg(&q, id); \
	}

#define RXQUEUE_bEmpty(q) QUEUE_bEmpty((q).sQRxdatabuff)
#define RXQUEUE_u8RemoveItem(q) QUEUE_u8RemoveItem((q).sQRxdatabuff, TRUE)
#define RXQUEUE_u8TailItem(q) QUEUE_u8TailItem((q).sQRxdatabuff)
#define RXQUEUE_bFull(q) QUEUE_bFull((q).sQRxdatabuff)
#define RXQUEUE_u16Count(q) QUEUE_u16Count((q).sQRxdatabuff)
#define RXQUEUE_vFlush(q) QUEUE__vFlush(&(q).sQRxdatabuff)
#define RXQUEUE_vRemoveItemAndGetPointer(q, ptr) { \
	uint8 u8idx = QUEUE_u8RemoveItem((q).sQRxdatabuff, TRUE); \
	(q).sRxdatabuff[u8idx].bStored = FALSE; \
	ptr = &((q).sRxdatabuff[u8idx]); \
	}
#define RXQUEUE_vAddItem(q) \
	QUEUE_vAddItem((q).sQRxdatabuff, (q).u8RxEntry, FALSE); \
	(q).u8RxEntry++; \
	if ((q).u8RxEntry >= (q).u8RxBuffSiz) (q).u8RxEntry = 0; \
	(q).sRxdatabuff[(q).u8RxEntry].bStored = FALSE;
#define RXQUEUE_vAllocItemAndGetPointer(q, ptr) ptr = &((q).sRxdatabuff[(q).u8RxEntry])

#define DECLARE_TXQUEUE(name, size) \
	static tsTxData sTxdatabuff##_##name[size]; \
	static uint8 au8Txdatabuff_idx##_##name[size]; \
	static uint8 u8TxBuffSiz##_##name = size

#define DECLARE_TXQUEUE_PTR(name) \
	tsTxData *sTxdatabuff##_##name; \
	uint8 *au8Txdatabuff_idx##_##name; \
	uint8 u8TxBuffSiz##_##name

#define DECLARE_TXQUEUE_EXTERN_PTR(name) \
	extern tsTxData *sTxdatabuff##_##name; \
	extern uint8 *au8Txdatabuff_idx##_##name; \
	extern uint8 u8TxBuffSiz##_##name

#define DECLARE_TXQUEUE_PTR_ARY(ptr, ary) \
	sTxdatabuff##_##ptr = sTxdatabuff##_##ary; \
	au8Txdatabuff_idx##_##ptr = au8Txdatabuff_idx##_##ary; \
	u8TxBuffSiz##_##ptr = u8TxBuffSiz##_##ary

#define TXQUEUE_vInit(q) { \
		/* memset(sTxdatabuff##_##q, 0, sizeof(tsTxData) * u8TxBuffSiz##_##q); */ \
		for (q.u8TxEntry = 0; q.u8TxEntry < u8TxBuffSiz##_##q; q.u8TxEntry++) { sTxdatabuff##_##q[q.u8TxEntry].bStored = FALSE; } \
		(q).sTxdatabuff = sTxdatabuff##_##q; \
		(q).au8Txdatabuff_idx = au8Txdatabuff_idx##_##q; \
		(q).u8TxBuffSiz = u8TxBuffSiz##_##q; \
		(q).u8TxEntry = 0; \
		QUEUE__vInit(&(q).sQTxdatabuff, (q).au8Txdatabuff_idx, (q).u8TxBuffSiz); \
	}

#define TXQUEUE_bEmpty(q) QUEUE_bEmpty((q).sQTxdatabuff)
#define TXQUEUE_u8RemoveItem(q) QUEUE_u8RemoveItem((q).sQTxdatabuff, TRUE)
#define TXQUEUE_u8TailItem(q) QUEUE_u8TailItem((q).sQTxdatabuff)
#define TXQUEUE_bFull(q) QUEUE_bFull((q).sQTxdatabuff)
#define TXQUEUE_u16Count(q) QUEUE_u16Count((q).sQTxdatabuff)
#define TXQUEUE_vFlush(q) QUEUE__vFlush(&(q).sQTxdatabuff)
#define TXQUEUE_vRemoveItemAndGetPointer(q, ptr) { \
	uint8 u8idx = QUEUE_u8RemoveItem((q).sQTxdatabuff, TRUE); \
	(q).sTxdatabuff[u8idx].bStored = FALSE; \
	ptr = &((q).sTxdatabuff[u8idx]); \
	}
#define TXQUEUE_vAddItem(q) \
	QUEUE_vAddItem((q).sQTxdatabuff, (q).u8TxEntry, FALSE); \
	(q).u8TxEntry++; \
	if ((q).u8TxEntry >= (q).u8TxBuffSiz) (q).u8TxEntry = 0; \
	(q).sTxdatabuff[(q).u8TxEntry].bStored = FALSE;
#define TXQUEUE_vAllocItemAndGetPointer(q, ptr) ptr = &((q).sTxdatabuff[(q).u8TxEntry])

#define MAX_RX_APP_PAYLOAD 108 //!< 受信パケットの最大サイズ @ingroup grp_ToCoNet_TxRxQueue
#define MAX_TX_APP_PAYLOAD 108 //!< 送信パケットの最大サイズ @ingroup grp_ToCoNet_TxRxQueue

/****************************************************************************
 * DataType
 ***************************************************************************/

/** @ingroup grp_ToCoNet_TxRxQueue
 * ライブラリ内部で使用する受信パケット格納構造体
 *
 */
typedef struct {
	uint16 u16SrcPan;		//!< 送信元のPAN ID
	uint16 u16DstPan;		//!< 送信先のPAN ID
	uint32 u32SrcAddr;		//!< 送信元のアドレス
	uint32 u32DstAddr;		//!< 送信先のアドレス
	uint32 u32ExtraInfo;	//!< 拡張情報
	uint8 u8LQI;			//!< 受信時のLQI
	uint8 u8Rxlen;			//!< パケットのペイロード長
	uint8 auData[MAX_RX_APP_PAYLOAD]; //!< ペイロード
	uint32 u32tick;			//!< 受信時刻
	uint8 u8Hops; 			//!< NWK層での中継回数
	uint8 u8RouteXOR;		//!< NWK層での中継経路の各アドレスを XOR 計算したもの。中継経路推定を行える
	uint8 u8LQI1st;			//!< NWK層での最初の中継機が受信したLQI値
	bool_t bSecure;			//!< 暗号化パケット
	bool_t bStored;			//!< 内部利用（格納済みのキューかどうかのフラグ）
} tsRxData;

/** @ingroup grp_ToCoNet_TxRxQueue
 * ライブラリ内部で使用する送信パケット格納構造体
 */
typedef struct {
	uint16 u16SrcPan, u16DstPan;
	uint32 u32SrcAddr, u32DstAddr, u32DstFinal;
	uint8 u8len;
	uint8 u8status;
	uint16 u16delay_max; // if set 0, transmit immediately.
	uint16 u16delay_min; // if set 0, transmit immediately.
	uint8 u8retry, u8txcount;
	uint16 u16retry_dur;
	uint8 auData[MAX_TX_APP_PAYLOAD];
	uint8 bAckReq;
	uint8 u8cbid; //!< Tx完了時のコールバック関数に渡されるID
	uint8 bSecurePacket; //!< 暗号化パケット
	uint8 bRelayPacket; //!< パケットデータに手を加えずそのまま送信する時のオプション
	void *pvCbFunc; // call back function to send back a finish message.
	void *pvOpt;		//!< モジュール内部で利用

	uint8 bStored;
	uint32 u32tick;

	// 失敗成功に関わらず、チャネルループを行う
	uint32 u32TxChList; //<! チャネルリストの設定値
	uint32 u32TxChListNotyet; //<! 未処理チャネルリスト
	uint8 u8TxChListNow; //<! 送信成功時のチャネル
} tsTxData;

/** @ingroup grp_ToCoNet_TxRxQueue
 * 受信パケットFIFOキュー
 */
typedef struct {
	tsRxData* sRxdatabuff; // Rx Data stored
	uint8 u8RxBuffSiz; // Rx Data Buffer size
	tsQueue sQRxdatabuff; // Reuse ByteQueue for FIFO
	uint8* au8Rxdatabuff_idx; // store index of sRxdatabuff
	uint8 u8RxEntry; // the index
} tsRxQueue;

/** @ingroup grp_ToCoNet_TxRxQueue
 * 送信パケットFIFOキュー
 */
typedef struct {
	tsTxData* sTxdatabuff; // Tx Data stored
	uint8 u8TxBuffSiz; // Tx Data Buffer size
	tsQueue sQTxdatabuff; // Reuse ByteQueue for FIFO
	uint8* au8Txdatabuff_idx; // store index of sRxdatabuff
	uint8 u8TxEntry; // the index
} tsTxQueue;

#ifdef __cplusplus
}
#endif // C++

#endif /* MAC_TXRXQUEUE_H_ */
