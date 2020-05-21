/****************************************************************************
 * $Rev:: 1946              $: Revision of last commit
 * $Author:: snich          $: Author of last commit
 * $Date:: 2009-02-03 09:51#$: Date of last commit
 * $HeadURL: http://svn/apps/Reference_Designs/JN-RD-6009-Active-RFID/Modules/BoardSoftware/Tags/Release_1v3/802.15.4_Toolbox/utils/ByteQueue.c $
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
 * Generic FIFO byte queue.
 *
 * @defgroup grp_ByteQueue Generic Byte Queue
 *
 * Generic FIFO byte queue.
 * Wrapper MACROS are provided in ByteQueue.h for queue creating and use.
*/

/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>

#include "Interrupt.h"
#include "ByteQueue.h"

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
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/** @ingroup grp_ByteQueue
 *
 * Initialises the queue.
 *
 * @param psQueue Pointer to the queue instance
 * @param pu8Buffer Pointer to the buffer to store the queue data
 * @param u16Size Size of the buffer pointed to by pu8Buffer
 */
PUBLIC void  QUEUE__vInit(tsQueue *psQueue,uint8 *pu8Buffer,uint16 u16Size)
{
    psQueue->u16Size = u16Size;
    psQueue->u16Head = 0;
    psQueue->u16Tail = 0;
    psQueue->u16Count = 0;
    psQueue->u8Buff = pu8Buffer;
}

/** @ingroup grp_ByteQueue
 *
 * Put a byte onto the queue.  The user has the option to protect the
 * queue access, so if the queue is been used from interrupt context and
 * also the main loop no conflicts will occur.
 *
 * @param psQueue Pointer to the Queue instance
 * @param u8Item Byte to add to the queue
 * @param bProtected Set to TRUE to lock interrupts during the access.
 *
 */
PUBLIC void QUEUE__vAddItem(tsQueue *psQueue, uint8 u8Item,bool_t bProtected)
{
    if (psQueue->u16Count < psQueue->u16Size)
    {
        /* Space available in buffer so add data */
        psQueue->u8Buff[psQueue->u16Head++] = u8Item;

        /* read, modify write operation must be atomic */
        if (bProtected == TRUE)
        {
        	MICRO_INT_STORAGE;
        	MICRO_INT_ENABLE_ONLY(0);
			if (psQueue->u16Head == psQueue->u16Size) {
				psQueue->u16Head = 0;
			}
			psQueue->u16Count++;
			MICRO_INT_RESTORE_STATE();
        }
        else
        {
			if (psQueue->u16Head == psQueue->u16Size) {
				psQueue->u16Head = 0;
			}
			psQueue->u16Count++;
        }
    }
}

/** @ingroup grp_ByteQueue
 *
 * Get a byte from the queue.  The user has the option to protect the
 * queue access, so if the queue is been used from interrupt context and
 * also the main loop no conflicts will occur.
 *
 * The user should check if the queue has data before calling
 *
 * @param psQueue Pointer to the Queue instance
 * @param bProtected Set to TRUE to lock interrupts during the access.
 *
 * @return byte pulled from the queue.
 */
PUBLIC uint8 QUEUE__u8RemoveItem(tsQueue *psQueue,bool_t bProtected)
{
	uint8 u8Item = 0;

    if (psQueue->u16Count > 0)
    {
        /* Data available in buffer so remove data */
        u8Item = psQueue->u8Buff[psQueue->u16Tail++];

        if (bProtected == TRUE)
        {
        	MICRO_INT_STORAGE;
        	MICRO_INT_ENABLE_ONLY(0);
        	if (psQueue->u16Tail == psQueue->u16Size) {
				psQueue->u16Tail = 0;
			}
            psQueue->u16Count--;
			MICRO_INT_RESTORE_STATE();
        } else {
        	if (psQueue->u16Tail == psQueue->u16Size) {
				psQueue->u16Tail = 0;
			}
			psQueue->u16Count--;
        }
    }

    return u8Item;
}

/** @ingroup grp_ByteQueue
 *
 * Check if the specified queue is empty.
 *
 * @param psQueue Pointer to the Queue instance
 * @return TRUE if queue is empty, FALSE otherwise.
 */
PUBLIC bool_t QUEUE__bEmpty(tsQueue *psQueue)
{
    return psQueue->u16Count == 0;
}

/** @ingroup grp_ByteQueue
 *
 * Check if the specified queue is full.
 *
 * @param psQueue Pointer to the Queue instance
 * @return TRUE if queue is full, FALSE otherwise.
 */
PUBLIC bool_t QUEUE__bFull(tsQueue *psQueue)
{
    return psQueue->u16Count == psQueue->u16Size;
}

/** @ingroup grp_ByteQueue
 * Remove all items in the queue.
 * @param psQueue Pointer to the Queue instance
 */
PUBLIC void QUEUE__vFlush(tsQueue *psQueue)
{
	MICRO_INT_STORAGE;
	MICRO_INT_ENABLE_ONLY(0);
    psQueue->u16Count = psQueue->u16Head = psQueue->u16Tail = 0;
    MICRO_INT_RESTORE_STATE();
}

/** @ingroup grp_ByteQueue
 * Get the number of items in the queue
 *
 * @param psQueue Pointer to the Queue instance
 */
PUBLIC uint16 QUEUE__u16Count(tsQueue *psQueue)
{
    return psQueue->u16Count;
}


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
