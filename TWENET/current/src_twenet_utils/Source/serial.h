/****************************************************************************
 * $Rev:: 1946              $: Revision of last commit
 * $Author:: snich          $: Author of last commit
 * $Date:: 2009-02-03 09:51#$: Date of last commit
 * $HeadURL: http://svn/apps/Reference_Designs/JN-RD-6009-Active-RFID/Modules/BoardSoftware/Tags/Release_1v3/802.15.4_Toolbox/Serial/serial.h $
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
#ifndef  SERIAL_H_INCLUDED
#define  SERIAL_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "uart.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/** @ingroup grp_Serial
 * Serial port descriptor used to specify port parameters.
 * The user application must create buffers for the
 * TX and RX queues when initialising the serial port.
 */
typedef struct
{
    uint8 *pu8SerialRxQueueBuffer; /** Pointer to the RX queue buffer */
    uint8 *pu8SerialTxQueueBuffer; /** Pointer to the TX queue buffer */
    uint32 u32BaudRate; /* Baud rate to use, i.e. 115200 */
    uint16 u16AHI_UART_RTS_LOW; /** RX buffer level to mark RTS (request to send)
    line High to stop the remote device sending data, set to 0xffff to ignore*/
    uint16 u16AHI_UART_RTS_HIGH; /** RX buffer level to mark RTS (request to send)
    line low to start the remote device sending data, set to 0xffff to ignore*/
    uint16 u16SerialRxQueueSize; /** Buffer size pointed to pu8SerialRxQueueBuffer */
    uint16 u16SerialTxQueueSize; /** Buffer size pointed to pu8SerialTxQueueBuffer */
    uint8 u8SerialPort; /** Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1 */
    uint8 u8RX_FIFO_LEVEL; /**  UART FIFO level when to issue a RX interrupt. */
}tsSerialPortSetup;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void     SERIAL_vInit(tsSerialPortSetup *psSetup);
PUBLIC void 	SERIAL_vInitEx(tsSerialPortSetup *psSetup, tsUartOpt *psUart);
PUBLIC bool_t   SERIAL_bTxChar(uint8 u8SerialPort,uint8 u8Chr);
PUBLIC bool_t   SERIAL_bTxString(uint8 u8SerialPort, uint8 *pu8Str);
PUBLIC int16    SERIAL_i16RxChar(uint8 u8SerialPort);
PUBLIC uint32   SERIAL_u32RxString(uint8 u8SerialPort, uint8 *pu8RxBuffer, uint32 u32BuffSize);
PUBLIC bool_t   SERIAL_bRxQueueEmpty(uint8 u8SerialPort);
PUBLIC bool_t   SERIAL_bTxQueueEmpty(uint8 u8SerialPort);
PUBLIC void     SERIAL_vTxReady(uint8 u8SerialPort);
PUBLIC void 	SERIAL_vFlush(uint8 u8Uart);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* SERIAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
