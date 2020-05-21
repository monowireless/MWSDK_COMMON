/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*! @file  ToCoNet_mod_Channel_Mgr ToCoNet.h
    @author seigo13
    @defgroup grp_ToCoNet_mod_Channel_Mgr ToCoNet モジュール - チャネルマネージャ
      チャネル遷移を自動化する構造体
 */

#ifndef TOCONET_MOD_CHANNEL_MGR_H_
#define TOCONET_MOD_CHANNEL_MGR_H_

#include <jendefs.h>

#define TOCONET_MOD_CHANNEL_MGR_DEFAULT_CYCLE_ms 80

/** @ingroup grp_ToCoNet_mod_Channel_Mgr
 * チャネルマネージャ設定構造体
 */
typedef struct {
	uint16 u16CycleDur_ms; //!< チャネル変更周期[ms]
} tsToCoNet_ChMgr_Config;

/** @ingroup grp_ToCoNet_mod_Channel_Mgr
 * チャネルマネージャ管理構造体
 */
typedef struct {
	tsToCoNet_ChMgr_Config sConf;
	uint32 u32ChMaskSave; //!< チャネルリスト作成時のチャネルリスト
	uint8 au8ChannelList[4]; //!< チャネルリスト(0で終端する)
	uint8 u8ChIdx; //!< 現在のチャネルリストのインデックス
} tsToCoNet_ChMgr_Context;

#endif /* TOCONET_MOD_CHANNEL_MGR_H_ */
