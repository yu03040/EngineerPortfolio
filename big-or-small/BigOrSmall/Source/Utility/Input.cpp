#include "Input.h"
#include<iostream>

using namespace std;

namespace utility
{
	string getInput(const std::string& message)
	{
		// プロンプト表示
		printf("%s > ", message.c_str());

		// 入力受け取り用
		string input;
		getline(cin, input);

		// 入力された文字列を返す
		return input;
	}
}