/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <string.h>

#include "jendefs.h"

#include <AppHardwareApi.h>

#include "btnMgr.h"
#include "utils.h"

#include "intr.h"
#include "Interrupt.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define BTM_STAT_INIT_STAGE1 0
#define BTM_STAT_INIT_STAGE2 1
#define BTM_STAT_REGULAR 0x10

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************///
typedef struct {
	uint32 bmPortMask;
	uint32 bmPort;
	uint32 bmHistory[16];
	uint32 bmState; // 0:fixed 1:changing
	uint32 bmChanged; // 1:newly fixed
	uint8 u8idx;
	uint8 u8state;
	uint8 u8count;
	uint8 u8MaxBmHistory;

	tsBTM_Config sConfig;
	bool_t bStarted;
	uint16 u16TickDelta;
	uint16 u16TickCount;
} tsBtmCtl;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************///
static void vBTM_TimerISR(uint32 u32Device, uint32 u32ItemBitmap);
static void vBTM_ISR(uint16 u16TickDelta);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************///

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************///
PRIVATE tsBtmCtl sBtmCtl;
PRIVATE tsTimerContext sTimer;

#ifdef SERIAL_DEBUG
extern tsFILE sOutptStream;
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************///

/****************************************************************************/
/***        Functions                                                     ***/
/****************************************************************************///

static void vBTM_Init_Common(uint32 bmPortMask) {
	// initialize the context
	memset((void*)&sBtmCtl, 0x0, sizeof(tsBtmCtl));
	// sBtmCtl.bmState = 1 << au8PortMap[0] - 1; // set all 1

	memset((void*)&sTimer, 0x0, sizeof(sTimer));

	// store port mask
	sBtmCtl.bmPortMask = bmPortMask;

	// Set DIO as INPUT
	vAHI_DioSetDirection(sBtmCtl.bmPortMask, 0);
}

PR_BTM_HANDLER prBTM_InitExternal(tsBTM_Config *sConf) {
	vBTM_Init_Common(sConf->bmPortMask);
	sBtmCtl.u16TickDelta = sConf->u16Tick_ms;
	sBtmCtl.u8MaxBmHistory = sConf->u8MaxHistory;

	sTimer.u8Device = sConf->u8DeviceTimer;

	if (sTimer.u8Device != 0xFF) {
		// configure timer
		sTimer.pvHandler = vBTM_TimerISR;
		sTimer.u16Hz = 1000/sConf->u16Tick_ms;
		sTimer.u8PreScale = 6;
		sTimer.bStarted = 0;

		vTimerConfig(&sTimer);

		return NULL;
	} else {
		return vBTM_ISR;
	}
}

void vBTM_Init(uint32 bmPortMask, uint8 u8TimerDevice) {
	vBTM_Init_Common(bmPortMask);

	sBtmCtl.u8MaxBmHistory = 5;

	// configure timer
	sTimer.pvHandler = vBTM_TimerISR;
	sTimer.u16Hz = 200;
	sTimer.u8Device = u8TimerDevice;
	sTimer.u8PreScale = 6;
	sTimer.bStarted = 0;

	vTimerConfig(&sTimer);

	// exits
	return;
}

PUBLIC void vBTM_Enable() {
	sBtmCtl.bStarted = TRUE;
	if (sTimer.u8Device != 0xFF) {
		vTimerStart(&sTimer);
	}
}

PUBLIC void vBTM_Disable() {
	sBtmCtl.bStarted = FALSE;
	if (sTimer.u8Device != 0xFF) {
		vTimerStop(&sTimer);
	}
}

PUBLIC bool_t bBTM_GetState(uint32 *pbmPorts, uint32 *pbmChanged) {
	uint8 u8stat;
	MICRO_INT_STORAGE;

	// store data in critical section.
	MICRO_INT_ENABLE_ONLY(0);
	*pbmPorts = sBtmCtl.bmPort;
	*pbmChanged = sBtmCtl.bmChanged;
	u8stat = sBtmCtl.u8state;
	sBtmCtl.bmChanged = 0; // clear changed flag.
	MICRO_INT_RESTORE_STATE();

	if (u8stat != BTM_STAT_REGULAR) {
		return FALSE;
	} else {
		return TRUE;
	}
}

/** @ingroup grp_Utils_BTM
 * 外部より呼び出される割り込みハンドラ。引数に割り込み区間を指定する。
 * @param u16TickDelta 割り込み区間 (ms)
 */
static void vBTM_ISR(uint16 u16TickDelta) {
	sBtmCtl.u16TickCount += u16TickDelta;

	if (sBtmCtl.bStarted && sBtmCtl.u16TickCount >= sBtmCtl.u16TickDelta) {
		sBtmCtl.u16TickCount = 0;
		vBTM_TimerISR(0, 0);
	}
}

/** @ingroup grp_Utils_BTM
 * 割り込みハンドラ。IO入力と連照判定を行う。
 *
 * @param u32Device 未使用（割り込みハンドラから呼ばれる）
 * @param u32ItemBitmap 未使用（割り込みハンドラから呼ばれる）
 */
static void vBTM_TimerISR(uint32 u32Device, uint32 u32ItemBitmap) {
	int i;

	// update index
	sBtmCtl.u8idx++;
	if (sBtmCtl.u8idx >= sBtmCtl.u8MaxBmHistory) {
		sBtmCtl.u8idx = 0;
	}

	// read all IO ports and store it into the history table
	uint32 bmRead = u32AHI_DioReadInput(); // Api returns as Hi=1, Lo=0
	bmRead = ~bmRead & sBtmCtl.bmPortMask; // Now Hi=0, Lo=1
	sBtmCtl.bmHistory[sBtmCtl.u8idx] = bmRead;

	// check changed bits
	uint32 bmChanged = 0;
	for (i = 0; i < sBtmCtl.u8MaxBmHistory - 1; i++) {
		bmChanged |= sBtmCtl.bmHistory[i] ^ sBtmCtl.bmHistory[i + 1];
	}

	// DEBUG
	//vfPrintf(&sOutptStream, "*");

	// wait for start status
	switch (sBtmCtl.u8state) {
	case BTM_STAT_INIT_STAGE1: // initial stage 1 (fill history)
		sBtmCtl.u8count++;
		if (sBtmCtl.u8count > sBtmCtl.u8MaxBmHistory) {
			sBtmCtl.u8state = BTM_STAT_INIT_STAGE2;
		}
		break;
	case BTM_STAT_INIT_STAGE2: // initial stage 2 (wait for stabilize)
		if (bmChanged)
			break;
		else
			sBtmCtl.u8state = BTM_STAT_REGULAR;

		sBtmCtl.bmChanged = sBtmCtl.bmPortMask;
		sBtmCtl.bmPort = sBtmCtl.bmHistory[sBtmCtl.u8idx];
		sBtmCtl.bmState = 0;
		break;
	case BTM_STAT_REGULAR: // regular update
		_C {
			// the mask indicates, previously fluctuated, but now stable.
			uint32 bmNewlyUpdated = sBtmCtl.bmState & ~bmChanged;

			// find new changed port value.
			uint32 bmPortUpdated = sBtmCtl.bmHistory[sBtmCtl.u8idx]
					& bmNewlyUpdated;
			sBtmCtl.bmPort &= ~bmNewlyUpdated;
			sBtmCtl.bmPort |= bmPortUpdated;

			// update bmState
			sBtmCtl.bmState = bmChanged;

			// record changed ports
			sBtmCtl.bmChanged |= bmNewlyUpdated;
		}
		break;
	default:
		sBtmCtl.u8state = BTM_STAT_INIT_STAGE1; // back to initial
		break;
	}

}
