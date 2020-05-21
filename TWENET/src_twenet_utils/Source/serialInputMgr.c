/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "serialInputMgr.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************///
extern volatile uint32 u32TickCount_ms; // time in ms

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: u8ProcessSerCmd
 *
 * DESCRIPTION: Process incoming byte to form binary input sequence.
 *   format:
 *     +0 +1 +2  +3 +4...4+len-1 4+len
 *     A5 5A lenH,L payload      XOR
 *
 *   timeout:
 *     the timestamp of first byte is recorded by reffering external value
 *     u32TickCount_ms, then calucalted elapsed time from starting byte.
 *     if the elapsed time excess SERCMD_TIMEOUT_ms, the coming byte is
 *     considered as first byte.
 *
 * PARAMETERS:      Name            RW  Usage
 *
 * RETURNS:
 *   state information is returned. once E_SERCMD_COMPLETE is returned,
 *   the input data shall be handled before calling this function.
 *   the next call will set tsSerCmd structure as new status.
 *
 *     - E_SERCMD_EMPTY : new state (no input)
 *     - E_SERCMD_ERROR : some error
 *     - E_SERCMD_READCRC : CRC error
 *     - E_SERCMD_COMPLETE : complete to input a command
 *
 * NOTES:
 ****************************************************************************/
//
bool_t bSerCmd_VerboseMode = FALSE;

extern volatile uint32 u32TickCount_ms; // system tick counter
PRIVATE uint32 u32TickVerbose;

PUBLIC uint8 u8ParseSerCmd(tsSerCmd *pCmd, uint8 u8byte) {
	// check for timeout
	if (pCmd->u8state != E_SERCMD_EMPTY) {
		if (u32TickCount_ms - pCmd->u32timestamp > SERCMD_TIMEOUT_ms) {
			pCmd->u8state = E_SERCMD_EMPTY; // start from new
		}
	}

	// check for verbose timeout
	if (bSerCmd_VerboseMode && u32TickCount_ms - u32TickVerbose > 300000) {
		// if no input came from UART, this routine does not work.
		bSerCmd_VerboseMode = FALSE;
	}

	// check for complete or error status
	if (pCmd->u8state >= 0x80) {
		pCmd->u8state = E_SERCMD_EMPTY;
	}

	// run state machine
	switch ((teSerCmd)(pCmd->u8state)) {
	case E_SERCMD_EMPTY:
		if (u8byte == SERCMD_SYNC_1) {
			pCmd->u8state = E_SERCMD_READSYNC;
			pCmd->u32timestamp = u32TickCount_ms; // store received time
		} else if (u8byte == '+') {
			pCmd->u32timestamp = u32TickCount_ms; // store received time
			pCmd->u8state = E_SERCMD_PLUS1;
		}
		break;
	case E_SERCMD_READSYNC:
		if (u8byte == SERCMD_SYNC_2) {
			pCmd->u8state = E_SERCMD_READLEN;
		} else {
			pCmd->u8state = E_SERCMD_ERROR;
		}
		break;
	case E_SERCMD_READLEN:
		if (u8byte & 0x80) {
			// long length mode (1...
			u8byte &= 0x7F;
			pCmd->u16len = u8byte;
			if (pCmd->u16len <= pCmd->u16maxlen) {
				pCmd->u8state = E_SERCMD_READLEN2;
			} else {
				pCmd->u8state = E_SERCMD_ERROR;
			}
		} else {
			// short length mode (1...127bytes)
			pCmd->u16len = u8byte;
			if (pCmd->u16len <= pCmd->u16maxlen) {
				pCmd->u8state = E_SERCMD_READPAYLOAD;
				pCmd->u16pos = 0;
				pCmd->u8xor = 0;
			} else {
				pCmd->u8state = E_SERCMD_ERROR;
			}
		}
		break;
	case E_SERCMD_READLEN2:
		pCmd->u16len = (pCmd->u16len * 256) + u8byte;
		if (pCmd->u16len <= pCmd->u16maxlen) {
			pCmd->u8state = E_SERCMD_READPAYLOAD;
			pCmd->u16pos = 0;
			pCmd->u8xor = 0;
		} else {
			pCmd->u8state = E_SERCMD_ERROR;
		}
		break;
	case E_SERCMD_READPAYLOAD:
		pCmd->au8data[pCmd->u16pos] = u8byte;
		pCmd->u8xor ^= u8byte; // update XOR checksum
		if (pCmd->u16pos == pCmd->u16len - 1) {
			pCmd->u8state = E_SERCMD_READCRC;
		}
		pCmd->u16pos++;
		break;
	case E_SERCMD_READCRC:
		if (u8byte == pCmd->u8xor) {
			pCmd->u8state = E_SERCMD_COMPLETE;
		} else {
			pCmd->u8xor = u8byte;
			pCmd->u8state = E_SERCMD_CRCERROR;
		}
		break;

	case E_SERCMD_PLUS1:
		if ((u8byte == '+') && (u32TickCount_ms - pCmd->u32timestamp > 200) && (u32TickCount_ms - pCmd->u32timestamp < 1000)) {
			pCmd->u8state = E_SERCMD_PLUS2;
			pCmd->u32timestamp = u32TickCount_ms;
		} else {
			pCmd->u8state = E_SERCMD_ERROR;
		}
		break;
	case E_SERCMD_PLUS2:
		if ((u8byte == '+') && (u32TickCount_ms - pCmd->u32timestamp > 200) && (u32TickCount_ms - pCmd->u32timestamp < 1000)) {
			pCmd->u8state = E_SERCMD_VERBOSE;
			bSerCmd_VerboseMode = bSerCmd_VerboseMode ? FALSE : TRUE;
			if (bSerCmd_VerboseMode) {
				u32TickVerbose = u32TickCount_ms;
			}
		} else {
			pCmd->u8state = E_SERCMD_ERROR;
		}
		break;

	default:
		break;
	}

	return pCmd->u8state;
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
