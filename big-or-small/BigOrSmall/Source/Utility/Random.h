#pragma once

#include<random>
#include<iostream>
#include "Singleton.h"

using namespace std;

namespace utility
{
	class Random : public Singleton<Random>
	{
	private:
		
		// コンストラクタ
		friend class Singleton<Random>;
		Random() = default;

	private:

		//デストラクタ
		~Random() = default;

	public:

		// 初期化
		void initialize();

		// 乱数生成
		int random(const int min, const int max);

	private:
		
		// 乱数生成器
		random_device randomDevice_;
		default_random_engine randomEngine_;
	};

}

