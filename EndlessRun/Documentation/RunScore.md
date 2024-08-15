# RunScore クラスの概要

`RunScore` クラスは、ゲームプレイ中のUIにおいて、プレイヤーが獲得したコイン数や走行距離を表示するためのクラスです。このクラスは、ゲームモードから取得したデータを `TextBlock` にバインドして、リアルタイムでプレイヤーに情報を提供します。

## 主な処理内容
- **プロパティのデフォルト値設定**: `TextBlock` ウィジェットに表示する値の初期設定を行います。
- **`TextBlock` に値をバインド**: プレイヤーのコイン数や走行距離をゲームモードから取得し、UIのテキストとしてリアルタイムに表示します。

## 関数の説明

### Initialize 関数

- **概要**: `Initialize` 関数はウィジェットが初期化される際に呼び出される関数で、`TextBlock` に表示するテキストのデリゲートを設定します。
- **バインドの設定**: `TextBlock_CoinsCount` と `TextBlock_MileageMeter` の `TextDelegate` に、それぞれコイン数と走行距離を設定する関数 (`SetTextBlockCoinsCount` と `SetTextBlockMileageMeter`) をバインドしています。これにより、テキストの表示内容が動的に更新されます。

### SetTextBlockCoinsCount 関数

- **概要**: この関数は、プレイヤーが獲得したコイン数をテキストとして取得し、`TextBlock_CoinsCount` に表示します。
- **ゲームモードからコイン数を取得**: `ARunGameMode` クラスからコインの総数を取得し、`UKismetTextLibrary::Conv_IntToText` 関数を使用して、`FText` に変換します。この変換されたテキストが `TextBlock_CoinsCount` に表示されます。

### SetTextBlockMileageMeter 関数

- **概要**: この関数は、プレイヤーが走行した距離をテキストとして取得し、`TextBlock_MileageMeter` に表示します。
- **走行距離の取得と変換**: `ARunGameMode` クラスから総走行距離を取得し、`UKismetMathLibrary::FTrunc` 関数を使って整数部分に切り捨て、`UKismetTextLibrary::Conv_IntToText` 関数でテキストに変換します。このテキストが `TextBlock_MileageMeter` に表示されます。

