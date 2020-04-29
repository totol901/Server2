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
#define INSERT_PACKET_PROCESS(type)		runFuncTable_.insert(std::make_pair(E_##type, &GameProcess::##type))
	
	INSERT_PACKET_PROCESS(C_REQ_JOIN_MAP);
}

void GameProcess::C_REQ_JOIN_MAP(Session* session, Packet* rowPacket)
{
	PK_C_REQ_JOIN_MAP* packet = (PK_C_REQ_JOIN_MAP*)rowPacket;

	if (USERMANAGER.at(session->id()))
	{
		SLog(L"* User already join, id: %d", session->id());
		return;
	}

	//해당 유저에게 방에 존재하는 사람들을 알려줌
	GAMEMANAGER.sendUserList(session);

	//유저 맵 입장
	User* newUser = new User(session);
	newUser->setName(packet->name_);
	USERMANAGER.insert(newUser);
	GAMEMANAGER.addUserIntoMap(newUser);
	
	//해당 유저 들어왔다는거 다른 클라에게 알려줌
	PK_C_ANS_JOIN_MAP ansPacket;
	ansPacket.name_ = newUser->name();
	ansPacket.isRedTeam_ = newUser->isRedTeam();
	ansPacket.posX_ = newUser->position().x_;
	ansPacket.posY_ = newUser->position().y_;
	ansPacket.posZ_ = newUser->position().z_;
	ansPacket.quatX_ = newUser->direction().x_;
	ansPacket.quatY_ = newUser->direction().y_;
	ansPacket.quatZ_ = newUser->direction().z_;
	ansPacket.quatW_ = newUser->direction().w_;
	ansPacket.state_ = newUser->state();

	USERMANAGER.sendMessageAllUser(&ansPacket);
}
