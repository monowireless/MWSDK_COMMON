/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/**
 * @defgroup grp_ToCoNet_BeaconHistory ビーコンの履歴
 *
 * シーケンス番号で運用されるパケットの重複を確認する
 */

#ifndef TOCONET_MOD_BEACONHISTORY_H_
#define TOCONET_MOD_BEACONHISTORY_H_

#include <string.h>
#include <jendefs.h>

#ifdef __cplusplus
extern "C" {
#endif // C++

extern volatile uint32 u32TickCount_ms;

#define TOCONET_BEACON_HISTORY_ELEMENT_ITEMS 8
#define TOCONET_BEACON_HISTORY_TIMEOUT 120
typedef struct {
	uint32 u32Address;   //!< ビーコンのアドレス
	uint8 u8count;       //!< 一定時間で得られたビーコンの回数
	uint8 u8elapsed;   //!< 一番最後のビーコンからの経過時間
	uint16 u16lqi_total; //!< LQI値の合計
	uint8 au8Lqi[TOCONET_BEACON_HISTORY_ELEMENT_ITEMS]; //!< LQI
	uint8 au8Tick[TOCONET_BEACON_HISTORY_ELEMENT_ITEMS]; //!< 時間
} tsToCoNet_BeaconHistoryElement;

void ToCoNet_BHist_vInit(uint8 u8Timeout);
uint32 ToCoNet_BHist_u32Tick();
uint32 ToCoNet_BHist_u32Add(uint32 u32addr, uint8 u8Lqi);

#ifdef PC_DEBUG
extern bool_t bVerbose;
#define _C if(1)
#define DBGOUT(lv, ...) if (bVerbose) fprintf(stdout, __VA_ARGS__); fflush(stderr);
#define LB "\n"
#endif

#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_MOD_BEACONHISTORY_H_ */
