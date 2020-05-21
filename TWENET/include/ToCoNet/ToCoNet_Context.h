/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * ToCoNet_main.h
 *
 *  Created on: 2013/01/08
 *      Author: seigo13
 */

#ifndef _TOCONET_CONTEXT_H_
#define _TOCONET_CONTEXT_H_

#include <jendefs.h>
#include "twetimers.h"
#include "fprintf.h"

#undef USE_HIGHER_TICK_COUNTER

typedef struct {
	// toconet is started
	bool_t bToCoNetStarted;

	// mac init has been initialized?
	bool_t bMacInitialized; // Mac Initialize state
	bool_t bMacStarted; // Mac Started status
	bool_t bMacBusy; // on some long process (etc, energy scan)
	uint32 u32TickMacBusy; // start tick of longer process (will time out, if hold too much)

	// sleep state
	uint8 bKeepStateMachineOnWakeup;

	// Timer object
	tsTimerContext sTimerApp;

	// frame count
	uint8 u8FrameCtCtl;
	// uint8 u8FrameCtApp; // not necessary???

	// RC Oscillator calibration
	uint32 u32RcOscError;

	// debug
	tsFILE *pSerStream;
	uint8 u8debug_level;

	// strong module
	bool_t bStrongModule;

	// version
	uint32 u32Version;

	// Serial
	uint32 u32MySerial;

	// The first random number
	uint16 u16RandValue;

	// Count up ms for every tick
#ifdef USE_HIGHER_TICK_COUNTER
	uint16 u16TickDelta_10us;  //!< 10uS 単位のカウンタの差分
#else
	uint8 u8TickDelta_ms;
#endif

	// 設定チャネル
	uint8 u8ChannelToSet;

	// Sleep Control
	bool_t bSleepOnProcess; //!< スリープ呼び出し中のときだけフラグが立つ

	// security key is registered.
	bool_t bSecureEnabled;

	// 稼働状態
	uint32 u32BootStage;
} tsToCoNet_Context;
extern tsToCoNet_Context sToCoNet_Context;

#endif