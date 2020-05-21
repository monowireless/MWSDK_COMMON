/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*! @file  ToCoNet_mod_Nwk_LayerTree.h
    @author seigo13
    @defgroup grp_ToCoNet_mod_Nwk_LayerTree ToCoNet モジュール - LayerTree ネットワーク
    @defgroup grp_ToCoNet_mod_Nwk_LayerTree_MiniNodes ToCoNet モジュール - LayerTree ネットワーク(送信専用ミニノード)
 */

#ifndef TOCONET_MOD_NWK_LAYERTREE_H_
#define TOCONET_MOD_NWK_LAYERTREE_H_

#include <jendefs.h>
#include "ToCoNet_mod_Nwk_Common.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * ネットワークの設定用の構造体
 */
typedef struct
{
	uint8 u8Layer; //!< レイヤー数 (1～63)
	uint8 u8Role; //!< 役割 (EndDevice/Router/Parent)
	uint8 u8Second_To_Rescan; //!< 未接続時の、再スキャンまでの秒数 (最小値は3)
	uint8 u8Second_To_Relocate; //!< 接続時の、親機確認のためのパケット間隔 (最小値は1)
	uint8 u8Second_To_Beacon; //!< 定期的なビーコン情報の送信間隔。旧い接続情報を持つ子機に対して情報を与える (最小値は1)
	uint8 u8Second_Beacon_Lost; //!< 定期的なビーコンがロスト状態になるまでの秒数 (0xFF,0x00は既定値120秒、最小はビーコン送信間隔の３倍程度を推奨）
	uint8 u8Ct_To_Relocate; //!< 再探索回数 (最小値は5)
	uint8 u8ScanDur_10ms; //!< スキャン時のチャネルあたりの待ち時間 (最小値は3)

	uint16 u16TxMinDelayDn_ms; //!< 下りパケット送信時の最大ディレー[ms] (最小値は30)
	uint16 u16TxMinDelayUp_ms; //!< 上りパケット送信時の最小ディレー[ms] (最小値は30)
	uint16 u16TxMaxDelayDn_ms; //!< 下りパケット送信時の最大ディレー[ms] (最小値は30)
	uint16 u16TxMaxDelayUp_ms; //!< 上りパケット送信時の最大ディレー[ms] (最小値は30)
	uint16 u16TxRetryDurDn_ms; //!< 下りパケット再送時の間隔 [ms]
	uint16 u16TxRetryDurUp_ms; //!< 上りパケット再送時の間隔 [ms]
	uint8 u8TxRetryCtUp; //!< 下りパケット再送回数 (0:def,1-7,0xFF=0回)
	uint8 u8TxRetryCtDn; //!< 上りパケット再送回数 (0:def,1-7,0xFF=0回)

	uint8 u8MaxSublayers; //!< サブレイヤの最大数 (1,2,0xFF:無効)
	uint8 u8LayerOptions; //!< 接続オプション (TOCONET_MOD_LAYERTREE_LAYEROPTMASK_* 参照)
	uint8 u8minLQI; //!< 接続時の LQI の閾値(これ以上ないと接続しない)

	uint8 u8ResumeOpt; //!< レジューム時のオプション (TOCONET_MOD_LAYERTREE_RESUMEOPT_* 参照)
	uint8 u8StartOpt; //!< 始動オプション (TOCONET_MOD_LAYERTREE_STARTOPT_FIXED_* 参照)
	uint32 u32AddrHigherLayer; //!< 指定ノードを上位に START する 0:デフォルト 0x8XXXXXXX:指定ノード

} tsToCoNet_NwkLyTr_Config;

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * ネットワーク状態管理構造体
 */
typedef struct {
	// base definition
	tsToCoNet_Nwk_Context_Base_Info sInfo; //!< 共通：ネットワーク基本情報
	tsToCoNet_Nwk_Context_Base_Method sMethod; //!< 共通：メソッド

	// LayerTree specific data follows.
	tsToCoNet_NwkLyTr_Config sConf; //!< 設定情報

	// Working variables
	uint32 u32AddrHigherLayer; //!< 親機のアドレス
	uint8 u8SessionId; //!< セッションID(親機との現在の接続を確認するためのID)
	uint8 u8Ct_Second_To_Rescan; //!< 親を再探索するまでの秒
	uint8 u8Ct_Second_To_Relocate; //!< 親の存在を確認するためのダミー送信までの間隔
	uint8 u8Ct_Second_To_Beacon; //!< 子機に対しての定期ビーコン間隔
	uint8 u8Ct_ScanDur_10ms; //!< 探索時のチャネルごとのタイムアウト時間
	uint8 u8Ct_LostParent; //!< 親機喪失を判断するまでのRelocate回数
	uint8 u8Ct_ScanFail; //!< スキャンの連続失敗回数
	uint8 u8Ct_FrameUp; //!< パケットのシーケンス番号(上り方向)
	uint8 u8Ct_FrameDn; //!< パケットのシーケンス番号(下り方向)
	bool_t bResuming; //!< スリープ復帰などで Resume するときの管理フラグ
} tsToCoNet_NwkLyTr_Context;


/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 親機・中継機から子機への下位データ転送の再送回数
 * - ブロードキャストであるため強制再送を意味する 0x80 のマスクを設定している。
 */
#define TOCONET_MOD_LAYERTREE_DOWNTX_RETRY 0x83
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 親機・中継機から子機への下位データ転送の最小遅延
 */
#define TOCONET_MOD_LAYERTREE_DOWNTX_DELAY_MIN 10
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 親機・中継機から子機への下位データ転送の最大遅延
 */
#define TOCONET_MOD_LAYERTREE_DOWNTX_DELAY_MAX 300
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 親機・中継機から子機への下位データ転送の送信間隔
 */
#define TOCONET_MOD_LAYERTREE_DOWNTX_RETRY_DUR 56

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 子機から親機・中継機への上位データ転送の再送回数
 */
#define TOCONET_MOD_LAYERTREE_UPTX_RETRY 3
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 子機から親機・中継機への上位データ転送の最小遅延
 */
#define TOCONET_MOD_LAYERTREE_UPTX_DELAY_MIN 10
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 子機から親機・中継機への上位データ転送の最大遅延
 */
#define TOCONET_MOD_LAYERTREE_UPTX_DELAY_MAX 30
/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 子機から親機・中継機への上位データ転送の再送間隔
 */
#define TOCONET_MOD_LAYERTREE_UPTX_RETRY_DUR 56

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * スキャン間隔。子機が親機・中継機を見失ったときに、再度スキャンする間隔（設定可能）
 */
#define TOCONET_MOD_LAYERTREE_DEFAULT_RESCAN_s 5

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * ロケート間隔。子機が親機・中継機に対してダミー送信にて存在を確認する間隔（設定可能）
 */
#define TOCONET_MOD_LAYERTREE_DEFAULT_RELOCATE_s 3

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * 親機・中継機をスキャン時の最大待ち時間。
 * - 輻輳をさけるため、近隣のノード数に対して十分な時間余裕が必要
 * - 1ノード10ms 程度は見込んでおく
 */
#define TOCONET_MOD_LAYERTREE_DEFAULT_CHSCAN_10ms 10

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * ロケートの最大失敗数。この失敗数を超えた場合、子機は親機を見失ったと判定する
 */
#define TOCONET_MOD_LAYERTREE_DEFAULT_MAX_FAIL_LOCATE 5

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * サブレイヤの最大数
 */
#define TOCONET_MOD_LAYERTREE_DEFAULT_MAX_SUBLAYER 2

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * ビーコン間隔
 */
#define TOCONET_MOD_LAYERTREE_DEFAULT_BEACON_DUR 10

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * レイヤー接続オプション(u8LayerOptions)
 * 直上のレイヤのみ接続する
 */
#define TOCONET_MOD_LAYERTREE_LAYEROPTMASK_CONNECT_ONLY_ABOVE (1<<0)

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 *  u8StartOpt を固定親機とし u32AddrHigherLayer の値を参照する
 *  必然的に、起動時のスキャンは省略される
 */
#define TOCONET_MOD_LAYERTREE_RESUMEOPT_NO_RELOCATE_ON_RESUME 0x01

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 *  u32AddrHigherLayer の値を参照し、そのアドレスを固定親機とする
 *  必然的に、起動時のスキャンは省略される。
 *
 *  ※ 同時に u8Second_To_Rescan, u8Second_To_Relocate, u8Ct_To_Relocate を 0xFF と設定すること
 */
#define TOCONET_MOD_LAYERTREE_STARTOPT_FIXED_PARENT 0x01

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 *  NB_BEACON により上位を解決する。
 *  u32AddrHigherLayer の値が設定されている場合は、始動時にそのアドレスを参照する。
 *  ※ u8Second_To_Rescan, u8Second_To_Relocate, u8Ct_To_Relocate は無効になる
 */
#define TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON 0x02

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * ネットワーク中継制御のための拡張情報を生成する
 *
 * @param bToParent 親機向けへの転送なら TRUE, 親機からの転送なら FALSE
 * @param u8Lqi1st 最初に受信したときの LQI 値
 * @param u8XorRt 中継アドレスのXOR
 * @param u8SeqRt 中継局の中継パケットのシーケンス番号（次の受信先で同一パケットのフィルタリングに使う）
 * @param u8Hops 中継数
 * @return 32bitの拡張情報
 */
static inline uint32 u32GenRouteCtlInfo(bool_t bToParent, uint8 u8Lqi1st, uint8 u8XorRt, uint8 u8SeqRt, uint8 u8Hops) {
	u8SeqRt = (u8SeqRt & 0x3F) | (bToParent ? 0x80 : 0x00);

	return (uint32)u8SeqRt  << 24 |
			(uint32)u8Lqi1st << 16 |
			(uint32)u8XorRt  << 8 |
			(uint32)u8Hops;
}

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * ネットワーク中継制御のための拡張情報を分解する
 *
 * @param u32RouteInfo 経路管理情報
 * @param pbToParent 親機向けへの転送なら TRUE, 親機からの転送なら FALSE
 * @param pu8Lqi1st 最初に受信したときの LQI 値
 * @param pu8XorRt 中継アドレスのXOR
 * @param pu8SeqRt 中継局の中継パケットのシーケンス番号（次の受信先で同一パケットのフィルタリングに使う）
 * @param pu8Hops 中継数
 */
static inline void vSplitRouteCtlInfo(uint32 u32RouteInfo, bool_t *pbToParent, uint8 *pu8Lqi1st, uint8 *pu8XorRt, uint8 *pu8SeqRt, uint8 *pu8Hops) {
	*pbToParent = (u32RouteInfo >> 24) & 0x80 ? TRUE : FALSE;
	*pu8SeqRt = (u32RouteInfo >> 24) & 0x3F;
	*pu8Lqi1st = (u32RouteInfo >> 16) & 0xFF;
	*pu8XorRt = (u32RouteInfo >> 8) & 0xFF;
	*pu8Hops = u32RouteInfo & 0xFF;
}

/** @ingroup grp_ToCoNet_mod_Nwk_LayerTree
 * LAYER TREE 形式のコンテキストにダウンキャストする
 */
#define TOCONET_MOD_LAYERTREE_DOWNCAST_CONTEXT() \
	sToCoNet_Modules.pvModNwkContext ? \
			(((tsToCoNet_Nwk_Context *)(sToCoNet_Modules.pvModNwkContext))->sInfo.u8NwkTypeId == TOCONET_NWK_TYPE_LAYER_TREE ? \
				(tsToCoNet_NwkLyTr_Context *)sToCoNet_Modules.pvModNwkContext \
				: NULL) \
			: NULL;


#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_MOD_NWK_LAYERTREE_H_ */
