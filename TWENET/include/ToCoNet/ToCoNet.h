/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/*
 * ToCoNet.h
 *
 *  Created on: 2012/12/30
 *      Author: seigo13
 */

/** @file
 *  ToCoNet メイン処理関数群
 *
 * @defgroup grp_ToCoNet_Main ToCoNet メイン処理部
 * ToCoNet での中核部分の処理を記述。
 *
 * @defgroup grp_ToCoNet_Utils ToCoNet ユーティリティ
 * ToCoNet で使用する補助関数群。
 */

#ifndef TOCONET_H_
#define TOCONET_H_

#include <jendefs.h>
#include "ToCoNet_event.h"
#include "ToCoNet_TxRxQueue.h"
#include "ToCoNet_Context.h"
#include "fprintf.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

/** @ingroup grp_ToCoNet_Main
 * ToCoNet 設定構造体。
 * cbAppColdStart() にて指定し、ネットワークやマイコンの動作を決定する。
 */
typedef struct _tsToCoNet_AppContext {
	uint32 u32AppId; 			//!< 32bitのアプリケーションID。本IDでToCoNet同士の識別を行う。（必須設定項目）
	uint32 u32ChMask; 			//!< 利用するチャネル群。NWK層やNeibourScanで利用する。(必須設定項目)
	uint16 u16ShortAddress; 	//!< モジュールのショートアドレス。指定しなければモジュールのシリアル番号から自動生成される。0xFFFFは指定出来ない。Nwk層利用時は指定しないこと。
	uint8 u8Channel; 			//!< モジュールのチャネル。NWK層の動作により変更される場合がある。(必須設定項目, Chマスクに存在するチャネルを指定すること)

	uint8 u8CPUClk; 			//!< 通常稼働時のCPUクロック。3:32MHz, 2:16Mhz, 1:8Mhz, 0:4Mhz を指定する(規定値は 2)
	uint8 u8TxPower; 			//!< モジュールの出力 3:最大 2: -11.5db 2: -23db 0:-34.5db となる (規定値は 3)
	uint8 u8TxMacRetry; 		//!< MAC層の再送回数 7..0 を指定する。(規定値は3, Nwk層では１を推奨)
	bool_t bRxOnIdle; 			//!< TRUE:無線回路アイドル時も受信回路をオープンする。受信が必要な場合は必ずTRUEに設定する。(規定値は FALSE, Nwk層ではTRUE必須)
	bool_t bPromiscuousMode; 	//!< テスト受信モード。通常は設定してはいけません (規定値は FALSE)
	bool_t bSkipBootCalib;		//!< 始動時のキャリブレーション処理を省略する
	uint8 u8Osc32Kmode; 		//!< 32K 水晶のモード (0x00: RC, 0x02: 32K水晶, 0x03: 32K発振器)
	uint8 u8CCA_Retry; 			//!< CCA のリトライ回数 (通常は変更しない)
	uint8 u8CCA_Level; 			//!< CCA アルゴリズムの開始レベル (通常は変更しない)
	uint8 u8MacInitPending; 	//!< TRUE:MAC 層の初期化をシステム始動時に行わない。無線部を使用せずに動作させる場合に設定します。

	bool_t bNoAckMode;			//!< Ack を一切返さない。起動時の設定のみ反映され、起動後は変更できない。(通常は変更しない)
	bool_t bRxExtPan;			//!< 他のPANからのパケットを受信する (1.0.8)

	uint8 u8RandMode; 			//!< 乱数生成方法の指定。0:ハード 1:システム経過時間を元に生成 2:MT法 3:XorShift法 (32kOscモードで外部水晶が利用されたときは 0 の場合 XorShift 方を採用する)
	uint16 u16TickHz;			//!< システムの Tick カウントの周期(規定値は250=4ms, 1000で割り切れる値にすること。事実上 1000, 500, 250, 200, 100 のみ)
} tsToCoNet_AppContext;

/** @ingroup grp_ToCoNet_Main
 * ToCoNet 設定構造体の実体
 */
extern tsToCoNet_AppContext sToCoNet_AppContext;

/** @ingroup grp_ToCoNet_Main
 * アプリケーション層向け無線受信データ構造体
 */
typedef struct {
	uint32 u32SrcAddr; 	//!< 送信元アドレス
	uint32 u32DstAddr; 	//!< 送信先アドレス

	uint8 u8Lqi; 		//!< 受信時のリンク品質(電波強度) LQI は Link Quality Indicator の略。
	uint8 u8Len; 		//!< データ長
	uint8 u8Cmd; 		//!< データ種別 (0..7)
	uint8 u8Seq; 		//!< シーケンス番号 (0x00..0xFF)
	uint8 *auData; 		//!< データ[0..u8Len-1]
	uint32 u32Tick; 	//!< 受信時のTickTime
	uint8 u8Hops; 		//!< NWK層での中継回数
	uint8 u8RouteXOR;	//!< NWK層での中継経路の各アドレスを XOR 計算したもの。中継経路推定を行える
	uint8 u8Lqi1St;		//!< NWK層での最初のルータが受信したLQI値

	bool_t bNwkPkt;		//!< NWK層からのパケットなら TRUE
	bool_t bSecurePkt;	//!< 暗号化されて届いた場合は TRUE
	uint16 u16ExtPan;	//!< 0:外部PANへの送信ではない 1..0x0FFF: 外部PANへの送信 (上位4bitはリザーブ)
} tsRxDataApp;

/** @ingroup grp_ToCoNet_Main
 * アプリケーション層向け無線送信データ構造体
 * - 構造体は 0 クリアした後、必要なもののみ設定する
 * - [N] を記載するものは Nwk 層で指定可能
 */
typedef struct {
	uint32 u32SrcAddr; 	//!< 送信元アドレス。16bit(ショート)または32bit(ロング)を指定する。(Nwk層では自動的に設定される)
	uint32 u32DstAddr; 	//!< [N] 送信先アドレス。16bit(ショート)または32bit(ロング)を指定する。

	uint8 u8Cmd; 		//!< [N] データ種別 (0..7)。データの簡易識別子。
	uint8 u8Seq; 		//!< [N] シーケンス番号
	uint8 u8Len; 		//!< [N] データ長
	uint8 u8CbId; 		//!< [N] 送信識別ID。送信完了イベントの引数として渡され、送信イベントとの整合を取るために使用する
	uint8 auData[MAX_TX_APP_PAYLOAD]; //!< [N] 送信データ
	bool_t bAckReq; 	//!< TRUE Ack付き送信を行う
	bool_t bSecurePacket; //!< (未実装) TRUE: AES による暗号化を行う
	uint8 u8Retry; 		//!< MACによるAck付き送信失敗時に、さらに再送する場合(ToCoNet再送)の再送回数

	uint16 u16ExtPan;		//!< 0:外部PANへの送信ではない 1..0x0FFF: 外部PANへの送信 (上位4bitはリザーブ)

	uint16 u16DelayMax; //!< 送信開始までのディレー(最大)[ms]。指定しない場合は 0 にし、指定する場合は Min 以上にすること。
	uint16 u16DelayMin; //!< 送信開始までのディレー(最小)[ms]
	uint16 u16RetryDur; //!< ToCoNet再送再送間隔[ms]。

	uint32 u32Tick; 	//!< 内部で使用される。
	void* pvOpt;		//!< モジュール内部で利用
} tsTxDataApp;

/** @ingroup grp_ToCoNet_Main
 * 中継パケット情報構造体。中継途上のパケットを検出する。
 * - u32CtlInfo, pTx はスタック開発者向けの情報。
 * - 現バージョンでは中継の失敗については報告しない。
 * - 現バージョンでは中継パケット情報を
 */
typedef struct {
	bool_t bUpstream; //!< TRUEならのぼり方向のパケット
	uint8 u8Hops; //!< ホップ数
	uint8 u8Lqi; //!< 中継機が受信したLQI
	uint32 u32DstAddr; //!< 宛先アドレス
	uint32 u32SrcAddr; //!< 送り先アドレス
	uint32 u32CtlInfo; //!< 中継制御用のコントロール情報
	bool_t bAbort; //!< TRUEなら中継しない
	tsTxData *pTx; //!< 中継送信用のデータ
} tsRoutePktInfo;

/** @ingroup grp_ToCoNet_Main
 * ToCoNet のメインループに対応するアプリケーションコールバック。
 */
extern void cbToCoNet_vMain();

/** @ingroup grp_ToCoNet_Main
 * 無線パケット受信時のアプリケーションコールバック。
 * @param psRx 受信データ
 */
extern void cbToCoNet_vRxEvent(tsRxDataApp *psRx);

/** @ingroup grp_ToCoNet_Main
 * 無線パケット送信完了時のアプリケーションコールバック。
 * @param u8CbId 送信要求時に指定した送信識別ID。
 * @param bStatus 完了フラグ。0:失敗 以外:成功
 */
extern void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus);

/** @ingroup grp_ToCoNet_Main
 * ハードウェア割り込みの遅延実行部に対応するアプリケーションコールバック。
 * @param u32DeviceId デバイスID。
 * @param u32ItemBitmap 割り込み要因ビットマップ。
 */
extern void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap);

/** @ingroup grp_ToCoNet_Main
 * ハードウェア割り込みの割り込みハンドラから呼び出されるアプリケーションコールバック。
 * 割り込みハンドラ中であるため、大きな処理は記述出来ない。
 * @param u32DeviceId デバイスID。
 * @param u32ItemBitmap 割り込み要因ビットマップ。
 * @return TRUEなら割り込みは処理済みとして遅延実行は行わない。
 */
extern uint8 cbToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap);

/** @ingroup grp_ToCoNet_Main
 * NWK層からイベント通知。
 * @param ev イベント種別。
 * @param u32evarg イベント引数。
 */
extern void cbToCoNet_vNwkEvent(teEvent ev, uint32 u32evarg);

/** @ingroup grp_ToCoNet_Main
 * 電源投入またはRAMOFFウェイクアップ開始時に呼び出されるコールバック関数。
 * 本関数はシステム始動時に２回呼び出される。
 * 最初にbStartがFALSEで呼び出される。この時点ではペリフェラル等も初期化されていない
 * 初期で、起動要因の確認や利用するToCoNetモジュールの宣言を行う。次にbStartがTRUE
 * で呼び出されたときには sToCoNet_AppContext の設定などを行う。
 * @param bStart 始動フラグ。FALSEはハードウェア初期化前の超初期、TRUEで各種設定
 * を行う。
 */
extern void cbAppColdStart(bool_t bStart);

/** @ingroup grp_ToCoNet_Main
 * RAMONウェイクアップ時に呼び出される関数。
 * 本関数はシステム始動時に２回呼び出される。
 * 最初にbStartがFALSEで呼び出される。この時点ではペリフェラル等も初期化されていない
 * ため、ウェイクアップ割り込み要因などの確認が可能。
 * 本関数でのToCoNet モジュールの登録は不要。
 * @param bStart 始動フラグ。FALSEはハードウェア初期化前の超初期、TRUEで各種設定
 * を行う。
 */
extern void cbAppWarmStart(bool_t bStart);

/** @ingroup grp_ToCoNet_Main
 * ToCoNet の時間管理を行うティックカウンタ[ms]。本カウント値は TICK TIMER を
 * 用いて更新され、更新の周期は sToCoNet_AppContext.u16TickHz により決定される。
 */
extern volatile uint32 u32TickCount_ms;

/** @ingroup grp_ToCoNet_Main
 * バージョン情報。
 */
extern const uint8 au8ToCoNetVersionInfo[];

// 802.15.4 MAC Handling (low level procedure).
void ToCoNet_vMacStart();

/** @ingroup grp_ToCoNet_Main
 *
 * MACの稼働状況を返す。
 *
 * @return TRUEなら初期化済み、FALSEならまだ初期化していない。
 */
bool_t ToCoNet_bMacStarted();

/** @ingroup grp_ToCoNet_Main
 *
 * MACを使った低レベル送信関数
 *
 * @return TRUEなら要求は受理された(まだ送信されていないので失敗成功は後のイベントで判断) FALSEなら失敗
 */
uint8 ToCoNet_bMacTxReq(tsTxDataApp *); // 送信要求を行う

/** @ingroup grp_ToCoNet_Main
 *
 * 送信要求が可能かどうか(FIFOキューに空きがあるか)を判定する。
 *
 * @return TRUEなら要求が可能. FALSEならキューがフルなので待つ必要がある。
 */
bool_t ToCoNet_bMaxTxQueueAvailable();

/** @ingroup grp_ToCoNet_Main
 *
 * 無線部の設定を行う。意図的に無線設定を変更する場合に呼び出す。
 *
 */
void ToCoNet_vRfConfig(); // 意図的にネットワーク設定を変えたいときに呼ぶ

/** @ingroup grp_ToCoNet_Main
 *
 * 無線部の設定を行う。意図的に無線設定を変更する場合に呼び出す。
 *
 */
void ToCoNet_vChConfig(); // 意図的にネットワーク設定を変えたいときに呼ぶ

/** @ingroup grp_ToCoNet_Utils
 *
 * PAN ID を取得する。PAN ID はシステムにより自動生成される。
 *
 * @return PAN ID を返す。
 *
 */
uint16 ToCoNet_u16GetPanId();

/** @ingroup grp_ToCoNet_Utils
 *
 * モジュールの32bitアドレスを返す。32bitアドレスはモジュールのシリアル番号を元に
 * システムにより自動生成される。
 *
 * @return 32bitモジュールアドレス。
 *
 */
uint32 ToCoNet_u32GetSerial();

/** @ingroup grp_ToCoNet_Utils
 *
 * ウェイクアップタイマーによるスリープを行う。
 *
 * @param u8Device 使用するウェイクアップタイマー。
 * @param u32Periodms スリープ期間[ms]。
 * @param bPeriodic 周期スリープを行うかどうか。
 *   周期スリープを指定した場合、前回の起床時間からの経過時間を差し引いて次の起床を行う。
 *   すでに次の周期をすぎていた場合は本関数呼び出し時点から指定期間のスリープとする。
 *   周期スリープを指定しない場合、本関数呼び出し時点から指定期間のスリープを行う。
 * @param bRamOff FALSE なら RAM を保持した状態でスリープを行う。TRUE なら、
 *   RAMを保持しないスリープを行う。RAM を保持しない場合、スリープ時の消費電流を
 *   減少させられるが、起床時の消費電流は大きくなる。
 */
void ToCoNet_vSleep(uint8 u8Device, uint32 u32Periodms, bool_t bPeriodic, bool_t bRamOff);

/** @ingroup grp_ToCoNet_Utils
 * デバッグ出力の初期化。初期化済みの tsFILE* (UART)を渡す。
 * @param pSer 出力先構造体（UART）
 */
void ToCoNet_vDebugInit(tsFILE*pSer);

/** @ingroup grp_ToCoNet_Utils
 * デバッグ出力レベルを指定する。
 * @param u8lvl デバッグレベルの指定。u8lvl以下のメッセージを出力する。
 */
void ToCoNet_vDebugLevel(uint8 u8lvl);

/** @ingroup grp_ToCoNet_Utils
 *
 * 32bitの乱数を得る。乱数の生成方法は sToCoNet_AppContext.u8RandMode で指定する。
 * デフォルトはハードウェアによる乱数生成だが、32Khz タイマーに高精度外部タイマーを用いた
 * 場合、期待した乱数は得られない。またハードウェアによる方法では 16bit 値の取得となるため
 * 上位は下位をビット変換したものとなる。
 * @return 32bitの乱数。
 */
uint32 ToCoNet_u32GetRand();

/** @ingroup grp_ToCoNet_Utils
 * 16bit乱数を計算する。
 * @return 16bitの乱数。
 */
#define ToCoNet_u16GetRand() (ToCoNet_u32GetRand() & 0xFFFF)

/** @ingroup grp_ToCoNet_Main
 * ライブラリのバージョンを取得する。
 * @return バージョン番号。0x00bbccdd bb:メジャー cc:マイナー dd:ビルド
 */
uint32 ToCoNet_u32GetVersion();

/** @ingroup grp_ToCoNet_Main
 * Ex ライブラリのバージョンを取得する。
 * @return バージョン番号。0x00bbccdd bb:メジャー cc:マイナー dd:ビルド
 */
uint32 ToCoNet_u32GetVersion_LibEx();

/** @ingroup grp_ToCoNet_Main
 * ユーティリティ(libToCoNetUtils)ライブラリのバージョンを取得する。
 * @return バージョン番号。0x00bbccdd bb:メジャー cc:マイナー dd:ビルド
 */
uint32 ToCoNet_u32GetVersion_LibUtils();


/** @ingroup grp_ToCoNet_Main
 * 暗号化エンジンの初期化用構造体
 */
typedef struct {
	uint8 u8KeyLen; //!< 鍵の長さ(16のみ)
} tsCryptDefs;

/** @ingroup grp_ToCoNet_Main
 * 暗号化の鍵(16バイト)を登録する。
 */
bool_t ToCoNet_bRegisterAesKey(uint8 *pu8Key, tsCryptDefs *sCryptDefs);

/** @ingroup grp_ToCoNet_Utils
 * RCタイマーをキャリブレーションする。
 *
 * 時間 T[sec] としたときに、RCタイマーのカウント数 N は
 *  N = (10000/キャリブレーション値) x 32000 x T
 * となる。
 *
 * @param u16val 0ならAPIにより、5000-15000なら設定値を指定する。0xFFFFを与えると設定値を取得する。
 * @return 設定されたキャリブレーション値
 */
uint16 ToCoNet_u16RcCalib(uint16 u16val);
#define ToCoNet_u16GetRcCalib() ToCoNet_u16RcCalib(0xFFFF)

/** @ingroup grp_ToCoNet_mod_Tx_Utils
 * パワーアンプ付きのモジュールかどうかの判定
 * @return TRUEならパワーアンプ付き
 */
bool_t ToCoNet_bIsStrong();

/** @ingroup grp_ToCoNet_mod_Tx_Standard
 * 速やかに送信キューを処理する（実験的な関数）
 *
 * 送信API実行直後に呼び出す。
 *   ToCoNet_bMacTxReq(); //  または ToCoNet_Nwk_bTx();
 *   ToCoNet_Tx_vProcessQueue(); // 送信処理をタイマーを待たずに実行する
 *
 * - 処理前のキューがある場合や、ライブラリ内での送信があるような場合、
 *   直前の送信要求が処理されることは保証されない
 * - 送信遅延が設定されている場合は、速やかに送信処理が行われない
 * - ネットワーク層の送信の場合、Mininode のみ利用可能。
 */
void ToCoNet_Tx_vProcessQueue();

#ifdef __cplusplus
}
#endif // C++

// include additional headers
#include "ToCoNet_packets.h"
#include "AppQueueApi_ToCoNet.h"
#include "ToCoNet_mod_NbScan.h"
#include "ToCoNet_mod_Nwk_Common.h"
#include "ToCoNet_mod_Nwk_LayerTree.h"
#include "ToCoNet_mod_DuplicateChecker.h"


#endif /* TOCONET_H */
