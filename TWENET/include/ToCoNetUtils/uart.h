/****************************************************************************
 * $Rev:: 1946              $: Revision of last commit
 * $Author:: snich          $: Author of last commit
 * $Date:: 2009-02-03 09:51#$: Date of last commit
 * $HeadURL: http://svn/apps/Reference_Designs/JN-RD-6009-Active-RFID/Modules/BoardSoftware/Tags/Release_1v3/802.15.4_Toolbox/Serial/uart.h $
 ****************************************************************************
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
 * Copyright Jennic Ltd 2007,2008. All rights reserved
 ****************************************************************************/

#ifndef  UART_H_INCLUDED
#define  UART_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
	bool_t bHwFlowEnabled; // TRUE, FALSE
	bool_t bParityEnabled; // TRUE, FALSE
	uint8 u8ParityType; // E_AHI_UART_EVEN_PARITY, E_AHI_UART_ODD_PARITY
	uint8 u8StopBit; // E_AHI_UART_1_STOP_BIT, E_AHI_UART_2_STOP_BIT
	uint8 u8WordLen; // 5-8 を指定 (E_AHI_UART_WORD_LEN_? は指定しない)
} tsUartOpt;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void 	UART_vInit(uint8 u8UARTNum,
                       void (*pfRxFn)(uint8,uint8),
                       void (*pfTxRdy)(uint8),
                       uint32 u32BaudRate,
                       uint8 u8RX_FIFO_LEVEL,
                       tsUartOpt *pOpt);
PUBLIC void 	UART_vTxChar(uint8 u8UARTNum,uint8 u8RxChar);
PUBLIC bool_t 	UART_bTxReady(uint8 u8UARTNum);
PUBLIC void 	UART_vRtsStartFlow(uint8 u8UARTNum);
PUBLIC void 	UART_vRtsStopFlow(uint8 u8UARTNum);
PUBLIC void 	UART_vSetTxInterrupt(uint8 u8UARTNum,bool_t bState);
PUBLIC void 	UART_vSetBaudRate(uint8 u8UARTNum,uint32 u32BaudRate);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* UART_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


