#include "Result.h"
#include "Source/Utility/Input.h"
#include "Source/Game/GameData.h"
#include "Game.h"

namespace game
{
	namespace scene
	{
		// 勝敗結果メッセージ文字列格納テーブル
		const string Result::kResultMessage[static_cast<int>(WinOrLose::WinOrLoseCount)] =
		{
			""					// None(ただの位置合わせ)
			, "おめでとう！"	// 勝ちメッセージ
			, "残念！"			// 負けメッセージ
		};

		// 再挑戦するかのメッセージ
		const string Result::kRetryMessage("再挑戦しますか？ [1:はい, 2:いいえ]");

		// 勝敗結果メッセージ取得
		const string& Result::resultMessage() const
		{
			const int index = static_cast<int>(state() == State::Win ? WinOrLose::Win : WinOrLose::Lose);
			return kResultMessage[index];
		}

		void Result::initialize()
		{
			selfState_ = SelfState::ShowMessage;
		}

		void Result::process()
		{
			switch (selfState_)
			{
				case game::scene::Result::SelfState::ShowMessage:
				{

					const int parentValue = GameData::instance().parentValue();
					printf("\n");
					printf("====================\n");
					printf("  Parent Value: %d\n", parentValue);
					printf("    %s\n", resultMessage().c_str());
					selfState_ = SelfState::WaitInput;
				}
				break;
				case game::scene::Result::SelfState::WaitInput:
				{
					if (GameData::instance().gameCount_ < 5) // 1回目は必ず通るので、コンティニューは4回
					{
						printf("--------------------\n");
						string input = utility::getInput(kRetryMessage);
						if (input.length() > 0)
						{
							if (input[0] == '1')
							{
								setNextState(State::Game);
								++GameData::instance().gameCount_;
							}
							else if (input[0] == '2')
							{
								setNextState(State::End);
							}
						}
					}
					else
					{
						setNextState(State::End);
					}
				}
				break;
			}
		}
	}
}