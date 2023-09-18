#pragma once
#include<iostream>
#include "State.h"

using namespace std;

namespace game
{
	namespace scene
	{
		// シーンの基底クラス
		class Base
		{
		public:

			// コンストラクタ
			Base() = delete;	// 引数なしコンストラクタは削除
			Base(const State state) : state_{ state } {};

			// デストラクタ
			virtual ~Base() = default;

		private:

			// このクラスが担当するシーンの種類
			State state_ = State::None;

			// 次に実行するシーン
			State nextState_ = State::None;

		public:

			/*
				インターフェース
				純粋仮想関数にし、定義の中身は派生クラスに託す
			*/
			virtual void initialize() = 0;
			virtual void process() = 0;
			virtual void finalize() = 0;

			State state()const
			{
				return state_;
			}

			// 次の	State をセット
			State nextState()const
			{
				return nextState_;
			}

			// 次のシーンが State されたか
			void setNextState(const State nextState)
			{
				nextState_ = nextState;
			}

			bool isNextState() const
			{
				return (nextState_ != State::None && nextState_ != State::SceneCount);
			}
		};

		class Factory
		{
		public:

			// シーンクラスのインスタンス生成メソッド
			static unique_ptr<Base> create(const State state);

		private:

			// インスタンスの操作を禁止する
			Factory() = delete;
			~Factory() = delete;
			Factory(const Factory&) = delete;
			Factory(Factory&&) = delete;
			Factory& operator=(const Factory&) = delete;
			Factory& operator=(Factory&&) = delete;
		};
	}
}