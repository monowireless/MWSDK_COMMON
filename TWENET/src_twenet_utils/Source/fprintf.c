/****************************************************************************
 * $Rev:: 1946              $: Revision of last commit
 * $Author:: snich          $: Author of last commit
 * $Date:: 2009-02-03 09:51#$: Date of last commit
 * $HeadURL: http://svn/apps/Reference_Designs/JN-RD-6009-Active-RFID/Modules/BoardSoftware/Tags/Release_1v3/802.15.4_Toolbox/utils/fprintf.c $
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
 * fprintf string formatting support
 *
 * @defgroup grp_fprintf string formatting utility
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <stdarg.h>
#include "fprintf.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define PAD_RIGHT       1
#define PAD_ZERO        2
#define PRINT_BUF_LEN   12

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static uint32 u32StringLen(const char *pcString);
static void vPrintString(tsFILE *psStream, const char *pcString, uint32 u32Width, uint32 u32Pad);
static void vPrintInteger(tsFILE *psStream, int32 i32Integer, uint32 u32Base, bool_t bSigned, uint32 u32Width, uint32 u32Pad, int32 i32LetBase);
static void vPrintBitmap(tsFILE *psStream, uint32 u32bm, uint32 u32Width, uint32 u32Pad);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/** @ingroup grp_fprintf
 * Format and output a string to the stream supplied.
 *
 * @param psStream Stream to which string should be output.
 * @param pcFormat Print format.
 * @param ... Optional arguments depending on print format.
 */
PUBLIC void vfPrintf(tsFILE *psStream, const char *pcFormat, ...)
{
    va_list arguments;
    va_start(arguments, pcFormat);

	uint32 u32Width, u32Pad;

	for (; *pcFormat != 0; ++pcFormat)
	{
		if (*pcFormat == '%')
		{
			pcFormat++;
			u32Width = u32Pad = 0;

			if (*pcFormat == '\0')
			{
                break;
			}

			if (*pcFormat == '%')
			{
                goto out;
			}

			if (*pcFormat == '-')
			{
				pcFormat++;
				u32Pad = PAD_RIGHT;
			}

			while (*pcFormat == '0')
			{
				pcFormat++;
				u32Pad |= PAD_ZERO;
			}

			while(*pcFormat >= '0' && *pcFormat <= '9')
			{
				u32Width *= 10;
				u32Width += *pcFormat - '0';
				pcFormat++;
			}

			if( *pcFormat == 's' )
			{
				char *s = (char *)va_arg(arguments, uint32);
				vPrintString(psStream, s?s:"(null)", u32Width, u32Pad);
				continue;
			}
			if( *pcFormat == 'd' )
			{
				vPrintInteger(psStream, va_arg(arguments, uint32), 10, 1, u32Width, u32Pad, 'a');
				continue;
			}
			if( *pcFormat == 'x' )
			{
				vPrintInteger(psStream, va_arg(arguments, uint32), 16, 0, u32Width, u32Pad, 'a');
				continue;
			}
			if( *pcFormat == 'X' )
			{
				vPrintInteger(psStream, va_arg(arguments, uint32), 16, 0, u32Width, u32Pad, 'A');
				continue;
			}
			if( *pcFormat == 'b' )
			{
				vPrintBitmap(psStream, va_arg(arguments, uint32), u32Width, u32Pad);
				continue;
			}
			if( *pcFormat == 'u' )
			{
				vPrintInteger(psStream, va_arg(arguments, uint32), 10, 0, u32Width, u32Pad, 'a');
				continue;
			}
			if( *pcFormat == 'c' )
			{
				char acCharStr[2];

				acCharStr[0] = va_arg(arguments, uint32);
				acCharStr[1] = '\0';
				vPrintString(psStream, acCharStr, u32Width, u32Pad);
				continue;
			}
		}
		else
		{
		out:
			(*psStream->bPutChar)(psStream->u8Device, *pcFormat);
		}
	}
	// (*psStream->bPutChar)(psStream->u8Device, '\0'); // remove it
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/**  @ingroup grp_fprintf
 * Prints an integer to the stream output.
 *
 * @param psStream Output Stream
 * @param i32Integer integer number to print
 * @param u32Base base
 * @param bSigned signed or unsigned
 * @param u32Width format width
 * @param u32Pad padding
 * @param i32LetBase let base
 */
static void vPrintInteger(tsFILE *psStream, int32 i32Integer, uint32 u32Base, bool_t bSigned, uint32 u32Width, uint32 u32Pad, int32 i32LetBase)
{
	char *pcString;
	char acCharBuff[PRINT_BUF_LEN];
    uint32 u32Integer = i32Integer;

	if (i32Integer == 0)
	{
		acCharBuff[0] = '0';
		acCharBuff[1] = '\0';
		vPrintString(psStream, acCharBuff, u32Width, u32Pad);
	}
	else
	{
        bool_t bNegative = FALSE;

        if ((bSigned) && (u32Base == 10) && (i32Integer < 0))
        {
            bNegative = TRUE;
            u32Integer = -i32Integer;
        }

        pcString = acCharBuff + PRINT_BUF_LEN - 1;
        *pcString = '\0';

        while (u32Integer)
        {
            int32 i32Temp = u32Integer % u32Base;

            if(i32Temp >= 10)
            {
                i32Temp += i32LetBase - '0' - 10;
            }
            *--pcString = i32Temp + '0';
            u32Integer /= u32Base;
        }

        if (bNegative)
        {
            if( u32Width && (u32Pad & PAD_ZERO) )
            {
                (*psStream->bPutChar)(psStream->u8Device, '-');
                u32Width--;
            }
            else
            {
                *--pcString = '-';
            }
        }
        vPrintString(psStream, pcString, u32Width, u32Pad);
	}
}

/** @ingroup grp_fprintf
 *  * Print binary bitmap.
 *
 * @param psStream
 * @param u32bm
 * @param u32Width
 * @param u32Pad
 */
static void vPrintBitmap(tsFILE *psStream, uint32 u32bm, uint32 u32Width, uint32 u32Pad) {
	if (u32Width >= 32) {
		u32Width = 32;
	}

	// find Most Significant Bit.
	if (u32Width == 0) {
		u32Width = 32;

		if (u32bm) {
			while(!((1UL << (u32Width - 1)) & u32bm)) {
				u32Width--;
			}
		} else {
			u32Width = 1;
		}
	}

	uint32 mask = 1UL << (u32Width - 1);
	bool_t bFirstBit = FALSE;

	while (mask) {
		uint8 c;

		if (u32bm & mask) {
			c = '1';
			bFirstBit = TRUE;
		} else {
			c = ((u32Pad & PAD_ZERO) || bFirstBit) ? '0' : ' ';
		}

		vPutChar(psStream, c);
		mask >>= 1;
	}
}

/** @ingroup grp_fprintf
 * Prints a string to the stream output including any required padding.
 *
 * @param psStream Stream on which string is to be output.
 * @param pcString String to be output.
 * @param u32Width of output string.
 * @param u32Pad Type of padding.
 */
static void vPrintString(tsFILE *psStream, const char *pcString, uint32 u32Width, uint32 u32Pad)
{
	char cPadChar;

	/* Set the character to be output when padding is required */
	if (u32Pad & PAD_ZERO)
	{
        cPadChar = '0';
	}
	else
	{
        cPadChar = ' ';
	}

    /* Calculate the number of padding characters actually required */
	if (u32Width > 0)
	{
		uint32 u32Len = u32StringLen(pcString);

		if (u32Len >= u32Width)
		{
            u32Width = 0;
		}
		else
		{
            u32Width -= u32Len;
		}
	}

	/* Pad left, if required */
	if (!(u32Pad & PAD_RIGHT))
	{
		while (u32Width > 0)
		{
			(*psStream->bPutChar)(psStream->u8Device, cPadChar);
			u32Width--;
		}
	}

    /* Output data */
    while(*pcString != '\0')
	{
		(*psStream->bPutChar)(psStream->u8Device, *pcString);
		pcString++;
	}

	/* Pad right, if required */
	while (u32Width > 0)
	{
		(*psStream->bPutChar)(psStream->u8Device, cPadChar);
		u32Width--;
	}
}

/** @ingroup grp_fprintf
 * Calculate the number of characters contained in a NULL terminated string
 *
 * @param pcString Pointer to string to be measured.
 * @return 32bit Length of string in characters.
 */
static uint32 u32StringLen(const char *pcString)
{
    uint32 u32Len = 0;

    while (*pcString != '\0')
    {
        u32Len++;
        pcString++;
    }
    return u32Len;
}



/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
