#pragma once

#include<type_traits>

namespace utility
{
	template<typename Type>
	class Singleton
	{
	public:
		// インスタンス取得
		static Type& instance()
		{
			// TypeがSingletonを継承しているかチェック
			static_assert(std::is_base_of<Singleton, Type>::value);

			// C++11以降では、ローカルなstatic変数の初期化子はマルチスレッド環境でも1回しか実行されない(スレッドセーフ)
			static Type kInstance;

			return kInstance;
		}

		virtual ~Singleton() = default;

	protected:

		// インスタンスの生成に制限をかけるため、protected(派生クラスからのコンストラクト時によばれる)
		Singleton() = default;

	private:

		// コピー禁止
		Singleton(const Singleton&) = delete;
		Singleton& operator =(const Singleton&) = delete;

		// ムーブ禁止
		Singleton(Singleton&&) = delete;
		Singleton& operator =(Singleton&&) = delete;
	};
}