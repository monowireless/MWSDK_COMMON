/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/ //
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <PeripheralRegs.h>

#include "utils.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/ //
#define WRITE_REG32(A, B) *(volatile uint32 *)(A) = (B)
//#define WRITE_REG16(A, B) *(volatile uint16 *)(A) = (B)
//#define WRITE_REG8(A, B)  *(volatile uint8 *)(A) =(B)
#define READ_REG32(A)     *(volatile uint32 *)(A)
//#define READ_REG16(A)     *(volatile uint16 *)(A)
//#define READ_REG8(A)      *(volatile uint8 *)(A)
//#define RMW_REG32(A, B, C) vRMW_REG32(A,B,C)
#ifdef JN514x
#define REG_TMR0_BASE       0x02005000UL
#define REG_TMR1_BASE       0x02006000UL
#define REG_TMR2_BASE       0x0200A000UL
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/ //
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/ //
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/ //
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/ //
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/ //

// 割り算のコストが大きいため、特定の数で割るときはビットシフトで処理する。
// ※ 分岐コストのほうがずっと小さいため
inline uint32_t s_div_by(uint32_t lhs, uint32_t rhs) {
	switch (rhs) {
		case 1024: return lhs >> 10;
		case 4096: return lhs >> 12;
		case 16384: return lhs >> 14;
		default: return lhs / rhs;
	}
}

/****************************************************************************
 *
 * NAME: vTimerConfig, Start, Stop
 *
 * DESCRIPTION:
 *
 * RETURNS:
 *
 ****************************************************************************/ //

void vTimerConfig(tsTimerContext *psTC) {
	// set local variable
	switch (psTC->u8Device) {
	case E_AHI_DEVICE_TIMER0:
		psTC->u8AHI_TIMER_X = E_AHI_TIMER_0;
		psTC->u32reg = REG_TMR0_BASE;
		break;
	case E_AHI_DEVICE_TIMER1:
		psTC->u8AHI_TIMER_X = E_AHI_TIMER_1;
		psTC->u32reg = REG_TMR1_BASE;
		break;
	case E_AHI_DEVICE_TIMER2:
		psTC->u8AHI_TIMER_X = E_AHI_TIMER_2;
		psTC->u32reg = REG_TMR2_BASE;
		break;
#ifdef JN516x
	case E_AHI_DEVICE_TIMER3:
		psTC->u8AHI_TIMER_X = E_AHI_TIMER_3;
		psTC->u32reg = REG_TMR3_BASE;
		break;
	case E_AHI_DEVICE_TIMER4:
		psTC->u8AHI_TIMER_X = E_AHI_TIMER_4;
		psTC->u32reg = REG_TMR4_BASE;
		break;
#endif
	default:
		psTC->u8AHI_TIMER_X = 0xff;
		break;
	}

	// duty max
	if (psTC->u16duty_max == 0) {
		psTC->u16duty_max = 1024;
	}

	// if started, stop
	if (psTC->bStarted) {
		if (psTC->u8Device == E_AHI_DEVICE_TICK_TIMER) {
			vAHI_TickTimerIntEnable(FALSE);
			psTC->bStarted = FALSE;
		} else {
			vAHI_TimerStop(psTC->u8AHI_TIMER_X);
			vAHI_TimerDisable(psTC->u8AHI_TIMER_X);
		}

		psTC->bStarted = FALSE;
	}

	// register call back
	if (psTC->pvHandler != NULL) {
		switch (psTC->u8Device) {
		case E_AHI_DEVICE_TIMER0:
			vAHI_Timer0RegisterCallback(psTC->pvHandler);
			break;
		case E_AHI_DEVICE_TIMER1:
			vAHI_Timer1RegisterCallback(psTC->pvHandler);
			break;
		case E_AHI_DEVICE_TIMER2:
			vAHI_Timer2RegisterCallback(psTC->pvHandler);
			break;
#ifdef JN516x
		case E_AHI_DEVICE_TIMER3:
			vAHI_Timer3RegisterCallback(psTC->pvHandler);
			break;
		case E_AHI_DEVICE_TIMER4:
			vAHI_Timer4RegisterCallback(psTC->pvHandler);
			break;
#endif
		case E_AHI_DEVICE_TICK_TIMER:
			vAHI_TickTimerRegisterCallback(psTC->pvHandler);
			break;
		}
	}

	if (psTC->u8Device == E_AHI_DEVICE_TICK_TIMER) {
		// TICK TIMER
		vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);
		vAHI_TickTimerWrite(0);
	} else {
		// TIMER0,1,2
		vAHI_TimerEnable(psTC->u8AHI_TIMER_X, // TIMER DEVICE
				psTC->u8PreScale, // prescalar value
				FALSE, // INT at rising (low to high)
				psTC->bDisableInt ? FALSE : TRUE, // INT at the end of timer period (high to low)
				psTC->bPWMout); // output signal on DIO

		vAHI_TimerClockSelect(psTC->u8AHI_TIMER_X, // TIMER DEVICE
				FALSE, // use internal 16MHz clock
				TRUE); // gate input pin active high

		vAHI_TimerConfigureOutputs(psTC->u8AHI_TIMER_X, // TIMER DEVICE
				TRUE,// inversion PWM output
				TRUE// gate input disable
				);
	}
}

void vTimerStart(tsTimerContext *psTC) {
	uint32 u32timct;

	if (psTC->u8Device == E_AHI_DEVICE_TICK_TIMER) {
		// 1mS = CLOCK[Hz]/1000
		u32timct = u32IO_FREQ_HZ / psTC->u16Hz;

		vAHI_TickTimerWrite(0);
		vAHI_TickTimerInterval(u32timct);
		vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_RESTART);
		vAHI_TickTimerIntEnable(TRUE); // must place after starting of Tick Timer.
	} else {
		// 割り算を行うため u16ct_total を再利用する。
		u32timct = psTC->u16ct_total;
		if (u32timct == 0) {
			u32timct= u32IO_FREQ_HZ / psTC->u16Hz / (1UL << psTC->u8PreScale);
		}

		if (psTC->bStarted) {
			// PWM 時のDUTY比をタイマー再起動せずに行う。
			// API が提供されていないため、レジスタ書き換えによって行う。
			uint32 u32Reg = psTC->u32reg;
			if (u32Reg) {
				u32Reg += 4; // offset address
				uint32 u32LoPeriod = (u32timct * psTC->u16duty) / psTC->u16duty_max;

				WRITE_REG32(u32Reg, u32LoPeriod);
			}
		} else {
			vAHI_TimerStartRepeat(psTC->u8AHI_TIMER_X, // TIMER DEVICE
					s_div_by((u32timct * psTC->u16duty), psTC->u16duty_max), // (u32timct * psTC->u16duty) / psTC->u16duty_max, // low period (PWM)
					u32timct); // total period
		}

		psTC->u16ct_total = (uint16)u32timct;
		psTC->bStarted = TRUE;
	}
}

/**
 * タイマーの周波数、DUTY 比を変更する。
 * - 稼働済みタイマーに限定する
 * - PreScale は変更しない。PreScaleの範囲で計算可能な周波数とする事。
 * @param psTC
 * @param u32timct 0xFFFFまでのタイマートータルカウンタ値
 */
static void vTimerChangeHzCore(tsTimerContext *psTC, uint32 u32timct) {
	// タイマーが稼働していないときは、そのままリターン
	if (u32timct == 0 || psTC->bStarted == FALSE) {
		return;
	} else {
		if (u32timct > 0xFFFF) {
			return;
		}
	}

	if (psTC->bStarted) {
		// PWM 時のDUTY比をタイマー再起動せずに行う。
		// API が提供されていないため、レジスタ書き換えによって行う。

		uint32 u32Reg = psTC->u32reg;

		if (u32Reg) {
			u32Reg += 4; // offset address (Lo Period)
			WRITE_REG32(u32Reg, s_div_by((u32timct * psTC->u16duty), psTC->u16duty_max));

			u32Reg += 4; // offset address (Total Period)
			WRITE_REG32(u32Reg, u32timct);
		}
	}

	psTC->u16ct_total = (uint16)u32timct;
}

/**
 * タイマーの周波数、DUTY 比を変更する。
 * - psTC->u16Hz を使用する、通常版
 * @param psTC
 */
void vTimerChangeHz(tsTimerContext *psTC) {
	uint32 u32timct = (u32IO_FREQ_HZ / psTC->u16Hz) >> psTC->u8PreScale;
	vTimerChangeHzCore(psTC, u32timct);
}

/**
 * タイマーの周波数、DUTY 比を変更する。
 * - mHz (ミリヘルツ) 値を渡す高精度版。
 *
 * @param psTC
 */
void vTimerChangeHzEx(tsTimerContext *psTC, uint32 u32mHz) {
	uint32 u32timct = ((u32IO_FREQ_HZ*250UL) / (u32mHz/4)) >> (psTC->u8PreScale);
		// 16000000 * 1000 は 32bit を超えてしまうので、
		// 250と4に分けて計算した。
	vTimerChangeHzCore(psTC, u32timct);
}

/**
 * タイマーの DUTY 比のみを変更する
 * @param psTC タイマーコンテクスト
 * @param u16CtSyncThres FFFF:同期しない 1-...:同期時にカウントのスレッシュホールド
 */
void vTimerChangeDutyCore(tsTimerContext *psTC, uint16 u16CtSyncThres, uint32 (*pCallBack)(uint32)) {
	uint32 u32timct = psTC->u16ct_total;

	// タイマーが稼働していないときは、そのままリターン
	if (u32timct == 0 || psTC->bStarted == FALSE) {
		return;
	}

	if (psTC->bStarted) {
		// PWM 時のDUTY比をタイマー再起動せずに行う。
		// API が提供されていないため、レジスタ書き換えによって行う。

		uint32 u32Reg = 0;
		switch (psTC->u8Device) {
		case E_AHI_DEVICE_TIMER0: u32Reg = REG_TMR0_BASE; break;
		case E_AHI_DEVICE_TIMER1: u32Reg = REG_TMR1_BASE; break;
		case E_AHI_DEVICE_TIMER2: u32Reg = REG_TMR2_BASE; break;
#ifdef JN516x
		case E_AHI_DEVICE_TIMER3: u32Reg = REG_TMR3_BASE; break;
		case E_AHI_DEVICE_TIMER4: u32Reg = REG_TMR4_BASE; break;
#endif
		}
		if (u32Reg) {
			u32Reg += 4; // offset address
			uint32 u32LoPeriod = s_div_by((u32timct * psTC->u16duty), psTC->u16duty_max); // (u32timct * psTC->u16duty) / psTC->u16duty_max;

			if (u16CtSyncThres != 0xFFFF) {
				// 周期の頭にシンクロする(ただし少しオーバーランするはずだ)
				//uint16 u16CtLo = READ_REG32(psTC->u32reg + 4);
				uint16 u16CtTot = READ_REG32(psTC->u32reg + 8);
				uint16 u16CtThres = u16CtTot - u16CtSyncThres;

				if (pCallBack) pCallBack(0);
				while (READ_REG32(psTC->u32reg) < u16CtThres); // ポーリングでカウント値の条件が成立するまで待つ
			}
			WRITE_REG32(u32Reg, u32LoPeriod);
			if (pCallBack) pCallBack(1);
		}
	}
}

/**
 * タイマーの DUTY 比のみを変更する
 * @param psTC
 */
void vTimerChangeDuty(tsTimerContext *psTC) {
	vTimerChangeDutyCore(psTC, 0xFFFF, NULL);
}

/**
 * タイマーの DUTY 比のみを変更する
 * (タイマーの１周期境界に同期して動作させる)
 * @param psTC
 */
void vTimerChangeDutySync(tsTimerContext *psTC, uint16 u16CtSyncThes, uint32 (*pCallBack)(uint32)) {
	vTimerChangeDutyCore(psTC, u16CtSyncThes, pCallBack);
}

void vTimerStop(tsTimerContext *psTC) {
	if (psTC->bStarted) {
		if (psTC->u8Device == E_AHI_DEVICE_TICK_TIMER) {
			vAHI_TickTimerIntEnable(FALSE); // must place after starting of Tick Timer.
			vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_STOP);
		} else {
			vAHI_TimerStop(psTC->u8AHI_TIMER_X);
			psTC->u16ct_total = 0; // カウンタの計算値のクリア
		}
		psTC->bStarted = FALSE;
	}
}

void vTimerDisable(tsTimerContext *psTC) {
	if (psTC->u8Device == E_AHI_DEVICE_TICK_TIMER) {
		vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);
	} else {
		vAHI_TimerStop(psTC->u8AHI_TIMER_X);
		vAHI_TimerDisable(psTC->u8AHI_TIMER_X);
		psTC->u16ct_total = 0; // カウンタの計算値のクリア
	}

	psTC->bStarted = FALSE;
}

/****************************************************************************
 *
 * NAME: vAnalogueConfig
 *
 * DESCRIPTION:
 *
 * RETURNS:
 *
 ****************************************************************************/ //
void vAnalogueConfig(void) {
	if (!bAHI_APRegulatorEnabled()) {
		vAHI_ApConfigure(E_AHI_AP_REGULATOR_ENABLE, E_AHI_AP_INT_DISABLE,
				E_AHI_AP_SAMPLE_4, E_AHI_AP_CLOCKDIV_1MHZ, E_AHI_AP_INTREF);

		while (!bAHI_APRegulatorEnabled())
			;
	}
}

void vAnalogueDisable(void) {
	vAHI_ApConfigure(E_AHI_AP_REGULATOR_DISABLE, E_AHI_AP_INT_DISABLE,
			E_AHI_AP_SAMPLE_4, E_AHI_AP_CLOCKDIV_1MHZ, E_AHI_AP_INTREF);
}

/****************************************************************************
 *
 * NAME: vWait
 *
 * DESCRIPTION:
 *   just add a small wait by loop.
 *
 * RETURNS:
 *
 ****************************************************************************/ //
void vWait(uint32 c) {
	static volatile uint32 u32ct = 0;
	while (c-- > 0)
		u32ct++;
}
