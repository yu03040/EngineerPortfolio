#include<iostream>
#include "Title.h"
#include "Source/Utility/Input.h"

using namespace game;
using namespace scene;
using namespace std;

// 初期化
void Title::initialize()
{
	selfState_ = selfState::ShowTitle;
}

// 更新
void Title::process()
{
	switch (selfState_)
	{
		case game::scene::Title::selfState::ShowTitle:

			printf("====================\n");
			printf("    Big Or Small\n");
			printf("--------------------\n");
			printf("ルール：\n");
			printf("1.ランダムに[0-9]の値を抽選します。\n");
			printf("2.値を以下のように予想してください。\n");
			printf("         [0-4]: Small\n");
			printf("         [5-9]: Big\n");
			printf("3.予想が当たったら勝ちです。\n");
			printf("====================\n");

			selfState_ = selfState::WaitEnter;

			break;

		case game::scene::Title::selfState::WaitEnter:

			printf("\n");
			utility::getInput("Press Enter Key");

			// 次のシーンを Game にして終了
			setNextState(State::Game);
			selfState_ = selfState::End;

			break;

		default:
			break;
	}
}

// 終了処理(何もしない)
void Title::finalize()
{
}
