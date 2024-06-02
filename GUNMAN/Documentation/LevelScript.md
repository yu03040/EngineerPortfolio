# Level Script Source Code
このドキュメントでは、Level Script のソースコードの主な部分を簡単に説明します。

## Base Map Script Class
[Base Map Script](/GUNMAN/Source/GUNMAN/LevelScript/BaseMapScript.cpp) は基本的なレベルを定義している。  
- ゲームコントローラーで UI 操作を可能に
 

## Title Map Script Class
[Title Map Script](/GUNMAN/Source/GUNMAN/LevelScript/TitleMapScript.cpp) はタイトル画面のレベルを定義している。  
このクラスは Base Map Script を継承している。
- フォーカスしている UI の色を変える
- UI を押したとき、それぞれの出力
	- ゲーム開始
	- ゲーム終了
	- 操作説明を開く

## Battle Map Script Class
[Battle Map Script](/GUNMAN/Source/GUNMAN/LevelScript/BattleMapScript.cpp) はバトル画面のレベルを定義している。  
このクラスは Base Map Script を継承している。
- フォーカスしている UI の色を変える
- UI を押したとき、それぞれの出力
	- タイトル画面に戻る
	- ゲームに戻る
	- ゲーム終了
- フォーカス位置の初期化

## GameClear Map Script Class
[GameClear Map Script](/GUNMAN/Source/GUNMAN/LevelScript/GameClearMapScript.cpp) はゲームクリア画面のレベルを定義している。  
このクラスは Base Map Script を継承している。
- フォーカスしている UI の色を変える
- UI を押したとき、それぞれの出力
	- コンティニュー
	- ゲーム終了

## GameOver Map Script Class
[GameOver Map Script](/GUNMAN/Source/GUNMAN/LevelScript/GameOverMapScript.cpp) はゲームオーバー画面のレベルを定義している。  
このクラスは Base Map Script を継承している。
- フォーカスしている UI の色を変える
- UI を押したとき、それぞれの出力
	- コンティニュー
	- ゲーム終了