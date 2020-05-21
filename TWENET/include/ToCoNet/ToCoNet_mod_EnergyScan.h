/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * ToCoNet_mod_EnregyScan.h
 *
 *  Created on: 2013/01/09
 *      Author: seigo13
 */

/**
 * @defgroup grp_ToCoNet_EnergySacn ToCoNet モジュール - エナジースキャン
 * チャネルのエナジーレベル（測定時間中の入力信号レベル）を測定する。
 * - 複数チャネルの測定も可能だが測定時間はチャネル数に比例する
 * - 各チャネルの測定時間は指定可能
 * - 測定期間中は送受信が出来ない
 */

#ifndef TOCONET_MOD_ENERGY_SCAN_H_
#define TOCONET_MOD_ENERGY_SCAN_H_

#include <jendefs.h>
#include <mac_pib.h>
#include <mac_sap.h>

/** @defgroup grp_ToCoNet_EnergySacn
 * EnergyScan 管理構造体
 * - 結果は uint8 u8Energy[] 配列のアドレスとして返される。
 *   - u8Energy[0]    結果格納数
 *   - u8Energy[1..]  結果
 */
typedef struct {
	void (*vHandleResult)(MAC_MlmeDcfmInd_s *); //!< 内部処理用のコールバック関数
	uint8 u8Energy[17]; //!< 結果格納配列
} tsToCoNet_EnergyScan_Context;

#endif /* TOCONET_MOD_ENERGY_SCAN_H_ */
