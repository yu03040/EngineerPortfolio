# アピールポイント

### 特に頑張ったところ：敵の AI

[Enemy Target Point](../Source/GUNMAN/Enemy/EnemyTargetPoint.cpp) を基底クラスとして

- BP_EnemyTargetPoint_Path_A
- BP_EnemyTargetPoint_Path_B
- BP_EnemyTargetPoint_Random

この三つの派生クラス（Blueprint）を作り、レベル上に配置する。

本来なら、置いた位置から敵が生成され指定のルートまたはランダムに巡回する。  
しかし、生成後はその場に棒立ちになってしまう。

原因は敵の基底クラスである [AI Enemy](../Source/GUNMAN/Enemy/AIEnemy.cpp) に問題があった。  
その問題は [AIEnemyController](../Source/GUNMAN/Enemy/AIEnemyController.cpp) がどのタイミングで特定の Pawn（[AI Enemy](../Source/GUNMAN/Enemy/AIEnemy.cpp)）を自動的に制御するかを決定するための列挙型である **EAutoPossessAI** が間違ったオプションになっていたこと。

#### 間違い

```C++
AutoPossessAI = EAutoPossessAI::PlacedInWorld;
```

#### 正解

```C++
AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
```

間違いのコードを見てもらえばわかるように  
レベルに配置された固定の Pawn に対して AI が有効になっていた。  
この設定では、レベルに配置された Pawn がゲーム開始時に自動的に AI に制御される。

しかし、元からレベルに配置した Pawn はいない。

そのため、正解のコードのように  
レベルに配置された Pawn とゲーム中にスポーンされた Pawn の両方が自動的に AI に制御される。  
どのような方法で生成された Pawn に対しても適用する。

結果、敵が指定のルートまたはランダムに巡回するようになり、  
自分が求めていた動作を確認できた。

これで、3 種類の敵を実装でき、ゲームのクオリティ向上に繋がった。  
この実装を通して、AI の難しさを実感した。ここで学んだことをドキュメントに残し、次また困ったときの参考にしたい。
