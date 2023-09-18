#include "Game.h"
#include<cassert>
#include "Source/Utility/Input.h"
#include "Source/Game/Gamedata.h"
#include "Source/Utility/Random.h"
#include "Result.h"

namespace game
{
	namespace scene
	{
		// 初期化
		void Game::initialize()
		{
			setSelfState(SelfState::SelectRandomValue);
		}

		// 更新処理
		void Game::process()
		{
			if (processFunction_)
			{
				processFunction_();
				if (selfState_ == SelfState::End)
				{
					if (GameData::instance().gameCount_ <= 5) // 5回勝負
					{
						setSelfState(SelfState::SelectRandomValue);
					}
				}
			}
		}

		// 終了処理
		void Game::finalize()
		{

		}

		// 内部処理
		/*
			SelfState によって processFunction_ を切り替える
		*/
		void Game::setSelfState(const SelfState selfState)
		{
			selfState_ = selfState;

			switch (selfState_)
			{
				case SelfState::SelectRandomValue:
					processFunction_ = bind(&Game::processSelectParentValue, this);
					break;
				case SelfState::WaitInput:
					processFunction_ = bind(&Game::processWaitInput, this);

					break;
				case SelfState::CheckResult:
					processFunction_ = bind(&Game::processCheckResult, this);
					break;
					// 何もしない process
				case SelfState::None:
					processFunction_ = bind(&Game::processNone, this);
					break;
				case SelfState::End:
					processFunction_ = bind(&Game::processNone, this);
					break;

					// 対応していない SelfState
				default:
					assert(false);
					break;
			}
		}

		// 親の数値生成
		void Game::processSelectParentValue()
		{
			// 乱数を引く(暫定で5で固定)
			const int parentValue = utility::Random::instance().random(kMin, kMax);
			GameData::instance().setParentValue(parentValue);

			// プレイヤーの入力待ちへ
			setSelfState(SelfState::WaitInput);
		}

		// 入力待ち
		void Game::processWaitInput()
		{
			childValue_ = BigOrSmall::None;

			printf("\n");
			printf("====================\n");
			printf("  Parent Value: *\n");
			printf("====================\n");

			string input = utility::getInput("Big or Small ? [1:Big, 2:Small]");
			if (input.length() > 0)
			{
				if (input[0] == '1')
				{
					childValue_ = BigOrSmall::Big;
				}
				else if (input[0] == '2')
				{
					childValue_ = BigOrSmall::Small;
				}

				// Big or Small の入力が決まったら判定へ
				if (childValue_ != BigOrSmall::None)
				{
					setSelfState(SelfState::CheckResult);
				}
			}
		}

		// 勝敗判定
		void Game::processCheckResult()
		{
			// 親の値の大小判定
			const BigOrSmall bigOrSmall = checkValue(GameData::instance().parentValue());

			// プレイヤーの入力と比較して勝敗判定
			if (childValue_ == bigOrSmall)
			{
				GameData::instance().winPoint++;
				setNextState(State::Win);
			}
			else
			{
				GameData::instance().losePoint++;
				setNextState(State::Lose);
			}

			// 終了状態へ
			setSelfState(SelfState::End);
		}
	}
}