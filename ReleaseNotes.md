# MWSDKライブラリ 改訂履歴

各ライブラリの主要な修正箇所を記載しています。

以下の個別ライブラリについても参照ください。

* mwxライブラリ ([解説](https://mwx.twelite.info/)|[レポジトリ](https://github.com/monowireless/mwx))
* twesettingsライブラリ ([レポジトリ](https://github.com/monowireless/twesettings))

バージョン名はリリース年と月を含めた MWSDK{YYYY}_{MM} となっています。UNOFFICIAL が付くものは、リリースのための包括的な検証作業を省略した適宜版となります。



# MWSDK2020_12

## TweApps

### App_CUE 1.0.1

* 新規

### App_CUE_OTA 1.0.1

* 新規

### App_Wings 1.1.4

* TWELITE CUE対応

### App_PAL 1.3.2

* TWELITE CUE 用のコードを追加
* NOTICEPALに受信のみモードを実装



## Act_samples

* Unit_PktParser の追加 (シリアル電文パーサー)
* Unit_EEPROM の追加 (EEPROMの読み書き)
* Unit_ADC (ADCの読み取り)
* Unit_CUE (TWELITE CUEのAct対応)



## mwx - 0.1.7

- 詳細は https://mwx.twelite.info/v/latest/ を参照
- pkt_parser の追加 (シリアルメッセージを解釈)
- EEPROM クラスの追加 (EEPROM の読み書き)
- receive_nwkless_pkt オプションを追加。NWK_SIMPLEネットワーク下で、無線パケットのアプリケーションID,チャネルは一致した通常パケットを受信することが出来る。



## twesettings - 0.2.6

コンパイル済みのライブラリは 2.5 のまま。ソースコードのみ。

* tweinteractive_defmenus.c が提供するメニューで項目数が10を超えたときの、リスト表示・選択動作の問題を修正。



# MWSDK2020_10

mwx ライブラリの更新、サンプルの追加・更新、より実践的なサンプル Actエクストラ(ActEx_???) の収録を行いました。

## TweApps

### App_PAL 1.2.1

* NOTICE PAL 対応
* 送信間隔を１秒おきの設定を可能にした

### App_Tag 2.2.2

* BME280のセンサー値演算式を修正

### App_Twelite 1.9.1

* シリアルメッセージで死活判定をするための ACK 応答を追加

### App_UART 1.4.1

* 重複パケットのタイムアウトを調整した (100ms 間隔で受信できる、重複判定のタイムアウトは１秒でこれを超えて同じパケットが届いた場合は再度表示される)

### App_Wings 1.1.3

* NOTICE PAL 対応
* その他、修正



## Act_samples

* 単機能のみを確認する Unit_??? サンプルの追加
* 通知PAL (NOTICE PAL) の LED ドライバ制御用のサンプル追加 (Unit_using_PAL_NOTICE)
* スリープ起床後送信するテンプレートコード Slp_Wk_and_Tx の追加
* Pal_MOT_oneshot の loop() を状態遷移で再記述
* Act で設定（インタラクティブモード）を用いるサンプル Setting を追加



## Act_extras

* ActEx_DIO_Trans を追加 (DI状態を高速サンプルして送信先のDOに再現)
* ActEx_Sns_BME280_SHT30 を追加 (環境センサー BME280, SHT30 デバイス)
* ActEx_Sns_MLX90614 (赤外温度センサー MLX90614 の利用)
* ActEx_Sns_VL53L1X (レーザー距離センサー VL53L1X の利用)



## mwx - 0.1.6

### [0.1.6b4]

- 商・余を計算する div100() をSerial等へ出力できるようにした
- smplbuf<> 配列クラスの実装変更。消費メモリの削減などを目的としてmwx::streamの継承をやめ、別途継承クラスとヘルパークラス定義した
- mwx_printf() mwx_snprintf() の関数を追加した
- the_twelite.stop_watchdog(), the_twelite.restart_watchdog() を追加した
- mwx::stream のメンテナンス: operator bool() の廃止。読み出しタイムアウトの設定で 0xff を指定した場合(.set_timeout(0xff))タイムアウトを無効に。その他 << 演算子の定義を追加。

### [0.1.6b3]

- NOTICE PAL / PCA9632 のサポートを追加 (解説 https://mwx.twelite.info/v/latest/boards/pal/pal_notice, サンプル https://github.com/monowireless/Act_samples/tree/master/Unit_using_PAL_NOTICE)
- 除算を行わない 8bit と 0..1000 間のスケール関数を追加。

### [6fd542c]

- 10,100,1000による除算（商と余を同時に計算） div10(), div100(), div1000() を追加。値域を制限し乗算とビットシフトを中心に構成。
- 暗号化パケットの対応メソッドを追加
  - packet_rx::is_secure_pkt() : 受信パケットが暗号化されているかどうかの判定
  - STG_STD::u8encmode() : インタラクティブモードでの暗号化設定を取得
  - STG_STD::pu8enckeystr() : インタラクティブモードでの暗号化鍵バイト列の取得

### [0.1.6b2]

- Serial1: デフォルトのポートは半導体の仕様では I2C と重複する DIO14,15 だが、通常 I2C に割り当てられるため DIO11(TxD), DIO9(RxD) とした。

- Serial: ボーレートの指定で /100 が発生するが、主要なボーレートについてこの計算を省略するようにした。

- Serial: available(), read() を外部で実施するための代理関数の保持を void* のみとし、仕様メモリを 8bytes 削減。

- typedef boolean の追加

- ネットワーク: 暗号化の対応を追加。

  - 暗号化を有効にするには NWK_SIMPLE::secure_pkt(const uint8_t*, bool = false) を設定追加する。１番目のパラメータは暗号キー、２番目を true にすると、平文のパケットも受信する。

    ```
    auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
    nwk	<< NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
    	<< NWK_SIMPLE::secure_pkt((const uint8_t*)"0123456789ABCDEF");
    	;
    ```

- SHT3xとBME280のセンサーサポート追加

- センサー: レガシーコード（Cライブラリのラッパクラス）で、設定パラメータや状態をやり取りするための仕掛けを追加した。

- センサー: SHT3x, BME280では I2C アドレスを指定可能とした。

- 設定: hide_items() を追加。不要な設定項目を削除可能。

- 設定: H/W UTIL メニューを追加。DIの状態表示、I2Cのプローブ、PAL EEPROM内容の表示。

- 設定: 暗号化関連のメニューの追加

### [4d8ce8b]

* I2C関連の修正(TwoWireクラスを用いて実装されたコードとの親和性を向上するための修正）
  * requestFrom(false) の処理時に NO_STOP メッセージの送信コードが無かったため処理が正常に行われなかった。
  * TwoWire のクラス名エリアスを追加した。
  * begin() 処理で、多重初期化しないようにした。
  * setClock() メソッドを追加（ただしダミー関数で何もしない）
  * WIRE_CONF::WIRE_???KHZ を追加。バスクロックの主要な設定値を追加した。

## twesettings - 0.2.5

更新無し



## TWENET C - 1.3.04 (L1304)

更新無し



# MWSDK2020_08_UNOFFICIAL - 2020-08-05

mwxライブラリに設定（インタラクティブモード）を追加しました。

## mwx - 0.1.5

* 設定ビヘイビア <STG_STD> を追加
  * mwx ライブラリ組み込みビヘイビアは network board application settings の４種類になったため、メインループの処理に settings の動作を追加
  * 設定ビヘイビアは Serial 入力を横取りするため、これに対応して別途入力用のFIFOキューを追加し、Serail.read() はこのキューからデータを取得するように変更した
  * <STG_STD>のテーブルは固定的であるが、項目名・ラベルの設定を差し替えるインタフェースを追加
* チャネルマネージャ（複数チャネル利用）の対応を追加

## twesettings - 0.2.5

* mwx にインタラクティブモードを追加するための整備
  * 設定名入れ替えのためのテーブル定義を追加
  * mwx 用のテーブルを定義していたが mwx ライブラリに移動

## TWENET C - 1.3.04 (L1304)

* 内部モジュールを mwx から登録するためのサポート関数を追加
* MOD_重複チェッカのタイムアウトと時間スケール値をパラメトリックにできるヘッダ定義を追加





# MWSDK2020_07_UNOFFICIAL - 2020-07-27

いくつかの問題とmwxライブラリのAPIを追加しました。

## mwx - 0.1.4

* MC3630 向けの初期化処理の強化
* setup(), wakeup() 中の delay() の精度向上
* Serial1 インスタンスの宣言の追加
* delayMilliseconds() の追加
* digitalReadBitmap() の追加
* Alalogue のコールバック関数が呼ばれない場合があった

## twesettings - 0.2.4

* μ秒待ちのAPIを追加 TWESYSUTIL_vWaitPollMicro()

## TWENET C - 1.3.03 (L1303)

変更なし



# MWSDK2020_05 - 2020-05-29

MWSDK2020_05 を起点に以後の変更内容を記載します。

## mwx - 0.1.3

## twesettings - 0.2.3

## TWENET C - 1.3.03 (L1303)

