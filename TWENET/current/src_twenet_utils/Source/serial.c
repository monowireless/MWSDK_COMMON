/****************************************************************************
 * $Rev:: 1946              $: Revision of last commit
 * $Author:: snich          $: Author of last commit
 * $Date:: 2009-02-03 09:51#$: Date of last commit
 * $HeadURL: http://svn/apps/Reference_Designs/JN-RD-6009-Active-RFID/Modules/BoardSoftware/Tags/Release_1v3/802.15.4_Toolbox/Serial/serial.c $
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
/** @file
 * Buffered Serial IO
 *
 * @defgroup grp_Serial Serial Utilities
 * Generic buffered UART serial communications with RX flow control on RTS.
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>
#include "Interrupt.h"
#include "uart.h"
#include "ByteQueue.h"
#include "serial.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define MAX_SERIAL_PORTS    2
#define CR_CHAR             0x0DU
#define LF_CHAR             0x0AU

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/**
 * Serial Port Descriptor.Used to encapsulate all data related
 * to the serial port.
 *
 * The user application will supply the buffers that will be used in
 * sRxQueue and sTxQueue.
 */
typedef struct
{
    tsQueue sRxQueue; 				/** RX Queue */
    tsQueue sTxQueue; 				/** TX Queue */
    uint16 u16AHI_UART_RTS_LOW;  	/** RTS Low Mark */
    uint16 u16AHI_UART_RTS_HIGH; 	/** RTS High Mark */
}tsSerialPort;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vRxChar(uint8,uint8 u8Char);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
// enable flag of installing interrupt.
int _SERIAL_bmTxIntEnabled;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/**
 *	Serial port instances.
 */
tsSerialPort asSerialPorts[MAX_SERIAL_PORTS];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/** @ingroup grp_Serial
 *
 * Initialise a serial port.  The port must be initialised before read or
 * writing data from/to it.
 *
 * @param psSetup Pointer to configuration information.
 * This data will not be referenced after the function completes.
 *
 */
PUBLIC void SERIAL_vInit(tsSerialPortSetup *psSetup)
{
	SERIAL_vInitEx(psSetup, NULL);
}

/** @ingroup grp_Serial
 *
 * Initialise a serial port.  The port must be initialised before read or
 * writing data from/to it.
 *
 * @param psSetup Pointer to configuration information.
 * @param psUartOpt Pointer to UART option
 * This data will not be referenced after the function completes.
 *
 */
PUBLIC void SERIAL_vInitEx(tsSerialPortSetup *psSetup, tsUartOpt *psUartOpt)
{

    if (psSetup->u8SerialPort > 1){
        return;
    }

    QUEUE__vInit(&asSerialPorts[psSetup->u8SerialPort].sRxQueue,
                 psSetup->pu8SerialRxQueueBuffer,
                 psSetup->u16SerialRxQueueSize);

    QUEUE__vInit(&asSerialPorts[psSetup->u8SerialPort].sTxQueue,
                 psSetup->pu8SerialTxQueueBuffer,
                 psSetup->u16SerialTxQueueSize);

    asSerialPorts[psSetup->u8SerialPort].u16AHI_UART_RTS_LOW = psSetup->u16AHI_UART_RTS_LOW;
    asSerialPorts[psSetup->u8SerialPort].u16AHI_UART_RTS_HIGH = psSetup->u16AHI_UART_RTS_HIGH;

    vAHI_UartSetRTSCTS(psSetup->u8SerialPort, FALSE); // release the Port

    UART_vInit(psSetup->u8SerialPort,
               vRxChar,
               SERIAL_vTxReady,
               psSetup->u32BaudRate,
               psSetup->u8RX_FIFO_LEVEL,
               psUartOpt);

    //UART_vRtsStartFlow(psSetup->u8SerialPort); /* [I SP001222_P1 284]*/// Set RTS as Low
}


/** @ingroup grp_Serial
 * Queues a byte for transmission.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 * @param u8Chr Byte to transmit.
 * @return TRUE if queue successfully. FALSE if queue is full.
 */
PUBLIC bool_t SERIAL_bTxChar(uint8 u8SerialPort,uint8 u8Chr)
{
	//uint32 u32IntStore;
    bool_t bStatus = TRUE;

    /*
     * prevent race condition where transmitter is busy, but becomes ready
     * between reading its status and queueing the character to transmit
     */
	MICRO_INT_STORAGE;
	MICRO_INT_ENABLE_ONLY(0);
    if (QUEUE_bEmpty(asSerialPorts[u8SerialPort].sTxQueue) && UART_bTxReady(u8SerialPort)) {
        if (!(_SERIAL_bmTxIntEnabled & (1 << u8SerialPort))) {
            UART_vSetTxInterrupt(u8SerialPort,TRUE); // MW (should do every time? modified to do only once)
            _SERIAL_bmTxIntEnabled |= (1 << u8SerialPort);
        }

        UART_vTxChar(u8SerialPort,u8Chr);
    } else {
        if (!QUEUE_bFull(asSerialPorts[u8SerialPort].sTxQueue)) {
        	QUEUE_vAddItem(asSerialPorts[u8SerialPort].sTxQueue, u8Chr,FALSE);
        } else {
            bStatus = FALSE;
        }
    }
    MICRO_INT_RESTORE_STATE();

    return bStatus;
}

/** @ingroup grp_Serial
 *
 * Add a NULL terminate string of characters to the transmit queue.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 * @param pu8Str Pointer to string to be transmitted.
 * @return TRUE if successful, false if queue fills up before last char is added.
 */
PUBLIC bool_t SERIAL_bTxString(uint8 u8SerialPort, uint8 *pu8Str)
{
    bool_t bResult = TRUE;

    while(*pu8Str)
    {
        if (SERIAL_bTxChar(u8SerialPort, *pu8Str))
        {
            pu8Str++;
        }
        else
        {
            bResult = FALSE;
            break;
        }
    }
    return bResult;
}

/** @ingroup grp_Serial
 *
 * Get a byte from the RX queue.  The function will return -1 if the buffer
 * is empty.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 * @return 8bit data or -1 if no data.
 */
PUBLIC int16 SERIAL_i16RxChar(uint8 u8SerialPort)
{
    int16 i16Result = -1;

    if(!QUEUE_bEmpty(asSerialPorts[u8SerialPort].sRxQueue)) {
        i16Result = (int16)QUEUE_u8RemoveItem(asSerialPorts[u8SerialPort].sRxQueue,TRUE);

        if (QUEUE_u16Count(asSerialPorts[u8SerialPort].sRxQueue) == asSerialPorts[u8SerialPort].u16AHI_UART_RTS_LOW) {
            UART_vRtsStartFlow(u8SerialPort); /* [I SP001222_P1 286]*/
        }
    }

    return i16Result;
}

/** @ingroup grp_Serial
 *
 * Get a NULL terminated string from the RX queue.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 * @param pu8RxBuffer Pointer to buffer into which received string is written.
 * @param u32BuffSize Size of buffer into which string is to be written
 * @return 32bit Number of characters in received string
 */
PUBLIC uint32 SERIAL_u32RxString(uint8 u8SerialPort, uint8 *pu8RxBuffer, uint32 u32BuffSize)
{
	int16 i16RxChar = 0;
	uint32 u32CharCount = 0;

	while ((uint8)i16RxChar != 0x0D)
	{
		while((i16RxChar = SERIAL_i16RxChar(u8SerialPort)) < 0);

		if (u32CharCount < u32BuffSize)
		{
            pu8RxBuffer[u32CharCount++] = (uint8)i16RxChar;
		}
		else
		{
            break;
		}
	}
	return(u32CharCount);
}

/** @ingroup grp_Serial
 * Returns the status of the serial RX queue.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 * @return TRUE if the queue is empty, FALSE otherwise.
 */
PUBLIC bool_t SERIAL_bRxQueueEmpty(uint8 u8SerialPort)
{
    return QUEUE_bEmpty(asSerialPorts[u8SerialPort].sRxQueue);
}

/** @ingroup grp_Serial
 * Returns the status of the serial RX queue.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 * @return TRUE if the queue is empty, FALSE otherwise.
 */
PUBLIC bool_t SERIAL_bTxQueueEmpty(uint8 u8SerialPort)
{
    return QUEUE_bEmpty(asSerialPorts[u8SerialPort].sTxQueue);
}

/** @ingroup grp_Serial
 * Transmitter ready callback.  Called from the UART handler to signal that
 * another byte can be loaded into the TX FIFO.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 */
PUBLIC void SERIAL_vTxReady(uint8 u8SerialPort)
{
    if (!QUEUE_bEmpty(asSerialPorts[u8SerialPort].sTxQueue)) {
        UART_vTxChar(u8SerialPort,QUEUE_u8RemoveItem(asSerialPorts[u8SerialPort].sTxQueue,FALSE));
    }
}

/** @ingroup grp_Serial
 * Blocking call to flush all data in the serial TX buffer.  When the function
 * returns all the data will be transmitted.
 *
 * @note Serial TX interrupts must be locked before calling this function
 *       - added UART_vSetTxInterrupt() to lock Tx empty INT. (MW)
 *
 * @param u8Uart Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 */
PUBLIC void SERIAL_vFlush(uint8 u8Uart)
{
    UART_vSetTxInterrupt(u8Uart, FALSE); // DISALBE TX EMPTY INT

    /* Serial data is buffered. Since we are in interrupt context.
     * the UART TX ints will not be services so we need to empty the
     * queue before going to sleep.
     */
    while(SERIAL_bTxQueueEmpty(u8Uart) == FALSE)
    {
        while ( !(u8AHI_UartReadLineStatus(u8Uart) & E_AHI_UART_LS_THRE ));
        SERIAL_vTxReady(u8Uart);
    }

    /* Now hang around waiting shift register to empty */
    while ( !(u8AHI_UartReadLineStatus(u8Uart) & E_AHI_UART_LS_TEMT) );

    UART_vSetTxInterrupt(u8Uart, TRUE); // RELEASE TX INT.

    _SERIAL_bmTxIntEnabled = 0; // just in case, reset the TxInt enabled flag.
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/**
 * Receive character callback.  Called from the UART handler when a character is received.
 *
 * @param u8SerialPort Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1.
 * @param u8Char Byte to queue
 */
PRIVATE void vRxChar(uint8 u8SerialPort,uint8 u8Char)
{
    QUEUE_vAddItem(asSerialPorts[u8SerialPort].sRxQueue, u8Char,FALSE);

    if (QUEUE_u16Count(asSerialPorts[u8SerialPort].sRxQueue) == asSerialPorts[u8SerialPort].u16AHI_UART_RTS_HIGH) {
        UART_vRtsStopFlow(u8SerialPort); /* [I SP001222_P1 285]*/
    }
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
