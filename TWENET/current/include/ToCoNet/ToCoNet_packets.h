/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */


/*
 * TOCONET_PACKETs.h
 *
 *  Created on: 2013/01/02
 *      Author: seigo13
 */

#ifndef TOCONET_PACKETS_H_
#define TOCONET_PACKETS_H_

#ifdef __cplusplus
extern "C" {
#endif // C++

/* HEADER
 *   OCTET : 識別ヘッダ (受信キューを切り替えるための仕分けヘッダ)
 *     ２進数で HIJK:LMNO
 *      HIJKLM-> 識別子（固定、APPID から自動生成）
 *               TOCONET_MAC_HEADER_BYTE1_PACKET_IDENT_MASK
 *
 *       N    -> 0 (直接送信), 1 (ネットワーク層への送信)
 *       O    -> 0 (制御用、送信元がモジュールのパケット) 1 (アプリケーション用、送信元がアプリケーション)
 *               TOCONET_MAC_HEADER_BYTE1_PACKET_TYPE_NWK_LAYER_MASK
 *                 TOCONET_PACKET_HEADER1_NWK_MASK (N)
 *                 TOCONET_PACKET_HEADER1_TYPE_MASK (O)
 *
 *                 TOCONET_PACKET_HEADER1_CTL
 *                 TOCONET_PACKET_HEADER1_APP
 *                 TOCONET_PACKET_HEADER1_CTL_OVER_NWK
 *                 TOCONET_PACKET_HEADER1_APP_OVER_NWK
 *
 *   OCTET : パケット内容を定義
 *      ２進数で HIJK:LMNO
 *       H    -> Security Enabled (セキュリティパケット)
 *               TOCONET_MAC_HEADER_BYTE2_SECURITY_BIT_MASK
 *       IJK  -> 予約
 *       L    -> 予約
 *       MNO  -> データストリームの識別子 (0:DATA, 1:CMD, 2~7: アプリケーション用に割り当て)
 *               TOCONET_MAC_HEADER_BYTE2_DATA_TYPE_MASK
 *                 TOCONET_PACKET_HEADER2_CTL_DATA (0x0)
 *                 TOCONET_PACKET_HEADER2_CTL_CMD (0x1)
 */
#define TOCONET_PACKET_HEADER1_CTL 0x00 //!< ヘッダ１、制御用パケット識別子
#define TOCONET_PACKET_HEADER1_APP 0x01 //!< ヘッダ１、アプリケーション用識別子
#define TOCONET_PACKET_HEADER1_CTL_OVER_NWK 0x02 //!< ヘッダ１、制御用パケット、ネットワーク経由、識別子
#define TOCONET_PACKET_HEADER1_APP_OVER_NWK 0x03 //!< ヘッダ１、アプリケーション用、ネットワーク経由、識別子

#define TOCONET_PACKET_HEADER1_TYPE_MASK 0x1 //!< ヘッダ１、制御用、アプリケーション用の識別マスク（ビット１）
#define TOCONET_PACKET_HEADER1_TYPE_MASK_CTL 0x0 //!< ヘッダ１、制御用、アプリケーション用の識別マスク（ビット１）
#define TOCONET_PACKET_HEADER1_TYPE_MASK_APP 0x1 //!< ヘッダ１、制御用、アプリケーション用の識別マスク（ビット１）

#define TOCONET_PACKET_HEADER1_NWK_MASK 0x2 //!< ヘッダ１、ネットワーク経由の識別マスク（ビット２）
#define TOCONET_PACKET_HEADER1_NWK_MASK_NWK 0x2 //!< ヘッダ１、ネットワーク経由の識別マスク（ビット２）
#define TOCONET_PACKET_HEADER1_NWK_MASK_NONWK 0x0 //!< ヘッダ１、ネットワーク経由の識別マスク（ビット２）

#define TOCONET_PACKET_HEADER2_CTL_DATA 0x00 //!< ヘッダ２、データ種別 (データ)
#define TOCONET_PACKET_HEADER2_CTL_CMD 0x01 //!< ヘッダ２、データ種別 (コマンド)

#define TOCONET_PACKET_CMD_APP_DATA 0x00 //!< データ種別、データパケット (ユーザAPI向け定義)
#define TOCONET_PACKET_CMD_APP_CMD 0x01 //!< データ種別、コマンドパケット (ユーザAPI向け定義)

#define TOCONET_PACKET_CMD_APP_USER 0x02 //!< USER コマンド名 (0x02~0x07)

// 設定されるとルーティング対象パケット (Nwk層による転送を行い、Nwk層がすべて処理する)
#define TOCONET_PACKET_IS_HEADER_ROUTING_MASK(h1) ((h1)&TOCONET_MAC_HEADER_BYTE1_PACKET_TYPE_NWK_LAYER_MASK)

// ヘッダ値を uint16 で取得
#define _TOCONET_HEADER_BASE(p) ((*(p)<<8)|(*(p+1) & 0x00F0))

/*
 * BITMAP Allocation
 *   XXXXYYYY -> マスクは 11110000 で、XXXX は 0001~0111 まで割当。
 *   足りなくなれば、使用数の少ないものをマスクを限定して拡張する。
 */

// Neighbour scan Master 側は 0001:000X
#define TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_MASTER_CMD_MASK (0xF0+0x8+0x4+0x2)
#define TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_MASTER_CMD_BASE 0x10

// Neighbour scan Slave 側は 0001:001X
#define TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_SLAVE_CMD_MASK (0xF0+0x8+0x4+0x2)
#define TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_SLAVE_CMD_BASE 0x12

// MessagePool は 0001:1??X を割当
#define TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_CMD_MASK (0xF0+0x8+0x4)
#define TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_CMD_BASE 0x18

// Layer Tree のコマンドは未使用
#define TOCONET_PACKET_CTL_NWK_LAYERTREE_CMD_MASK 0xF0
#define TOCONET_PACKET_CTL_NWK_LAYERTREE_CMD_BASE 0x20

/**
 * TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_REQUEST
 *   近隣ノードスキャン要求。
 *   近隣ノードへブロードキャストし、条件に合致するノードが応答を返す。
 *
 *   送信元： 拡張アドレス 64bit
 *   送信先： ショートアドレス 0xFFFF または指定アドレス
 *   方式: ブロードキャスト
 *
 * [2] BE_DWORD  : HEADER
 * [1] SEQ       : SEQUENCE NUMBER
 *
 * [1] OCTET     : TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_REQUEST
 * [4] LE_DWORD  : アプリケーションID
 * [2] LE_WORD   : スキャンタイム
 * [1] OCTET     : アプリケーション任意識別子
 * [1] OCTET     : ネットワーク層の役割指定マスク
 * [1] OCTET     : ネットワーク層の親機までの距離 (未実装)
 * [1] OCTET     : ネットワーク層の状態マスク (未実装)
 */
#define TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_REQUEST (TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_SLAVE_CMD_BASE+0x0)

/**
 * TOCONET_PACKET_CTL_NEIGHBOUR_LOCATE
 *   近隣ノードの確認要求
 *   近隣ノードへブロードキャストし、条件に合致するノードが応答を返す。
 *
 *   送信元： 拡張アドレス 64bit
 *   送信先： 指定アドレス
 *   方式: ACK あり送信
 *
 * [2] BE_DWORD  : HEADER
 * [1] SEQ       : SEQUENCE NUMBER
 *
 * [1] OCTET     : TOCONET_PACKET_CTL_NEIGHBOUR_LOCATE
 */
#define TOCONET_PACKET_CTL_NEIGHBOUR_LOCATE (TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_SLAVE_CMD_BASE+0x1)

/**
 * TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_RESPOND
 *   近隣ノードスキャン要求に対する応答。
 *   アプリケーションIDに合致するノードのみ応答を返す。
 *
 *   送信元： 拡張アドレス 64bit
 *   送信先: 送信元アドレス
 *   方式: ACK あり送信
 *
 * [2] BE_DWORD  : HEADER
 * [1] SEQ       : SEQUENCE NUMBER
 *
 * [1] OCTET     : TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_REQUEST
 * [4] LE_DWORD  : アプリケーションID
 * [2] LE_WORD   : ショートアドレス
 * [1] OCTET     : アプリケーション任意識別子 (子機、親機といった用途など)
 * [1] OCTET     : ネットワーク層の役割指定(中継機、コーディネータなどの区別)
 * [1] OCTET     : ネットワーク層の親機までの距離
 * [1] OCTET     : ネットワーク層の状態
 * [1] OCTET     : 現在階層のルートまでの距離
 * [1] OCTET     : ネットワーク層のセッションID(受け取った下流側は同じIDなら上流へパケットを流しても安全である)
 */
#define TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_RESPOND (TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_MASTER_CMD_BASE+0x0)
/**
 * TOCONET_PACKET_CTL_NEIGHBOUR_BEACON
 *   近隣に対して定期的にノードの存在を知らせる。
 *   内容は TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_RESPOND と同等。
 */
#define TOCONET_PACKET_CTL_NEIGHBOUR_BEACON (TOCONET_PACKET_CTL_NEIGHBOUR_SCAN_MASTER_CMD_BASE+0x1)


/**
 * TOCONET_PACKET_CTL_MESSAGEPOOL_DEPLOY
 *
 *   メッセージを各ルータに展開する
 *
 * [2] BE_DWORD  : HEADER
 * [1] SEQ       : SEQUENCE NUMBER
 *
 * [1] OCTET     : TOCONET_PACKET_CTL_MESSAGEPOOL_DEPLOY
 * [1] OCTET     : メッセージ SLOT(未使用)
 * [2] LE_WORD   : メッセージの寿命（受信時からの経過秒）、０は寿命無し
 * [1] OCTET     : メッセージ長
 * [N] メッセージペイロード
 */
#define TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_DEPLOY (TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_CMD_BASE + 0x1)

/**
 * TOCONET_PACKET_CTL_MESSAGEPOOL_REQUEST
 *
 *   メッセージの要求を行う
 *
 * [2] BE_DWORD  : HEADER
 * [1] SEQ       : SEQUENCE NUMBER
 *
 * [1] OCTET     : TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_REQUEST
 * [1] OCTET     : メッセージ SLOT
 * [1] OCTET     : OPTION
 * [N] OCTETS    : PAYLOAD
 */
#define TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_REQUEST (TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_CMD_BASE + 0x2)

/**
 * TOCONET_PACKET_CTL_MESSAGEPOOL_INFORM
 *
 *   要求されたメッセージの伝達
 *
 * [2] BE_DWORD  : HEADER
 * [1] SEQ       : SEQUENCE NUMBER
 *
 * [1] OCTET     : TOCONET_PACKET_CTL_MESSAGEPOOL_INFORM
 * [1] OCTET     : メッセージ SLOT
 * [2] LE_WORD   : メッセージの経過時間
 * [1] OCTET     : メッセージ長
 * [N] メッセージペイロード
 */
#define TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_INFORM (TOCONET_PACKET_CTL_NWK_MESSAGEPOOL_CMD_BASE + 0x3)

#ifdef __cplusplus
}
#endif // C++

#endif /* TOCONET_PACKETS_H_ */
