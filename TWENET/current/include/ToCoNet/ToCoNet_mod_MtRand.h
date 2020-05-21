/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * ToCoNet_mod_MtRand.h
 *
 *  Created on: 2013/01/18
 *      Author: seigo13
 */

#ifndef TOCONET_MOD_MTRAND_H_
#define TOCONET_MOD_MTRAND_H_

#include <jendefs.h>

#ifdef __cplusplus
extern "C" {
#endif // C++

/**
 * @defgroup grp_ToCoNet_MtRand ToCoNet モジュール - 乱数生成モジュール
 * メルセンヌ ツイスター法による乱数生成を ToCoNet への組み込みとして使用する。
 * - ToCoNet 組み込みとして使用せず独立して使用する場合は、Utils の randMT.c を使用すること。
 * - ※ 使用する場合は BSD License に基づく表記が必要になります。
 */

/** @ingroup grp_ToCoNet_MtRand
 * MT RAND の処理の呼び出し用の関数ポインタ
 */
typedef struct {
	void (*vInit)(uint32 u32Seed); //!< 種の初期化
	uint32 (*u32GetRand)(void); //!< 乱数の生成
} tsToCoNet_MtRand_Context;


#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_MOD_NBSCAN_SLAVE_H_ */
