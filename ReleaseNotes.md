# MWSDKライブラリ 改訂履歴

各ライブラリの主要な修正箇所を記載しています。

以下の個別ライブラリについても参照ください。

* mwxライブラリ ([解説](https://mwx.twelite.info/)|[レポジトリ](https://github.com/monowireless/mwx))
* twesettingsライブラリ ([レポジトリ](https://github.com/monowireless/twesettings))

バージョン名はリリース年と月を含めた MWSDK{YYYY}_{MM} となっています。UNOFFICIAL が付くものは、リリースのための包括的な検証作業を省略した適宜版となります。

# MWSDK2022_07

サンプルActの追加、TWELITE ARIA/TWELITE CUE/TWELITE PALに新機能追加、他。

## TWELITE APPS

* 000desc.txtに英訳を追加
* Samp_PingPong、Samp_I2Cを新規採用非推奨に変更

### キューアプリ(App_CUE/App_CUE_OTA) v1.1.0
* 指定した間隔ごとに加速度を1サンプルずつ送信する機能の追加
* 一定期間計測した加速度の平均、最小最大値を送信する機能の追加

### アリアアプリ(App_ARIA/App_ARIA_OTA) v1.1.0
* 温湿度の計測値にオフセットと係数を掛ける機能を追加

### パルアプリ(App_PAL) v1.5.1
* 動作センサーパルで加速度を1サンプルずつ送信する機能の追加
* 動作センサーパルで 一定期間計測した加速度の平均、最小最大値を送信する機能の追加
* 環境センサーパルで温湿度の計測値にオフセットと係数を掛ける機能を追加 

### 親機・中継機アプリ(App_PAL) v1.2.0
* 加速度の平均、最大最小値を出力する機能の追加

## Act_samples
* 000desc.txtに英訳を追加
* BRD_I2C_TEMPHUMID の追加
  * I2C センサーデバイスを用いて、定期起床からの計測および送信を行うサンプル

## Act_extras
* 000desc.txtに英訳を追加
* ActEx_AQM0802 の追加
  * Sitronix社のI2C接続LCDコントローラ ST7032 を実装したLCDモジュール AQM0802 を使用するサンプル

## mwx - 0.2.0
* ヒープ領域へのメモリ確保を行う Wire オブジェクトを変更
* utils.hでの名前の衝突を避けるため、関数名をG_OCTET()からG_BYTE()に変更
* attachIntDio()において、vAHI_DioInterruptEnable()の順番を変更
* ユニバーサルレシーバ (NWK_LAYERED, NWK_SIMPLE またはネットワークレスパケットを同一実行コードで受信する) をサポートするために the_twelite.network2 を追加
* NWK_LAYERED を追加 (現時点では親機受信のみ対応)
* MWXの初期化時にアプリケーションのバージョンを設定する MWX_Set_Usder_App_Ver() 関数を導入
* mwx::pnew() を追加し配置newの記述の簡素化
* EASTLのサポート追加
* EASTL用のnew[]演算子の追加
* MWXのソースコードのほとんどをプリコンパイルし、コンパイルの高速化を図った
* DIOイベントが無関係なポートに引き渡されていたのを修正

## twesettings - 0.2.6

MWSDK2021_09より変更なし。コンパイル済みのライブラリは 2.5 のまま。ソースコードのみ。



## TWENET C - 1.3.05 (L1305)

MWSDK2021_09より変更なし。

# MWSDK2021_12

TWELITE ARIA 対応に伴う修正、他。



## TweApps

### App_PAL (1.3.4)

* TWELITE ARIA 向けの対応を追加



### App_UART (1.4.6)

* 末尾にNUL文字を出力していたが、出力しないように修正
* 出力のカスタマイズで `>` (チェックサム範囲終了)が動作するようになった。
  * 例えば`MSG;<*>;%X\\n`と設定すると送信文字列 "`HELLO`" に対して "`MSG;HELLO;nn<改行>`" (nnは`HELLO`のXORチェックサム) が出力される。




### App_ARIA (1.0.0)

* 新規追加



### App_ARIA_OTA (1.0.0)

* 新規追加



### App_Wings (1.1.6)

* 親機が App_Wings でない場合に、中継機設定の App_Wings が中継しない問題を修正。



## Act_samples

### BRD_ARIA

スリープを伴う間欠駆動をしながら、計測、無線送信を行うサンプル。



### Unit_brd_ARIA

ボードビヘイビア `ARIA` に含まれる SHT4x センサオブジェクト `.sns_SHT4x` を利用し、センサーのみを動作させるサンプル。



## mwx - 0.1.9

* TWELITE ARIA 対応の追加
  * ボードビヘイビア `<ARIA>` (`BrdARIA`クラス) の追加
  * センサー定義 `SnsSHT4x` の追加
* インタラクティブモード設定 `<STG_STD>` のメニューの組み込み機能 (Extra Menu) が動作するように修正（※ mwx0.1.8 ではインタラクティブモード表示中に Serial 経由の出力を自動的に抑制するようにしたが、組み込み機能のコード中からも出力できるように手続きを用意した）



## twesettings - 0.2.6

MWSDK2021_09より変更なし。コンパイル済みのライブラリは 2.5 のまま。ソースコードのみ。



## TWENET C - 1.3.05 (L1305)

MWSDK2021_09より変更なし。



# MWSDK2021_09

* 各サンプルのVS Code対応を強化 
  * TWELITE STAGE から環境変数を設定し、STAGE経由でVS Codeを開いた場合には、特別の名設定なしに、VS Code内のコード解釈やビルド実行を行えるようにした。



## TweApps

### App_CUE (1.0.3)

* スリープ時間を61秒以上に設定すると発生する問題を修正



### App_IO (1.3.2)

* UARTコマンド処理で８ポート分までしか反映されない問題を修正



### App_PAL (1.3.3)

* OTAメッセージの調整
* MC3630加速度センサーの初期化手続きの時間待ちを確実に行うようにした
* MC3630の加速度センサー割り込み時のしきい値を設定で変更できるようにした



### App_Tag (2.2.4) (2525A)

* ADXL345 加速度センサーでスリープ復帰時ごとのセンサー値を得る場合、適切なセンサー値を得られない場合があったため、複数サンプル取得して不適切なサンプルの影響を除外した。
* パケット重複除去のためのDUPCHKモジュールに明示的に適切なパラメータを与えるようにした
* MAX31855のセンサー取得のための待ち処理を明示的に行うようにした



### App_UART (1.4.1 -> 1.4.5)

* TWELITE UART専用ビルドの追加
* 高頻度に送信するときにパケットIDが想定外に一巡して重複チェッカによりパケットが除去される場合があったが、オプションビット0x0000:4000の設定で高頻度向けの設定にするようにした
* モード E を追加しこれをデフォルトとした。従前のモードD（透過モード）では、到着したパケットのアドレス情報など無線パケットより得られる情報を得ることが出来なかったが、出力に諸情報をカスタマイズ可能な形で含められるようにした



## Act_samples

* 以下のように順を追って理解しやすいようにサンプルを整備した
  * act0..4
  * Scratch (シリアル入力で送信)
  * Slp_Wk_and_Tx (スリープ→起床→送信)
  * Parent_MONOSTICK (受信)
  * ...
* インタラクティブモード対応サンプルを拡充
* `on_rx_packet()`及び`on_tx_comp()`コールバック関数に対応
* `Parent_MONOSTICK`サンプルの強化、パケット内容を解釈してセンサー値などを表示
* スリープを行うサンプルでは`SM_SIMPLE`ステートマシンを用いて`loop()`内を完結に記述
* `Unit_Cue_MagBuz`サンプル（磁石が離れると圧電ブザーを鳴らす）の追加
* `Unit_doint-bhv`サンプル（ビヘイビアを用いたDIO割り込み検出）の追加
* サンプル名のリネーム
  * `PAL_MOT` -> `PAL_MOT-fifo`
  * `PAL_MOT-oneshot` -> `PAL_MOT-single`
  * `PAL_AMB-behave` -> `PAL_AMB_bhv`



## mwx - 0.1.8

* `Serial1`のポート、代替ポートの定義が適切でなかった
* `Serial`(UART0)のボーレートを変更できるようにした
* 受信パケット(`on_rx_packet()`)、送信完了(`on_tx_comp()`)を知らせるイベントコールバックを追加
  * コールバック関数の定義をしなければ従前の手続きも利用可能
* `<STG_STD>`インタラクティブモード設定の定義ID間違いや一部デフォルト値の変更など
* `<STG_STD>`インタラクティブモード設定でAppIDに加えて、チャネルと論理デバイスIDのデフォルト値を変更できるようにした
* `the_twelite` と `<NWK_SIMPLE>` オブジェクトの設定を、一部の設定についてインタラクティブモード`<STG_STD>` オブジェクトで行えるようにした
* `<NWK_SIMPLE>`で再送回数のデフォルト値を設定できるようにした
* `<STG_STD>` インタラクティブモードの画面が出ている間はアプリケーションからの`Serial`(UART0)の入出力を行わないようにした
* `CUE::PIN_SET`, `PAL???"":PIN_SET`を追加 (`PIN_BTN`はボタンのないCUEで`PIN_BTN`を用いるのは不自然であるため)
* `random()`の名前空間を`mwx::`に移動 (グローバル名にエリアス)
* MONOSTICKのウォッチドッグ設定を32ms単位で行うようにした



## twesettings - 0.2.6

変更なし。コンパイル済みのライブラリは 2.5 のまま。ソースコードのみ。



## TWENET C - 1.3.05 (L1305)

* チャネルマネージャ使用時に、最初のチャネル変更周期までは無線動作していない問題を修正
* チャネル切替が失敗したときにチャネル切り替え保留を示す変数を０クリアしないように、64ms以内に再度チャネル切り替えするようにした（現状はチャネルは切替可能までポーリング待ちをしているため、保留動作はしないため、動作に変化は想定しないが実装上自然な処理にした）
* Macの初期化関数（初期化済みなら何もしないが僅かながらのオーバーヘッドになりうる）を頻繁に呼び出していたため、これを抑制した
* `ByteQyeue`の割り込み処理を厳格化した
* ハードウェア割り込みがほぼ同時に複数発生しハードウェアイベントキューに複数イベントが有る場合、イベントキューの後ろにあるイベントが遅延する可能性があった（先頭だけ処理して一旦処理ループを抜ける）





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

