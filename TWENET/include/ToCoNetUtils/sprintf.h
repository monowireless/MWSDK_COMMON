/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

#ifndef SPRINTF_H_
#define SPRINTF_H_

#include <jendefs.h>
#include "fprintf.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

PUBLIC void SPRINTF_vInit32();
PUBLIC void SPRINTF_vInit64();
PUBLIC void SPRINTF_vInit128();
PUBLIC void SPRINTF_vInit256();
PUBLIC void SPRINTF_vInit512();
PUBLIC void SPRINTF_vInit1024();

PUBLIC void SPRINTF_vRewind();
PUBLIC uint8* SPRINTF_pu8GetBuff();
PUBLIC uint16 SPRINTF_u16Length();

PUBLIC uint32 u32string2hex(uint8 *p, uint8 u8len);
PUBLIC uint32 u32string2dec(uint8 *p, uint8 u8len);

extern tsFILE *SPRINTF_Stream;

#ifdef __cplusplus
}
#endif // C++

#endif /* SPRINTF_H_ */
