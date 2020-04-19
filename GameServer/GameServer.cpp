// GameServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"

int main()
{
	_wsetlocale(LC_ALL, L"korean");
	GameServerClass ServerClass;

	//싱글턴 클래스들 초기화
	if (SUCCEEDED(ServerClass.init()))
	{
		DBManager::getInstance().run();

		//서버 스래드 돌려줌
		ServerClass.run();
		//서버 스래드 종료됨, shutdown() => 패킷 받기 중단 -> 이미 받은 패킷들 처리 -> DB에 업뎃 -> 종료 준비 끝
	}
	else
	{
		SLog(L"! Server Init Failed!");
	}

	//싱글턴 클래스들 해제
	ServerClass.release();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
