# ArmedWeapon Source Code

このドキュメントでは、ArmedWeapon のソースコードの主な部分を簡単に説明します。

## Weapon Class

[Weapon](/GUNMAN/Source/GUNMAN/ArmedWeapon/Weapon.cpp) は武器を定義している。

## Weapon Ammunition Class

[Weapon Ammunition](/GUNMAN/Source/GUNMAN/ArmedWeapon/WeaponAmmunition.cpp) は弾薬の動作を定義している。

- 弾薬が生成されたときに、銃から弾薬を飛ばす  
  ※ 飛ばす処理は **BP_EnemyAI_Path_A**, **BP_EnemyAI_Path_B**, **BP_EnemyAI_Random** に実装

## Weapon Data Table

[WeaponStructure.h](/GUNMAN/Source/GUNMAN/ArmedWeapon/WeaponStructure.h) は武器情報のデータテーブル（DT_Weapon）を定義している。  
※データテーブルは構造体の集まり。

### DT_Weapon

- 武器はライフル、ピストル、ショットガンの 3 種類
