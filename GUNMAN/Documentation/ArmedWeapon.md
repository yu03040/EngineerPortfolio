# ArmedWeapon Source Code
このドキュメントでは、ArmedWeapon のソースコードの主な部分を簡単に説明します。

## Rifle Class
[Rifle](/GUNMAN/Source/GUNMAN/ArmedWeapon/Rifle.cpp) は武器を定義している。  

## Rifle Ammunition Class
[Rifle Ammunition](/GUNMAN/Source/GUNMAN/ArmedWeapon/RifleAmmunition.cpp) は弾薬の動作を定義している。

### ゲーム開始処理（BeginPlay）
- 弾薬が生成されたときに、銃から弾薬を飛ばす

## Weapon Data Table
[WeaponStructure.h](/GUNMAN/Source/GUNMAN/ArmedWeapon/WeaponStructure.h) は武器情報のデータテーブル（DT_Weapon）を定義している。  
※データテーブルは構造体の集まり。

### DT_Weapon
- 武器はライフル、ピストル、ショットガンの 3 種類
	