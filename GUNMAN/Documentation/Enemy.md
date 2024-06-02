# Enemy Source Code
このドキュメントでは、Enemy のソースコードの主な部分を簡単に説明します。

## AI Enemy Class
[AI Enemy](/GUNMAN/Source/GUNMAN/Enemy/AIEnemy.cpp) は敵キャラクターの動作を定義している。  
このクラスから巡回ルートごとに敵を派生(BP_PathAAIEnemy, BP_PathBAIEnemy, BP_RandomAIEnemy)

### GUNMAN Character Constructor
- 敵キャラクターの初期化

### ゲーム開始処理（BeginPlay）
- 敵からダメージを受けたときダメージ処理
- 敵の頭上にHPバーを表示

## AI Enemy Controller Class
[AI Enemy Controller](/GUNMAN/Source/GUNMAN/Enemy/AIEnemyController.cpp) はゲーム内のAIキャラクターの制御を定義している。  
- 以下のロジックを基に BehaviorTree(Patrol AI) を構築
	- 受け取ったアクターの情報を更新
	- プレイヤーが視界に存在するか情報を更新