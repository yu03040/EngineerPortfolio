#pragma once
#include<functional>
#include "Base.h"

namespace game
{
	namespace scene
	{
		// ゲームシーン
		class Game : public Base
		{
		public:
			// コンストラクタ
			Game() :Base(State::Title) {}

			// デストラクタ
			~Game() = default;

			// インターフェースの実装
			void initialize() override;
			void process() override;
			void finalize() override;

		private:

			// 内部State
			enum class SelfState
			{
				None					// 初期状態
				, SelectRandomValue		// ランダムな数値を設定
				, WaitInput				// 入力待ち
				, CheckResult			// 結果確認
				, End					// 終了

				, SelfStateCount		// 要素数
			};

			// 現在の内部State
			SelfState selfState_ = SelfState::None;
			void setSelfState(const SelfState selfState);

		private:

			// 内部処理
			void processNone() {} // Endも兼ねる
			void processSelectParentValue();
			void processWaitInput();
			void processCheckResult();

			// 内部処理格納用の関数オブジェクト
			function<void()> processFunction_;

		private:

			// 該当の数字より大きいか小さいか
			enum class BigOrSmall
			{
				None

				, Big
				, Small

				, InputCount
			};

			// プレイヤーの選択
			BigOrSmall childValue_ = BigOrSmall::None;

			// 生成する値の範囲
			static const int kMin = 0;
			static const int kMax = 9;

			// Big Or Small 判定の境界値
			static const int kBigThreshold = 5;

			// 判定
			BigOrSmall checkValue(const int value) const
			{
				return (kBigThreshold <= value) ? BigOrSmall::Big : BigOrSmall::Small;
			}
		};
	}
}


