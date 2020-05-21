/****************************************************************************
 * $Rev:: 1946              $: Revision of last commit
 * $Author:: snich          $: Author of last commit
 * $Date:: 2009-02-03 09:51#$: Date of last commit
 * $HeadURL: http://svn/apps/Reference_Designs/JN-RD-6009-Active-RFID/Modules/BoardSoftware/Tags/Release_1v3/802.15.4_Toolbox/Serial/uart.c $
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
 * Hardware abstraction layer for UART peripheral
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "AppHardwareApi.h"
#include "uart.h"

//#include "config.h"
#include "utils.h"


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define MAX_UARTS 2

#ifdef JN514x // different addr in JN514x
#define UART0_START_ADR      0x02003000UL
#define UART1_START_ADR      0x02004000UL
#else
#define UART0_START_ADR      0x30000000UL
#define UART1_START_ADR      0x40000000UL
#endif
#define UART_LCR_OFFSET      0x0C
#define UART_DLM_OFFSET      0x04

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/**
 * UART descriptor.
 */
typedef struct
{
    void (*pfRxFn)(uint8,uint8); /** Pointer to RX data callback function  */
    void (*pfTxRdy)(uint8); /** Pointer to TX Ready callback function */
    uint32 u32StartAddress; /** UART Register start address */
    uint8 u8RX_FIFO_LEVEL; /** RX FIFO level that will cause an interrupt */
}tsUART;
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void UART_vHandleUartInterrupt(uint32 u32Device, uint32 u32ItemBitmap);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/** List of UART descriptors */
PRIVATE tsUART asUARTS[MAX_UARTS];
PRIVATE tsUartOpt sOpt[MAX_UARTS];

#define BUF_LEN 16
PRIVATE uint8 au8TxBuf[2][BUF_LEN];
PRIVATE uint8 au8RxBuf[2][BUF_LEN];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/**
 * Initialise the specified UART. The UART is interrupt driven.
 *
 * This must be called before sending or receiving any data.
 *
 * @param u8UARTNum Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 * @param pfRxFn Pointer to callback to use when a byte has been received
 * @param pfTxRdy Pointer to callback to use to indicate another character can be transmitted
 * @param u32BaudRate Serial baud rate
 * @param u8RX_FIFO_LEVEL RX FIFO level to use to trigger a RX interrupt.
 */
PUBLIC void UART_vInit(uint8 u8UARTNum,
                       void (*pfRxFn)(uint8,uint8),
                       void (*pfTxRdy)(uint8),
                       uint32 u32BaudRate,
                       uint8 u8RX_FIFO_LEVEL,
                       tsUartOpt *pOpt)
{
	/* Parity, Stopbit */
	if (pOpt == NULL) {
		/* default is Parity:none, StopBit:1 */
		sOpt[u8UARTNum].bHwFlowEnabled = FALSE;
		sOpt[u8UARTNum].bParityEnabled = FALSE;
		sOpt[u8UARTNum].u8ParityType = 0;
		sOpt[u8UARTNum].u8WordLen = 8;
		sOpt[u8UARTNum].u8StopBit = E_AHI_UART_1_STOP_BIT;
	} else {
		sOpt[u8UARTNum] = *pOpt;
	}

	// 8bit がデフォルト
	if (sOpt[u8UARTNum].u8WordLen < 5 || sOpt[u8UARTNum].u8WordLen > 8) {
		sOpt[u8UARTNum].u8WordLen = 8;
	}

    asUARTS[u8UARTNum].pfRxFn = pfRxFn;
    asUARTS[u8UARTNum].pfTxRdy = pfTxRdy;
    asUARTS[u8UARTNum].u8RX_FIFO_LEVEL = u8RX_FIFO_LEVEL;

    /* if CTS/RTS is not necessary, release DIO here */
    vAHI_UartSetRTSCTS(u8UARTNum, sOpt[u8UARTNum].bHwFlowEnabled);

    /* Enable UART */
#ifdef JN514x
    vAHI_UartEnable(u8UARTNum);
#else
    bAHI_UartEnable( u8UARTNum, au8TxBuf[u8UARTNum], BUF_LEN, au8RxBuf[u8UARTNum], BUF_LEN );
#endif

    /* ここはおまじない */
    vAHI_UartReset(u8UARTNum, TRUE, TRUE);
    vAHI_UartReset(u8UARTNum, FALSE, FALSE);

    /* 割り込みハンドラの登録 */
    if (u8UARTNum == E_AHI_UART_0){
        asUARTS[u8UARTNum].u32StartAddress = UART0_START_ADR;
        vAHI_Uart0RegisterCallback(UART_vHandleUartInterrupt);
    }
    else{
        asUARTS[u8UARTNum].u32StartAddress = UART1_START_ADR;
        vAHI_Uart1RegisterCallback(UART_vHandleUartInterrupt);
    }

    /* Set the clock divisor register to give required buad, this has to be done
       directly as the normal routines (in ROM) do not support all baud rates */
    if (u32BaudRate <= 0xFF) {
    	// 255以下を指定するとディバイザの設定とする (1Mbps / ディバイザ)
    	vAHI_UartSetBaudDivisor(u8UARTNum,u32BaudRate);
    } else
    if (u32BaudRate &0x80000000) {
    	// MSB を立てる(0x80XXYYYY)と XX が CBP, YYYY がディバイザとなる
    	// より高精度なボーレートの設定が可能
    	uint8 u8cbp = (u32BaudRate >> 16) & 0xFF;
    	// set by divisor and cbp
    	u32BaudRate &= 0x7FFFFFF;
    	vAHI_UartSetBaudDivisor(u8UARTNum, u32BaudRate & 0xFFFF);
    	if (u8cbp) {
    		vAHI_UartSetClocksPerBit(u8UARTNum, u8cbp);
    	}
    } else {
    	// u32BaudRate は bps として、適切な設定を行う。
    	// 57600,115200,230400 はディバイザのみでは誤差が大きいため CBP も設定。
    	uint32 u32Remainder;
    	uint16 u16Divisor;
    	uint8 u8cbp;

		switch(u32BaudRate) {
		case 57600:
			u16Divisor = 23;
			u8cbp = 11;
			break;
		case 115200:
			u16Divisor = 10;
			u8cbp = 13;
			break;
		case 230400:
			u16Divisor = 5;
			u8cbp = 13;
			break;
		default:
			// ボーレートから最も近いディバイザの値を計算する
			/* Calculate divisor for baud rate = 16MHz / (16 x baud rate) */
			u16Divisor   = (uint16)(16000000UL / (16UL * u32BaudRate));
			/* Correct for rounding errors */
			u32Remainder = (uint32)(16000000UL % (16UL * u32BaudRate));
			if (u32Remainder >= ((16UL * u32BaudRate) / 2)) u16Divisor += 1;

			/* set CBP as 0, no use */
			u8cbp = 0;
			break;
		}

		// ボーレート
    	vAHI_UartSetBaudDivisor(u8UARTNum, u16Divisor);
    	if (u8cbp) {
    		// CBP の設定
    		vAHI_UartSetClocksPerBit(u8UARTNum, u8cbp);
    	}
    }

    // パリティ、ストップビット、ビット数
    vAHI_UartSetControl(u8UARTNum,
       		sOpt[u8UARTNum].u8ParityType,
			sOpt[u8UARTNum].bParityEnabled,
			sOpt[u8UARTNum].u8WordLen - 5,
			sOpt[u8UARTNum].u8StopBit,
    		FALSE);

    // 割り込み開始
    vAHI_UartSetInterrupt(u8UARTNum, FALSE, FALSE, FALSE, TRUE, asUARTS[u8UARTNum].u8RX_FIFO_LEVEL);    // No TX ints!
}

/**
 * Sets the serial baud rate by calculating the clock divisor
 * necessary to achieve the baud rate.
 *
 * @note Since the clock source is from the system 16MHz clock
 * there will be an error on some baud rates.
 *
 * @param u8UARTNum Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 * @param u32BaudRate Baud rate to use.
 *
 */
#if 0
PUBLIC void UART_vSetBaudRate(uint8 u8UARTNum,uint32 u32BaudRate)
{
    uint8 *pu8Reg;
    uint8  u8TempLcr;
    uint16 u16Divisor;
    uint32 u32Remainder;

    /* Put UART into clock divisor setting mode */
    pu8Reg    = (uint8 *)(asUARTS[u8UARTNum].u32StartAddress + UART_LCR_OFFSET);
    u8TempLcr = *pu8Reg;
    *pu8Reg   = u8TempLcr | 0x80;

    /* Write to divisor registers:
       Divisor register = 16MHz / (16 x baud rate) */
    u16Divisor = (uint16)(16000000UL / (16UL * u32BaudRate));

    /* Correct for rounding errors */
    u32Remainder = (uint32)(16000000UL % (16UL * u32BaudRate));

    if (u32Remainder >= ((16UL * u32BaudRate) / 2))
    {
        u16Divisor += 1;
    }

    pu8Reg  = (uint8 *)asUARTS[u8UARTNum].u32StartAddress;
    *pu8Reg = (uint8)(u16Divisor & 0xFF);
    pu8Reg  = (uint8 *)(asUARTS[u8UARTNum].u32StartAddress + UART_DLM_OFFSET);
    *pu8Reg = (uint8)(u16Divisor >> 8);

    /* Put back into normal mode */
    pu8Reg    = (uint8 *)(asUARTS[u8UARTNum].u32StartAddress + UART_LCR_OFFSET);
    u8TempLcr = *pu8Reg;
    *pu8Reg   = u8TempLcr & 0x7F;
}
#endif


/**
 * UART interrupt service routine.  Handles RX and TX for both
 * interrupts.
 *
 * @param u32Device Device ID that generated the interrupt
 * @param u32ItemBitmap Interrupt type information.
 */
PRIVATE void UART_vHandleUartInterrupt(uint32 u32Device, uint32 u32ItemBitmap)
{
    uint8 u8UARTNum;

    if (u32Device == E_AHI_DEVICE_UART0){
        u8UARTNum = 0;
    }
    else if (u32Device == E_AHI_DEVICE_UART1){
        u8UARTNum = 1;
    }
    else{
        return;
    }

#ifdef UART_ERROR_DEBUG
    uint8 u8lineStatus = u8AHI_UartReadLineStatus(u8UARTNum);
    if ((u8lineStatus &  E_AHI_UART_LS_OE ) > 0)
    {
        vPrintf("u8lineStatus:%x\r\n",u8lineStatus);
    }
#endif

    if ((u32ItemBitmap & 0x000000FF) == E_AHI_UART_INT_RXDATA ||
            (u32ItemBitmap & 0x000000FF) ==  E_AHI_UART_INT_TIMEOUT)
    {
        if (NULL != asUARTS[u8UARTNum].pfRxFn)
        {
            while(u8AHI_UartReadLineStatus(u8UARTNum) & E_AHI_UART_LS_DR)
            {
                asUARTS[u8UARTNum].pfRxFn(u8UARTNum,u8AHI_UartReadData(u8UARTNum));
            }
        }
    }
    else if (u32ItemBitmap == E_AHI_UART_INT_TX)
    {
        if (NULL != asUARTS[u8UARTNum].pfTxRdy)
        {
            // Fill up uart again.
            uint8 index;
            //for(index=0;index<16;index++)
            for(index=0;index<BUF_LEN-1;index++)
            {
                asUARTS[u8UARTNum].pfTxRdy(u8UARTNum);
            }
        }
    }
}


/**
 * Set UART RS-232 RTS line high to stop any further data coming in
 *
 * @param u8UARTNum Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 */
PUBLIC void UART_vRtsStopFlow(uint8 u8UARTNum)
{
    vAHI_UartSetControl(u8UARTNum,
    		sOpt[u8UARTNum].u8ParityType,
    		sOpt[u8UARTNum].bParityEnabled,
    		sOpt[u8UARTNum].u8WordLen - 5,
    		sOpt[u8UARTNum].u8StopBit,
    		E_AHI_UART_RTS_HIGH);
}


/**
 *  Set UART RS-232 RTS line low to allow further data
 *
 * @param u8UARTNum Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 */
PUBLIC void UART_vRtsStartFlow(uint8 u8UARTNum)
{
    vAHI_UartSetControl(u8UARTNum,
    		sOpt[u8UARTNum].u8ParityType,
    		sOpt[u8UARTNum].bParityEnabled,
    		sOpt[u8UARTNum].u8WordLen - 5,
    		sOpt[u8UARTNum].u8StopBit,
    		E_AHI_UART_RTS_LOW);
}

/**
 *  Put a byte into the UART TX FIFO.
 *
 * @param u8UARTNum Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 * @param u8Char Vyte to send
 */
PUBLIC void UART_vTxChar(uint8 u8UARTNum,uint8 u8Char)
{
    vAHI_UartWriteData(u8UARTNum, u8Char);
}

/**
 * Determine if the TX FIFO is empty.
 * @param u8UARTNum Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 * @return TRUE if empty, FALSE otherwise.
 */
PUBLIC bool_t UART_bTxReady(uint8 u8UARTNum)
{
    return u8AHI_UartReadLineStatus(u8UARTNum) & E_AHI_UART_LS_THRE;
}

/**
 *  Enable / disable the tx interrupt
 *
 * @param u8UARTNum Serial port to use, either E_AHI_UART_0 or E_AHI_UART_1
 * @param bState Set to TRUE to enable, FALSE to disable.
 */
PUBLIC void UART_vSetTxInterrupt(uint8 u8UARTNum,bool_t bState)
{
    vAHI_UartSetInterrupt(u8UARTNum, FALSE, FALSE, bState, TRUE, asUARTS[u8UARTNum].u8RX_FIFO_LEVEL);
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
