# WeaponStructure 構造体の概要

## 主な処理内容

`WeaponStructure` 構造体は、武器に関連するさまざまな情報を格納するためのデータ構造です。この構造体は、特に武器に関するソケットや音、発砲時のエフェクト、弾薬クラスなど、武器に関連する重要なプロパティを保持します。

- **武器のアタッチに関する情報**: 武器をどのソケットにアタッチするかなどの情報を含んでいます。
- **武器のサウンドとエフェクト**: 装備時や発射時に使用される音や発砲時のエフェクトも定義されています。
- **データテーブルとしての利用**: この構造体は `WeaponDataTable` としてデータテーブル形式で他のクラスから参照され、武器のカスタマイズや挙動を制御します。

## 基本構造

`FWeaponStructure` は `FTableRowBase` を継承しており、データテーブルとして使用できるように設計されています。`USTRUCT` マクロにより、構造体が `BlueprintType` として指定されており、Blueprint からも利用できるようになっています。

この構造体は、武器に必要なビジュアル、サウンド、エフェクト、弾薬に関する情報をまとめて保持しており、武器の挙動や見た目、発射時の動作などを統一的に管理するために設計されています。

### 各プロパティの説明

- **AttachSocketName**: 
  - 武器をキャラクターにアタッチする際に使用するソケットの名前を指定します。アニメーションシーケンスとして使用される例では `ThirdPersonidle` があり、ここでのデフォルト値は `"None"` です。
  
- **EquipSocketName**: 
  - 装備時に武器を接続するためのソケット名を指定します。この場合、右手 (`Weapon_hand_rソケット`) に装備される設定です。

- **EquipmentNoise**: 
  - 武器を装備するときに再生される音を指定します。`USoundBase` 型で、音の再生に使用されます。

- **HasPistol**: 
  - このフラグは、武器がピストルかどうかを判断するために使用されます。`bool` 型で、デフォルト値は `false` です。

- **GunshotSound**: 
  - 武器を発射した際の銃声を格納するためのプロパティです。`USoundBase` 型で、発射時にこの音が再生されます。

- **MuzzleFire**: 
  - 発射時の発砲炎のエフェクトを指定します。`UParticleSystem` 型で、発砲時の視覚的な効果を定義しています。

- **MuzzleFireSoketName**: 
  - 発砲炎が表示されるソケットの名前を指定します。デフォルトでは `"MuzzleFlash"` という名前になっています。

- **FiringMontage**: 
  - 武器を発射したときにキャラクターに再生されるアニメーションを指定します。`UAnimMontage` 型で、発砲アクションに関連する身体の動きを制御します。

- **AmmunitionClass**: 
  - 武器の弾薬クラスを指定します。このクラスは `AWeaponAmmunition` 型のサブクラスとして指定され、実際に武器から発射される弾薬のクラスを決定します。

- **AmmunitionSocketName**: 
  - 弾薬が出るソケットの名前を指定します。デフォルトでは `"AmmoEject"` という名前が設定されており、弾が排出される位置を定義しています。 