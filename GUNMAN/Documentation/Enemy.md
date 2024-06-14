# Enemy Source Code

このドキュメントでは、Enemy のソースコードの主な部分を簡単に説明します。

## AI Enemy Class

[AI Enemy](/GUNMAN/Source/GUNMAN/Enemy/AIEnemy.cpp) は敵キャラクターの動作を定義している。

- 敵からダメージを受けたときダメージ処理
- 敵の頭上に HP バーを表示
- 敵を見失ったときの AI の動き

このクラスから巡回ルートごとに敵を派生

- BP_EnemyAI_Path_A
- BP_EnemyAI_Path_B
- BP_EnemyAI_Random

それぞれの処理は…

- 武器のアタッチ
- プレイヤ―を追いかけるときは走り、巡回するときは歩く
- プレイヤ―を見つけたときと見失ったときブラックボードを更新
- 攻撃処理

## AI Enemy Controller Class

[AI Enemy Controller](/GUNMAN/Source/GUNMAN/Enemy/AIEnemyController.cpp) はゲーム内の AI キャラクターの制御を定義している。

- 以下のロジックを基に BehaviorTree(Patrol AI) を構築
  - 受け取ったアクターの情報を更新
  - プレイヤーが視界に存在するか情報を更新

## Enemy Target Point Class

[Enemy Target Point](GUNMAN/Source/GUNMAN/Enemy/EnemyTargetPoint.cpp) は敵を生成する場所と生成条件を定義している。  
出現条件：Max Enemy Count（生成させる敵の最大値）を超えるまで出現させる

- 例）`MaxEnemyCount = 5` なら 5 体、6 体目は出現しない

この基底クラスを基にブループリントで 3 つの派生クラスを作成

- BP_EnemyTargetPoint_Path_A：このクラスは敵が出現してから A のルートを巡回する
- BP_EnemyTargetPoint_Path_B：このクラスは敵が出現してから B のルートを巡回する
- BP_EnemyTargetPoint_Random：このクラスは敵が出現してからランダムに巡回する

A, B のルートは BP_PathPoint というブループリントで巡回してほしい座標を決定している。
