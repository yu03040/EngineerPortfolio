#include<cassert>
#include "Base.h"
#include "Title.h"
#include "End.h"
#include "Game.h"
#include "Result.h"

using namespace std;

namespace game
{
	namespace scene
	{
		unique_ptr<Base> Factory::create(const State state)
		{
			// インスタンス生成
			unique_ptr<Base> scene;

			// state に合わせて対応のシーンを生成
			switch (state)
			{
				case State::Title:
					scene = make_unique<Title>();
					break;
				case State::Game:
					scene = make_unique<Game>();
					break;
				case State::Win:
					scene = make_unique<Result>(State::Win);
					break;
				case State::Lose:
					scene = make_unique<Result>(State::Lose);
					break;
				case State::End:
					scene = make_unique<End>();
					break;

				default:
					assert(false);
					break;
			}

			// 初期化処理
			scene->initialize();

			return scene;
		}
	}
}
