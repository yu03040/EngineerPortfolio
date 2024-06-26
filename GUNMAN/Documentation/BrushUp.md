# ブラッシュアップした箇所

GUNMMAN のブラッシュアップした箇所について一つずつまとめていく。  
1 ～ 9 までは 4 月あたりの変更点で、それ以降の番号は 5, 6 月に変更した。

## 1.プレイヤ―の各武器の攻撃力を変更

今まではプレイヤーの攻撃力は 10 で一定  
それだと連射性能が一番高いライフルしか実用性がない。

面白くないので、 武器の攻撃力(WeaponATK)の変数を作り、連射性能が低いピストルやショットガンは攻撃力を高くし、連射性能が高い TPS と FPS のライフルは攻撃力を低くした。

これで、ゲームに戦略性を持たせることができた。

|               ライフル(TPS)               |           ショットガン(TPS)           |            ピストル(TPS)            |               ライフル(FPS)               |
| :---------------------------------------: | :-----------------------------------: | :---------------------------------: | :---------------------------------------: |
| ![Rifle_TPS](Images/Weapon_Rifle_TPS.png) | ![Shotgun](Images/Weapon_Shotgun.png) | ![Pistol](Images/Weapon_Pistol.png) | ![Rifle_FPS](Images/Weapon_Rifle_FPS.png) |
|                5 ダメージ                 |              15 ダメージ              |             10 ダメージ             |                5 ダメージ                 |

[1.ソースコード（TPS）](https://github.com/yu03040/EngineerPortfolio/blob/main/GUNMAN/Source/GUNMAN/GUNMANCharacter.cpp#L378)  
[2.ソースコード（FPS）](https://github.com/yu03040/EngineerPortfolio/blob/main/GUNMAN/Source/GUNMAN/GUNMANCharacter.cpp#L305)

## 2.敵の攻撃力を変更

プレイヤーと同様に敵も攻撃力が 10 で一定  
それだと自分の体力が 1000 もあるため、ダメージが低く面白くない。  
そこで、各敵の攻撃力を上げてゲームに緊張感を持たせました。

| BP_EnemyAI_Random | BP_EnemyAI_Path_A | BP_EnemyAI_Path_B |
| :---------------: | :---------------: | :---------------: |
| ランダムに動く敵  | 巡回路 A を動く敵 | 巡回路 B を動く敵 |
|   100 ダメージ    |    60 ダメージ    |    50 ダメージ    |

※ これらのブループリントは基底クラスとして [AIEnemy](../Source/GUNMAN/Enemy/AIEnemy.cpp) クラスをもつ。

## 3.敵の射程距離を変更

今までは敵がプレイヤーに近い位置(1 メートル)にならないと攻撃してこなかった。  
逃げながら戦えばプレイヤーの体力が減ることはほとんどない。

そこで、ゲームの難易度をあげるために 10 メートル離れた位置から攻撃してくるように変更した。

## 4.敵の色を変更

敵の色を黒色で統一していた。  
すべての敵が同じルートを巡回するわけではなく、攻撃力も違うので変更することにした。  
これで、攻撃力が大きい敵を識別できるので戦略が立てられるようになった。

|          BP_EnemyAI_Random           |          BP_EnemyAI_Path_A          |          BP_EnemyAI_Path_B          |
| :----------------------------------: | :---------------------------------: | :---------------------------------: |
|           ランダムに動く敵           |          巡回路 A を動く敵          |          巡回路 B を動く敵          |
| ![Random](Images/EnemyAI_Random.png) | ![PathA](Images/EnemyAI_Path_A.png) | ![PathB](Images/EnemyAI_Path_B.png) |

## 5.敵が建物に隠れていてもシルエットが見えるようにを変更

敵に変更を加えてきたことでゲームの難易度が上がり、  
クリアしづらいので、敵の位置をどこからでもわかるようにシルエットが見えるよう変更した。

これで、敵の位置が把握しやすくなったのでちょうどいい難易度のゲームにできた。

![Silhouette](Images/Enemy_AI_Silhouette.png)

この画像のように敵が灰色のシルエットとして表示される。  
ゲームを開始して描画が終わった後に、テクスチャに対して後処理(ポストプロセス)をして追加効果(シルエット表示)をつけた。

## 6.敵の体力バー(UI)が 100 分率にならない不具合を修正

敵の体力は 30 だったのにもかかわらず、  
Health を 100 で割っていたので UI 表示がおかしくなっていた。

修正したことで、敵の体力を把握しやすくなり、戦いやすい。

[UIEnemy クラスのソースコード](../Source/GUNMAN/UMG/UIEnemy.cpp#L31)

## 7.敵の出現位置が巡回ルート上にない不具合の修正

今までは、ゲームが始まって敵が生成される位置と巡回ルートの開始位置は離れていた。  
そのため、敵が巡回ルートの開始位置まで移動し、そこから巡回ルート上を移動していた。

しかし、これだと巡回ルートの開始位置に行く途中でプレイヤーを見つけることがあり、巡回ルートが意味がない。

そこで、巡回ルートの開始位置のそばから敵が生成されるようにレベル上のアクターの配置を変更した。

## 9.フィールドの色味を変更

フィールドの明るさがバラバラなところが多く不自然に感じたので、ライトの調整をした。  
原因はポイントライト(球体状の光)やスポットライト(コーン状の光)を多用していたからだった。

|                      修正前                      |                     修正後                     |
| :----------------------------------------------: | :--------------------------------------------: |
| ![BattleMap_Before](Images/BattleMap_Before.png) | ![BattleMap_After](Images/BattleMap_After.png) |

修正前の画像を見てもらえばわかるように天井は照らされているが、他の場所は暗くなっている。  
そのため、ディレクショナルライト(太陽光)とスカイライト(環境光)のふたつで全体を明るくし、他のライトは削除した。

また、天井があり空間がひとかたまりのメッシュになっているので、光が透過されない。  
そこで、天井のライティングの「cast shadow」をオフにして光を透過させるようにした。

## 10.チュートリアル画面の操作方法説明を変更

操作方法の説明が文字だけとなっており、ぱっと見て分かりにくい。  
そこで、ゲームコントローラーの画像を基に説明するように変更した。

|                         修正前                          |                        修正後                         |
| :-----------------------------------------------------: | :---------------------------------------------------: |
| ![Tutorial_Before](Images/TitleMap_Tutorial_Before.png) | ![Tutorial_After](Images/TitleMap_Tutorial_After.png) |

このようにすることで、操作方法がイメージしやすい。

## 11.プレイヤーの UI と制限時間の UI を分けて管理するように変更

制限時間はゲームのルールに関係するため、ゲームモードで管理する方が正しいと考えた。

|               GANMANCharacter の UI               |            GUNMANGameMode の UI            |
| :-----------------------------------------------: | :----------------------------------------: |
| ![Tutorial_Before](Images/UI_Character_After.png) | ![Tutorial_After](Images/UI_TimeLimit.png) |

プレイヤーの UI は体力と倒した数にし、ゲームモードの UI は制限時間にした。

## 12.生ポインタを TObjectPtr に変更

UPROPERTY 変数の生ポインタ（ \* ）は直接メモリアドレスを扱うが、ガベージコレクションとは互換性がない。

メモリ管理は完全に開発者の責任になる。

従って、TObjectPtr を使用して自動的にメモリ管理を行い、  
既存の UE ポインタ型よりもシンプルかつ安全に UObject を扱う。

## 13.getter, setter メソッドを使用するように変更

Ugetter および setter メソッドを使用することで

- データカプセル化
- プロパティの変更監視
- デバッグの容易化
- 内部実装の隠蔽
- Blueprint との統合

など、多くのメリットが得られる。  
これにより、コードの保守性や安全性が向上し、開発プロセスがより効率的かつ効果的になった。

## 14.敵を無限生成から指定数分生成に変更

同じことを繰り返しても飽きると思ったので、指定数分だけ生成するようにした。

また、敵の攻撃力の変更と敵の射程距離の変更により、難易度が上昇していることもあり、  
敵を 10 体倒せばクリアにした。

※元々、ブループリント

- BP_EnemyTargetPoint_Path_A
- BP_EnemyTargetPoint_Path_B
- BP_EnemyTargetPoint_Random

に書かれていたものだが基底クラス EnemyTargetPoint の方に書いておき、  
レベルに配置するだけにした。

[EnemyTargetPoint クラスのソースコード](../Source/GUNMAN/Enemy/EnemyTargetPoint.cpp)

## 15.武器と弾薬のクラスのファイル名を変更

最初は武器をライフルだけ実装するつもりだったので、Rifle クラスと RifleAmmunition クラスを作った。  
しかし、ピストルやショットガンを実装したため、Rifle クラスと RifleAmmunition クラスが基底クラスだと不明瞭だった。

そこで、基底クラスの名前を Weapon に変更し、そこから派生クラスをブループリントで作成した。

- BP_Rifle
- BP_RifleAmmunition
- BP_ShotGun
- BP_ShotGunAmmunition
- BP_Pistol
- BP_PistolAmmunition

これで、一貫性を保ちコード全体の整合性を失わずに済んだ。  
また、可読性も上がり、初めてコードを見る方にも理解しやすくなった。

[Weapon クラスのソースコード](../Source/GUNMAN/ArmedWeapon/Weapon.cpp)  
[WeaponAmmunition クラスのソースコード](../Source/GUNMAN/ArmedWeapon/WeaponAmmunition.cpp)

## 16.入力を Input Event から Enhanced Input に変更

Unreal Engine 4 から開発を始めため Input Event のままになっていた。  
しかし、Enhanced Input は従来よりも柔軟に入力を管理できるので変更した。

### Input MappingContext(IMC)

バトル中の入力と UI 操作の入力を分けグループ化

- IMC_Default（バトル中の入力）
- IMC_Map（UI 操作の入力）

### Input Action(IA)

各種の入力を定義

- IMC_Default には以下の入力を設定

  - IA_Jump（ジャンプ）
  - IA_MoveForward, IA_MoveRight（移動）
  - IA_Run（走る）
  - IA_Look（視点移動）
  - IA_Fire（攻撃）
  - IA_ReadyGun（武器を構える）
  - IA_Toggle（FPS/TPS 切り替え）
  - IA_SwitchAndEquipWeapons（武器切り替え）
  - IA_PauseMenu（ポーズメニューを開く）

- IMC_Map には以下の入力を設定
  - IA_Enter（決定）
  - IA_UpArrowKey（上移動）
  - IA_DownArrowKey（下移動）

## 17.プレイヤーと敵のアニメーション BP での更新処理を C++ に移行

アニメーション BP の Event Blueprint Update Animation よりも UAnimInstance クラスを継承して  
Native Update Animation を C++ コードでオーバーライドした方が付加的な処理が少なく、高速であるので移行した。

その他のステートマシンなどの AnimGraph は継承したアニメーション BP で実装している。

[GUNMANAnimInstance クラスのソースコード](../Source/GUNMAN/GUNMANAnimInstance.cpp)  
[EnemyAnimInstance クラスのソースコード](../Source/GUNMAN/Enemy/EnemyAnimInstance.cpp)
