/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * ToCoNet_mod_NbScan.h
 *
 *  Created on: 2013/01/09
 *      Author: seigo13
 */

/**
 * @defgroup grp_ToCoNet_NbScan ToCoNet モジュール - Neighbour Scan (近隣探索)
 *
 * Neighbour Scan は、近隣にどのようなノードが存在するかを探索する。
 * -# チャネルマスクに指定された各チャネルに対して、ブロードキャストパケットにより応答要求を出力する\n
 *    応答要求は３回連続で送信される。
 * -# オプションで指定された時間中、スレーブ側のモジュール(mod_NbScan_Slave) を登録したノードからの応答を待つ。
 * -# 近隣ノードは探索要求を受信後、探索条件に合致していた場合、ランダムなタイミングで送信元へ応答を返す。
 * -# 探索条件に合致するノードの情報を内部に記録する。LQIが大きいものを優先して登録する。
 * -# 探索が完了すればアプリケーションまたは呼び出し元のモジュールへのコールバックによりイベント通知を行う。
 *
 */
#ifndef TOCONET_MOD_NBSCAN_H_
#define TOCONET_MOD_NBSCAN_H_

#include <jendefs.h>
#include "ToCoNet_event.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

/** @ingroup grp_ToCoNet_NbScan
 * 探索結果リストの最大数
 */
#define NBSCAN_MAXLIST 16

/** @ingroup grp_ToCoNet_NbScan
 * 探索モードのマスク(通常探索)
 */
#define TOCONET_NBSCAN_NORMAL_MASK 0x1

/** @ingroup grp_ToCoNet_NbScan
 * 探索モードのマスク(アドレスを指定し、そのノードが応答を返すかどうかの簡易探索)
 */
#define TOCONET_NBSCAN_QUICK_EXTADDR_MASK 0x10

/** @ingroup grp_ToCoNet_NbScan
 * 探索された１ノード分の情報
 */
typedef struct {
	uint32 u32addr; //!< MAC ADDR (0xFF...FF は無効)
	uint16 u16addr; //!< SHORT ADDR (0xFFFF は無効)
	uint16 u16app_version; //!< アプリケーションバージョン (未使用)
	uint16 u16nwk_version; //!< ネットワークバージョン (未使用)
	uint8 u8ch; //!< チャネル
	uint8 u8lqi; //!< 受信パケットのLQI
	uint8 u8app_role; //!< アプリケーションで任意指定 (未使用)
	uint8 u8nwk_role; //!< ネットワークでの役割、中継機など
	uint8 u8nwk_layer; //!< ネットワーク階層 (親機までのホップ数)
	uint8 u8nwk_layer_sub; //!< ネットワークサブ階層 (階層のトップまでのホップ数)
	uint8 u8nwk_status; //!< 動作中等のステータス
	uint8 u8nwk_session_id; //!< Scan 時のネットワーク接続ID
	uint8 bFound; //!< 構造体に値が入っていれば TRUE
} tsToCoNet_NbScan_Entitiy;


/** @ingroup grp_ToCoNet_NbScan
 * 探索結果情報
 */
typedef struct {
	uint8 u8found; //!< 検索数
	uint8 u8scanMode; //!< 探索モード
	uint8 u8IdxLqiSort[NBSCAN_MAXLIST]; //!< LQIソート済みのインデックス
	tsToCoNet_NbScan_Entitiy sScanResult[NBSCAN_MAXLIST]; //!< 探索結果
} tsToCoNet_NbScan_Result;

#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_MOD_NBSCAN_H_ */
