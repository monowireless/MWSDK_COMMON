# MWSDK_COMMON_SNAP 改訂履歴

各ライブラリの主要な修正箇所を記載しています。

以下の個別ライブラリについても参照ください。

* mwxライブラリ ([解説](https://mwx.twelite.info/)|[レポジトリ](https://github.com/monowireless/mwx))
* twesettingsライブラリ ([レポジトリ](https://github.com/monowireless/twesettings))

バージョン名はリリース年と月を含めた MWSDK{YYYY}_{MM} となっています。UNOFFICIAL が付くものは、リリースのための包括的な検証作業を省略した適宜版となります。



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

