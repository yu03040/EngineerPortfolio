## クラスの概要

### ResultMenu クラス
**ResultMenu** クラスは、ゲームが終了した際に表示されるリザルトメニューのUIを管理します。主な機能として、プレイヤーがリザルト画面を通じてランキングを確認することができるようにし、ゲームを一時停止して入力を制限し、またマウスカーソルを表示する処理を行っています。

### Ranking クラス
**Ranking** クラスは、リザルトメニュー内でランキングのデータを表示するためのウィジェットです。ユーザーがリザルト画面でランキングを確認する際に使用され、ランキングデータの表示や、特定の操作（例えば、タイトル画面に戻る）の処理を担当します。

### RankingItem クラス
**RankingItem** クラスは、ランキングの個々のエントリーを表示するためのウィジェットです。ランキングの順位、スコア、距離などを表示し、これらのデータを設定するための関数が含まれています。

## 関数の説明

### ResultMenu クラス

1. **`NativeConstruct` 関数**
   - `NativeConstruct` はウィジェットが表示された際に自動的に呼び出される関数です。ここでは、ゲームの一時停止、入力の制限、マウスカーソルの表示を行い、UI操作を可能にします。
   - `DisplayRanking` 関数を呼び出し、ランキングのウィジェットを表示します。

2. **`DisplayRanking` 関数**
   - ランキングを表示するために必要なウィジェットクラスを取得し、`RankingWidget` を生成します。
   - 生成した `RankingWidget` を `Ranking_ScaleBox` に追加し、`SetUpRankingData` 関数を使用してランキングデータを設定します。

### Ranking クラス

1. **`NativeConstruct` 関数**
   - `NativeConstruct` では、タイトル画面に戻るボタン (`BackToTitle_Button`) にクリックイベント (`OnClickedBackToTitle_Button`) を関連付けます。

2. **`OnClickedBackToTitle_Button` 関数**
   - タイトル画面に戻るボタンがクリックされたときに実行される処理で、クリック音を再生し、0.5秒の遅延の後にタイトル画面へ遷移します。

3. **`SetUpRankingData` 関数**
   - 現在のランキングデータを表示し、全プレイヤーのランキングデータをソートしてベスト5を表示します。また、`GameMode` を使用してランキングデータを保存します。

4. **`DisplayRankingItemInVerticalBox` 関数**
   - 全プレイヤーのランキングデータをループ処理で表示し、`RankingList_VerticalBox` に各プレイヤーのランキングアイテムを追加します。

5. **`DisplayRankingItemInScaleBox` 関数**
   - 現在のプレイヤーのランキングデータを表示し、`YourScore_ScaleBox` に追加します。

### RankingItem クラス

1. **`SetUpRankingItem` 関数**
   - この関数では、受け取った `RankingData` に基づいて、順位 (`Rank`)、スコア (`Score`)、距離 (`Distance`) を表示します。
   - `RankSelect` 関数を使用して、順位が有効かどうかを確認し、それに応じて順位を表示します。

2. **`RankSelect` 関数**
   - `RankSelect` 関数は、条件に基づいてテキストオプションを選択するユーティリティ関数です。順位が正しい場合に順位を表示し、そうでない場合は空のテキストを返します。