# EndlessRun - LevelScriptActor クラス説明

## LevelScriptActor クラスの概要

- **レベル構成**: EndlessRunには、「TitleMap」と「RunGameMap」という二つのレベルがあります。
- **C++クラス**: これらのレベルのレベルブループリントをC++で実装しています。特に、TitleMapのレベルブループリントを`ATitleMap`クラスとして実装しています。
- **ソースファイル**: 
  - TitleMapのソースファイルとヘッダファイルはそれぞれ "TitleMap.cpp" と "TitleMap.h" です。
  - RunGameMapのソースファイルも同様に "RunGameMap.cpp" と "RunGameMap.h" ですが、こちらは特に使用していません。
- この説明では`TitleMap`クラスに焦点を当てます。

## TitleMap クラスのソースコードの内容

`ATitleMap`クラスでは、主に以下の二つの処理を行っています。

1. **タイトルメニューの表示**: ゲームの開始時にタイトルメニューを表示します。これは、UMGを使用して作成されたウィジェットを通じて実装されています。
2. **ゲーム開始時のイベント処理**: ゲーム開始時に、動的マルチキャストデリゲート（`DECLARE_DYNAMIC_MULTICAST_DELEGATE`マクロ）を用いて、Blueprintのイベントディスパッチャーと同等の処理を行います。