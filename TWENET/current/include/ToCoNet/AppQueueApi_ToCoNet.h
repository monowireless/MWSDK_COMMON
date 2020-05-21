/*****************************************************************************
 *
 * MODULE:             Application API header
 *
 * COMPONENT:          AppQueueApi.h
 *
 * AUTHOR:             CJG
 *
 * DESCRIPTION:        Access functions and structures used by the application
 *                     to interact with
 *                     the Jennic 802.15.4 stack.
 *
 * $HeadURL: http://svn/sware/Projects/Jeneric/Modules/GDB/Trunk/Include/gdb.h $
 *
 * $Revision: 14602 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-06-18 15:10:40 +0100 (Thu, 18 Jun 2009) $
 *
 * $Id: gdb.h 14602 2009-06-18 14:10:40Z mwild $
 *
 *****************************************************************************
 *
 * This software is owned by Jennic and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Jennic products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on each
 * copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 *
 * Copyright Jennic Ltd. 2009 All rights reserved
 *
 ****************************************************************************/

/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */


#ifndef  APP_Q_API_H_INCLUDED
#define  APP_Q_API_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <mac_sap.h>
#include <AppApi.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef void (*PR_QIND_CALLBACK)();
typedef uint8 (*PR_QIND_CALLBACK_CUSTOM)(void *);
typedef uint8 (*PR_HWQINT_CALLBACK)(uint32, uint32);

typedef struct
{
    uint32 u32DeviceId;
    uint32 u32ItemBitmap;
} AppQApiHwInd_s;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC uint32 u32AppQApiInitT(PR_QIND_CALLBACK_CUSTOM prMlmeCallback,
                             PR_QIND_CALLBACK prMcpsCallback,
                             PR_HWQINT_CALLBACK prHwCallback);
PUBLIC uint32 u32AppQApiInitNetwork();

PUBLIC MAC_MlmeDcfmInd_s *psAppQApiReadMlmeIndT(void);
PUBLIC MAC_McpsDcfmInd_s *psAppQApiReadMcpsIndT(void);
PUBLIC AppQApiHwInd_s *psAppQApiReadHwIndT(void);
PUBLIC void vAppQApiReturnMlmeIndBufferT(MAC_MlmeDcfmInd_s *psBuffer);
PUBLIC void vAppQApiReturnMcpsIndBufferT(MAC_McpsDcfmInd_s *psBuffer);
PUBLIC void vAppQApiReturnHwIndBufferT(AppQApiHwInd_s *psBuffer);
PUBLIC void vAppQApiPostHwItT(uint32 u32Device, uint32 u32ItemBitmap);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* APP_Q_API_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

