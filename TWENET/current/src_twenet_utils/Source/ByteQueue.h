/****************************************************************************
 * $Rev:: 1946              $: Revision of last commit
 * $Author:: snich          $: Author of last commit
 * $Date:: 2009-02-03 09:51#$: Date of last commit
 * $HeadURL: http://svn/apps/Reference_Designs/JN-RD-6009-Active-RFID/Modules/BoardSoftware/Tags/Release_1v3/802.15.4_Toolbox/utils/ByteQueue.h $
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
/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

#ifndef  BYTE_QUEUE_H_INCLUDED
#define  BYTE_QUEUE_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/** @ingroup grp_ByteQueue
 *
 * Declare a queue.  All the necessary structures will be declared with
 * this MACRO.
 *
 * @param n Queue instance name
 * @param s Queue size in bytes.
 */
#define QUEUE_DECLARE_Q(n, s) uint8 qData_##n[s]; tsQueue n = { s, 0, 0, 0, (uint8 *)&qData_##n};

/**
 * Get the address of the specified queue name
 * @param a Queue instance name
 * @returns &a
 */
#define QUEUE_REF(a) &(a)

/** @ingroup grp_ByteQueue
 *
 * Check if the specified queue is full.
 *
 * @param q Queue instance
 * @return TRUE if queue is full, FALSE otherwise.
 */
#define QUEUE_bFull(q)          QUEUE__bFull(QUEUE_REF(q))

/** @ingroup grp_ByteQueue
 *
 * Check if the specified queue is empty.
 *
 * @param q Queue instance
 * @return TRUE if queue is empty, FALSE otherwise.
 */
#define QUEUE_bEmpty(q)         QUEUE__bEmpty(QUEUE_REF(q))

/** @ingroup grp_ByteQueue
 *
 * Get a byte from the queue.  The user has the option to protect the
 * queue access, so if the queue is been used from interrupt context and
 * also the main loop no conflicts will occur.
 *
 * The user should check if the queue has data before calling
 *
 * @param q Queue instance
 * @param p Set to TRUE to lock interrupts during the access.
 *
 * @return byte pulled from the queue.
 */
#define QUEUE_u8RemoveItem(q,p)   QUEUE__u8RemoveItem(QUEUE_REF(q),(p))

/** @ingroup grp_ByteQueue
 *
 * Put a byte onto the queue.  The user has the option to protect the
 * queue access, so if the queue is been used from interrupt context and
 * also the main loop no conflicts will occur.
 *
 * @param q Queue instance
 * @param i Byte to add to the queue
 * @param p Set to TRUE to lock interrupts during the access.
 *
 */
#define QUEUE_vAddItem(q, i,p)    QUEUE__vAddItem(QUEUE_REF(q), (i),(p))

/** @ingroup grp_ByteQueue
 * Remove all items in the queue.
 * @param q Queue instance
 */
#define QUEUE_vFlush(q)         QUEUE__vFlush(QUEUE_REF(q))

/** @ingroup grp_ByteQueue
 * Get the number of items in the queue
 *
 * @param q Queue instance
 */
#define QUEUE_u16Count(q)       QUEUE__u16Count(QUEUE_REF(q))

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/**
 * Queue Descriptor
 */
typedef struct
{
    uint16 u16Size; /** Number of bytes the queue can store */
    uint16 u16Head; /** Index to the head of the queue where data will be added */
    uint16 u16Tail; /** Index to the tail of the queue where data will be retrieved */
    uint16 u16Count; /** Number of elements in the queue */
    uint8  *u8Buff;  /** Pointer to the queue buffer */
} tsQueue;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC bool_t 	QUEUE__bFull(tsQueue *psQueue);
PUBLIC bool_t 	QUEUE__bEmpty(tsQueue *psQueue);
PUBLIC uint8 	QUEUE__u8RemoveItem(tsQueue *psQueue,bool_t bProtected);
PUBLIC void   	QUEUE__vAddItem(tsQueue *psQueue, uint8 u8Item,bool_t bProtected);
PUBLIC void   	QUEUE__vFlush(tsQueue *psQueue);
PUBLIC uint16 	QUEUE__u16Count(tsQueue *psQueue);
PUBLIC void  	QUEUE__vInit(tsQueue *psQueue,uint8 *pu8Buffer,uint16 u16Size);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
