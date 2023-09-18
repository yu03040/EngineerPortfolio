#include "Random.h"
#include<cassert>

namespace utility
{
	// 初期化
	void Random::initialize()
	{
		// シード値のセット
		randomEngine_.seed(randomDevice_());
	}

	// 乱数生成
	int Random::random(const int min, const int max)
	{
		assert(min <= max);

		uniform_int_distribution<> distribution(min, max);
		return distribution(randomEngine_);
	}
}