#include "stdafx.h"
#include "GameProcess.h"

GameProcess::GameProcess()
	:ContentsProcess()
{
	this->initialize();
}

GameProcess::~GameProcess()
{
}

void GameProcess::initialize()
{
	ContentsProcess::initialize();

	this->registSubPacketFunc();
}

void GameProcess::release()
{
	ContentsProcess::release();
}

void GameProcess::registSubPacketFunc()
{
#define INSERT_PACKET_PROCESS(type)		runFuncTable_.insert(std::make_pair(E_##type, &ChattingProcess::##type))

}
