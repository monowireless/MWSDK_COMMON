/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

#include <jendefs.h>
#include "fprintf.h"

// SPRINTF
PRIVATE uint16 SPRINTF_MAX_BUFF;
PRIVATE uint8 *au8sprintf;
PRIVATE uint8 u8sprintf_idx = 0;

PRIVATE tsFILE SPRINTF_sStream;
tsFILE *SPRINTF_Stream;

PRIVATE bool_t SPRINTF_bPutChar(uint8 u8SerialPort,uint8 u8Chr)
{
	if (au8sprintf == NULL || u8sprintf_idx >= SPRINTF_MAX_BUFF - 2) {
		return FALSE; // max 253chars
	}

	au8sprintf[u8sprintf_idx] = u8Chr;

	u8sprintf_idx++;
	au8sprintf[u8sprintf_idx] = 0;

	return TRUE;
}

#define FUNC_DEF(num) PUBLIC void SPRINTF_vInit##num() { \
	static uint8 au8buff[num]; \
	SPRINTF_sStream.bPutChar = SPRINTF_bPutChar; \
	SPRINTF_MAX_BUFF = num ; \
	au8sprintf = au8buff; \
	SPRINTF_Stream = &SPRINTF_sStream; \
}

FUNC_DEF(32)
FUNC_DEF(64)
FUNC_DEF(128)
FUNC_DEF(256)
FUNC_DEF(512)
FUNC_DEF(1024)

PUBLIC void SPRINTF_vRewind() {
	u8sprintf_idx = 0;
}

PUBLIC uint8* SPRINTF_pu8GetBuff() {
	return au8sprintf;
}

PUBLIC uint16 SPRINTF_u16Length() {
	return u8sprintf_idx;
}

PUBLIC uint32 u32string2hex(uint8 *p, uint8 u8len) {
	uint32 u32res = 0;

	if (u8len > 8) return 0;

	while(u8len--) {
		u32res <<= 4;
		if (*p >= '0' && *p <= '9') {
			u32res += *p -'0';
		} else if (*p >= 'A' && *p <= 'F') {
			u32res += *p - 'A' + 10;
		} else if (*p >= 'a' && *p <= 'f') {
			u32res += *p - 'a' + 10;
		}
		p++;
	}

	return u32res;
}

PUBLIC uint32 u32string2dec(uint8 *p, uint8 u8len) {
	uint32 u32res = 0;

	if (u8len > 8) return 0;

	while(u8len--) {
		u32res *= 10;
		if (*p >= '0' && *p <= '9') {
			u32res += *p -'0';
		} else if (*p >= 'A' && *p <= 'F') {
			u32res += *p - 'A' + 10;
		} else if (*p >= 'a' && *p <= 'f') {
			u32res += *p - 'a' + 10;
		}
		p++;
	}

	return u32res;
}
