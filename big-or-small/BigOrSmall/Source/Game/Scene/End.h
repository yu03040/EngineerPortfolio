#pragma once
#include "Base.h"
#include<cassert>

namespace game
{
	namespace scene
	{
		class End : public Base
		{
		public:

			// コンストラクタ
			End() : Base(State::End) {};

			// デストラクタ
			~End() = default;

			// インターフェースの実装(何もない)
			void initialize() override {};
			void process() override
			{
				assert(false);
			};
			void finalize() override {};
		};


	}
}
