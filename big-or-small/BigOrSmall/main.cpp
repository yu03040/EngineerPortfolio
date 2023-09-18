#include"Source/Game/BigOrSmall.h"
#include <stdlib.h>

int main()
{
	// ゲームインスタンス生成
	game::BigOrSmall& bigOrSmall = game::BigOrSmall::instance();

	{// ゲームの実行
		// 初期化
		// 成功したらメインループを実行する
		if (bigOrSmall.initialize())
		{
			// メインループ
			bigOrSmall.mainLoop();
		}

		// 終了処理
		bigOrSmall.finalize();
	}

	system("pause"); // 続行するには何かキーを押してください . . .
	return 0;
}