#pragma once
#include<memory>
#include "Source/Utility/Singleton.h"
#include "Scene/Base.h"

using namespace std;

namespace game
{
	class BigOrSmall : public utility::Singleton<BigOrSmall>
	{
	private:
		// コンストラクタ
		/*
		
		 Singleton から継承した instance() のみで生成できるように private にする。
		 private なので Singleton からも実行できないが、フレンドクラスにして Singleton
		 からは参照できるようにする。
		 
		*/
		friend class utility::Singleton<BigOrSmall>;
		BigOrSmall() = default;

		// 実行中のシーン
		unique_ptr<scene::Base> scene_;

	public:
		
		// デストラクタ
		~BigOrSmall() = default;

		bool initialize();	// 初期化
		void mainLoop();	// メインループ
		void finalize();	// 終了処理
	};
}
