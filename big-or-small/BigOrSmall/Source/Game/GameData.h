#pragma once
#include "Source/Utility/Singleton.h"

namespace game
{
	class GameData :public utility::Singleton<GameData>
	{
	private:

		// コンストラクタ
		friend class utility::Singleton<GameData>;
		GameData() = default;

	public:

		// デストラクタ
		~GameData() = default;

	public:

		// データリセット
		void reset();

		// 親の値をセット
		void setParentValue(const int value)
		{
			parentValue_ = value;
		}

		// 親の値を取得
		int parentValue()
		{
			return parentValue_;
		}

	private:
		
		// 親が生成する値
		int parentValue_ = 0;

	public:

		int winPoint = 0;
		int losePoint = 0;
		int gameCount_;
		int winCount_;
	};
}


