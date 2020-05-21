/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * Utils.h
 *
 *  Created on: 2012/02/11
 *      Author: seigo13
 */

#ifndef TWETIMERS_H_
#define TWETIMERS_H_

#include <jendefs.h>

#ifdef __cplusplus
extern "C" {
#endif // C++

// Timer context for vTimerXXX, vTickTimerXXX
typedef struct _tsTimerContext {
	uint8 u8PreScale; //!< pre scale value (for vTimerXXX)
	uint8 u8Device; //!< device (for vTimerXXX)
	uint8 u8AHI_TIMER_X; // E_AHI_TIMER0 ... 2
	bool_t bStarted; //!< flag if timer is running or not.

	uint16 u16Hz; //!< frequency
	uint16 u16ct_total; //!< 計算された１周期のカウント数

	uint16 u16duty; //!< PWM のDUTY比 0:0% 1024:100%
	uint16 u16duty_max; //!< PWM の DUTY比の最大値。0 で初期化した場合は、1024 に設定される。
	bool_t bPWMout; //!< TRUE ならPWM出力
	bool_t bDisableInt; //!< TRUE なら割り込み無し

	uint32 u32reg; //!< TIMER のレジスタアドレス

	void (*pvHandler)(uint32, uint32); // event handler
} tsTimerContext;

/****************************************************************************
 * Prototype Declaration
 ***************************************************************************/ //
void vTimerConfig(tsTimerContext *psTC);
void vTimerStart(tsTimerContext *psTC);
void vTimerStop(tsTimerContext *psTC);
void vTimerDisable(tsTimerContext *psTC);
void vTimerChangeHz(tsTimerContext *psTC);
void vTimerChangeHzEx(tsTimerContext *psTC, uint32 u32mHz);
void vTimerChangeDuty(tsTimerContext *psTC);
void vTimerChangeDutySync(tsTimerContext *psTC, uint16 u16CtSyncThes, uint32 (*pCallBack)(uint32));

#ifdef __cplusplus
}
#endif // C++

#endif /* TWETIMERS_H_ */
