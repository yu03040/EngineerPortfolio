namespace game
{
	namespace scene
	{
		enum class State
		{
			None			// 初期化

			, Title			// タイトル
			, Game			// ゲーム中
			, Win			// 結果表示(勝ち)
			, Lose			// 結果表示(負け)
			, End			// ゲーム終了

			, SceneCount	// 要素数
		};
	}
}