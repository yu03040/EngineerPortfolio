#pragma once
#include "Base.h"
#include<string>

namespace game
{
	namespace scene
	{
		class Result : public Base
		{
		public:
			// コンストラクタ
			/*
				引数なしコンストラクタは削除(結果が必ず入るので)
			*/
			Result() = delete;
			Result(const State state) : Base(state) {}

			// デストラクタ
			~Result() = default;

			// インターフェースの実装
			void initialize() override;
			void process() override;
			void finalize() override {};

		private:

			// 勝敗
			enum class WinOrLose
			{
				None

				, Win
				, Lose

				, WinOrLoseCount
			};

			// 勝敗結果メッセージ文字列格納用テーブル
			static const string kResultMessage[static_cast<int>(WinOrLose::WinOrLoseCount)];

			// 再挑戦するかのメッセージ
			static const string kRetryMessage;

			// 勝敗結果メッセージ取得
			const string& resultMessage() const;

		private:

			// 内部 State
			enum class SelfState
			{
				None				// 初期値

				, ShowMessage		// 勝敗結果メッセージ表示
				, WaitInput			// 入力待ち
				, End				// 終了

				, SelfStateCount
			};

			// 現在の内部State
			SelfState selfState_ = SelfState::None;
		};
	}
}


