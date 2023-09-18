#include<memory>
#include "BigOrSmall.h"
#include "Scene/Title.h"
#include "Scene/End.h"
#include "Source/Utility/Random.h"
#include "Source/Game/GameData.h"

using namespace game;
using namespace scene;
using namespace std;
using namespace utility;

bool BigOrSmall::initialize()
{
	// 乱数初期化
	Random::instance().initialize();

	// ゲームデータ初期化
	GameData::instance().reset();

	// 最初のシーンを設定
	scene_ = Factory::create(State::Title);

	return true;
}

void BigOrSmall::mainLoop()
{
	// End シーンになるまでループ
	while (scene_->state() != scene::State::End)
	{
		// 現在のシーン更新
		scene_->process();

		// 次のStateがセットされていたら切り替える
		if (scene_->isNextState())
		{
			// ステートをキープ
			State state = scene_->nextState();

			// 現在のシーン終了処理
			scene_->finalize();

			// 次のシーン作成
			scene_ = Factory::create(state);
		}
	}

	printf("\n%d回勝負\n",GameData::instance().gameCount_);
	printf("%d勝%d敗\n", GameData::instance().winPoint, GameData::instance().losePoint);
}

void BigOrSmall::finalize()
{
}
