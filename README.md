# MWSDK_COMMON
Include the libraries and various source code necessary for application development from the TWELITE STAGE SDK, but not the TWELITE STAGE app, toolchain, etc.<br>TWELITE STAGE SDK のうち、アプリケーション開発に必要なライブラリや各種ソースコードを含めます。TWELITE STAGE アプリやツールチェインなどは含みません。

Please refer to [Revision History](ReleaseNotes_en.md) for details of changes for each release.<br>
リリースごとの変更内容については[改訂履歴](ReleaseNotes_jp.md)を参照ください。



## Folders / フォルダ

The following is a list of the main folders included in this repository.<br>本レポジトリに含まれる主要フォルダを列挙します。



* ChipLib - Silicon Chip library
* LICENSE - License files
* MkFiles - Make files
* TWENET - TWENET libraries
* Act_samples - Sample code (ACT)
* Act_extras - Sample code, advanced or complicated (ACT)
* Wks_TweApps - Source code of  TWELITE Apps.



## Not included / 含まれないもの

The following is a list of items not included in this repository. <br>本レポジトリに含まれないものを列挙します。



* TWELITE STAGE App / TWELITE STAGE アプリ
* Tools
* BIN 


## Updating MWSDK folder / MWSDK フォルダの更新方法

[Get the latest version - MWSDK(TWELITE SDK)](https://sdk.twelite.info/v/en/latest) / [最新版の取得 - MWSDK(TWELITE SDK)](https://sdk.twelite.info/latest)



## LICENSE / ライセンス

Unless otherwise noted, the MWSLA-1 (Monowireless Software License Agreement) applies to our software.<br>
特に断りがない限り、当社製のソフトウェアについては MWSLA-1 (モノワイヤレスソフトウェア使用許諾契約書) が適用されます。

Software not made by us, such as libraries, gcc and other toolchains, and bash (mingw) made by semiconductor manufacturers, are subject to the licenses associated with them. Please refer to the comments in each individual source code or the license description of the distributor.<br>
半導体メーカー製のライブラリ・gccなどのツールチェイン・bash (mingw) といった、当社製ではないソフトウェアについては、これらに付随するライセンスに従います。各個別のソースコードのコメントや、配布元のライセンス記述を参照してください。



## Others / その他

### (Windows) WIN_BASH.cmd

This is a batch file to open the bash command prompt screen after setting the environment variable `MWSDK_ROOT` to the folder where `WIN_BASH.cmd` is stored.

`WIN_BASH.cmd`が格納されるフォルダを環境変数 `MWSDK_ROOT`としたうえで bash コマンドプロンプト画面を開くためのバッチファイルです。Windows 上でコマンドラインビルドを行う場合に利用してください。
