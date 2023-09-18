#include "GameData.h"

namespace game
{
	void GameData::reset()
	{
		parentValue_ = 0;
		winPoint = 0;
		losePoint = 0;
		gameCount_ = 1; // 絶対 1 回はやるから 1 で初期化
		winCount_ = 0;
	}
}