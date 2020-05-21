/****************************************************************************
 *
 * MODULE:            Utilities/appZpsBeaconHandler.h
 *
*/
/****************************************************************************
*
* This software is owned by NXP B.V. and/or its supplier and is protected
* under applicable copyright laws. All rights are reserved. We grant You,
* and any third parties, a license to use this software solely and
* exclusively on NXP products [NXP Microcontrollers such as JN5148, JN5142, JN5139].
* You, and any third parties must reproduce the copyright and warranty notice
* and any other legend of ownership on each copy or partial copy of the
* software.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.

* Copyright NXP B.V. 2012. All rights reserved
*
***************************************************************************/

#ifndef _appZpsBeaconHandler_h_
#define _appZpsBeaconHandler_h_

#ifdef __cplusplus
extern "C" {
#endif

/***********************/
/**** INCLUDE FILES ****/
/***********************/

#include "jendefs.h"
#include "mac_sap.h"
#include "zps_apl_af.h"

/************************/
/**** MACROS/DEFINES ****/
/************************/

#define BF_BITMAP_BLACKLIST        0x1
#define BF_BITMAP_WHITELIST        0x2
#define BF_BITMAP_LQI              0x4
#define BF_BITMAP_CAP_ENDDEVICE    0x8
#define BF_BITMAP_CAP_ROUTER       0x10
#define BF_BITMAP_PERMIT_JOIN      0x20

#define BF_USED_BITMASK          0x3F


#define BF_BCN_PL_PROTOCOL_ID(x)       ((x)[0])
#define BF_BCN_PL_STACK_PROFILE(x)     ((x)[1] & 0xf)
#define BF_PL_PROTOCOL_VER(x)      ((x)[1] >> 4)
#define BF_BCN_PL_ZR_CAPACITY(x)       (((x)[2] >> 2) & 0x1)
#define BF_BCN_PL_ZED_CAPACITY(x)      (((x)[2] >> 7) & 0x1)
#define BF_BCN_PL_EXT_PAN_ID_PTR(x)    &((x)[3])
#define BF_ASSOC_PERMIT_BIT             15
#define BF_ASSOC_PERMIT_MASK            ((uint16)(0x1 << BF_ASSOC_PERMIT_BIT))
#define BF_GET_ASSOC_PERMIT(x)          (((x) & BF_ASSOC_PERMIT_MASK) >> BF_ASSOC_PERMIT_BIT)

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/

/* Bit Map */
/*|    7      |    6     |      5      |    4        |        3      |      2    |      1        |      0        |*/
/*|    x      |    x     |  PermitJoin |  cap Router | cap EndDevice |    LQI    |  White List   |   BlackList   |*/
typedef struct
{
	uint64    *pu64ExtendPanIdList;
	uint8    u8ListSize;
	uint8    u8Lqi;
	uint8    u8FilterMap;
} tsBeaconFilterType;

/****************************/
/**** EXPORTED VARIABLES ****/
/****************************/

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/
PUBLIC void ZPS_bAppAddBeaconFilter(tsBeaconFilterType *psAppBeaconStruct);
PUBLIC void ZPS_bAppRemoveBeaconFilter(void);

#endif /* _appZpsBeaconHandler_h_ */

/* End of file $RCSfile: appZpsBeaconHandler.h,v $ *******************************************/

