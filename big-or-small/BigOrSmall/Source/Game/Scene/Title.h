#pragma once
#include "Base.h"

namespace game
{
	namespace scene
	{
		class Title : public Base
		{
		public:

			// コンストラクタ
			Title() : Base(State::Title) {};

			// デストラクタ
			~Title() = default;

			// インターフェースの実装
			void initialize() override;
			void process() override;
			void finalize() override;

		private:
			// 内部State
			enum class selfState
			{
				None				// 初期状態
				, ShowTitle			// タイトル表示
				, WaitEnter			// 入力待ち
				, End				// 終了

				, SelfStateCount	// 要素数
			};

			// 現在の内部State
			selfState selfState_ = selfState::None;
		};

	}
}

