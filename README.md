# MWSDK_COMMON

MWSDK(Win/macOS/Linux) から各ライブラリ、サンプルなど共通部を切り出しています。このレポジトリはMWSDKリリース後の修正などが含まれます。必要に応じて適用してください。



## ZIP ダウンロードについて

Github 上のZIPダウンロードも同様にサブモジュールに含まれるファイルは含まれません。以下のレポジトリより別々にダウンロードしてください。

* ライブラリ
  https://github.com/monowireless/MWSDK_COMMON_SNAP
* Act サンプルディレクトリ
  https://github.com/monowireless/Act_samples
* TWEAPPS (以下リンク先のレポジトリ一覧を開いて App??? Samp??? を個別に参照してください)
  https://github.com/monowireless?tab=repositories
* Act サンプル追加分 (以下リンク先のレポジトリ一覧を開いて ActEx??? を個別に参照してください)
  https://github.com/monowireless?tab=repositories



## Gitのクローンについて

本レポジトリは、サブモジュールを用いています。オプションを設定せずにクローンした場合は、サブモジュールのディレクトリはクローンされません。



サブモジュールも同時にクローンするコマンドを実行してください。

```
# クローンとサブモジュールを同時に実行する
git clone --recursive git://github.com/monowireless/MWSDK_COMMON.git
```

クローン後にサブモジュールをアップデートするには、以下のコマンドを実行します。

```
# クローン後にサブモジュールをアップデートする
git submodule update --init --recursive
```



## Gccツールチェインについて

MWSTAGE 配布パッケージに含まれます。



## 関連情報

* https://mono-wireless.com/jp/products/stage/

  TWELITE STAGE の紹介（MWSDKでの作業を簡単にするアプリケーションで、TWELITE STAGEの配布パッケージ内にMWSDKが含まれます）

* https://mwx.twelite.info/
  MWSDKに含まれるmwxライブラリの解説です。TWELITE 上でのアプリを簡易に記述することを目的としています。

* https://sdk.twelite.info/
  [参考] MWSDK単体の解説です。

