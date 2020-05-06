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
	INSERT_PACKET_PROCESS(C_MOVE_START_INPUT);
	INSERT_PACKET_PROCESS(C_SHELL_SHOOT_INPUT);
	INSERT_PACKET_PROCESS(C_SHEEL_DEMAGE_INPUT);
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
	ansPacket.oid_ = session->id();
	ansPacket.isRedTeam_ = newUser->isRedTeam();
	ansPacket.posX_ = newUser->position().x_;
	ansPacket.posY_ = newUser->position().y_;
	ansPacket.posZ_ = newUser->position().z_;
	ansPacket.quatX_ = newUser->direction().x_;
	ansPacket.quatY_ = newUser->direction().y_;
	ansPacket.quatZ_ = newUser->direction().z_;
	ansPacket.quatW_ = newUser->direction().w_;

	USERMANAGER.sendMessageAllUser(&ansPacket);
}

void GameProcess::C_MOVE_START_INPUT(Session* session, Packet* rowPacket)
{
	PK_C_MOVE_START_INPUT* packet = (PK_C_MOVE_START_INPUT*)rowPacket;

	User* user = USERMANAGER.at(session->id());
	if (user)
	{
		Vector3 pos = Vector3(packet->PosX_, packet->PosY_, packet->PosZ_);
		Quaternion quat = Quaternion(packet->quatX_, packet->quatY_, packet->quatZ_, packet->quatW_);
		user->setPostion(pos);
		user->setDirection(quat);

		PK_S_MOVE_START ansPacket;
		ansPacket.oid_ = session->id();
		ansPacket.PosX_ = packet->PosX_;
		ansPacket.PosY_ = packet->PosY_;
		ansPacket.PosZ_ = packet->PosZ_;
		ansPacket.quatX_ = packet->quatX_;
		ansPacket.quatY_ = packet->quatY_;
		ansPacket.quatZ_ = packet->quatZ_;
		ansPacket.quatW_ = packet->quatW_;
		ansPacket.moveNum_ = packet->moveNum_;
		ansPacket.state_ = packet->state_;

		USERMANAGER.sendMessageAllUser(&ansPacket);
	}
}

void GameProcess::C_SHELL_SHOOT_INPUT(Session* session, Packet* rowPacket)
{
	PK_C_SHELL_SHOOT_INPUT* packet = (PK_C_SHELL_SHOOT_INPUT*)rowPacket;

	User* user = USERMANAGER.at(session->id());
	if (user)
	{
		Vector3 pos = Vector3(packet->PosX_, packet->PosY_, packet->PosZ_);
		Quaternion quat = Quaternion(packet->quatX_, packet->quatY_, packet->quatZ_, packet->quatW_);
	
		PK_S_SHELL_SHOOT ansPacket;
		ansPacket.oid_ = session->id();
		ansPacket.PosX_ = packet->PosX_;
		ansPacket.PosY_ = packet->PosY_;
		ansPacket.PosZ_ = packet->PosZ_;
		ansPacket.quatX_ = packet->quatX_;
		ansPacket.quatY_ = packet->quatY_;
		ansPacket.quatZ_ = packet->quatZ_;
		ansPacket.quatW_ = packet->quatW_;
		ansPacket.speed_ = packet->speed_;

		USERMANAGER.sendMessageAllUser(&ansPacket);
	}
}

void GameProcess::C_SHEEL_DEMAGE_INPUT(Session* session, Packet* rowPacket)
{
	PK_C_SHEEL_DEMAGE_INPUT* packet = (PK_C_SHEEL_DEMAGE_INPUT*)rowPacket;

	User* user = USERMANAGER.at(packet->oid_);
	if (user)
	{
		PK_S_SHEEL_DEMAGE ansPacket;
		ansPacket.oid_ = packet->oid_;
		ansPacket.demage_ = packet->demage_;
		
		int HP = user->currentHP() - packet->demage_;
		user->setCurrentHP(HP);

		USERMANAGER.sendMessageAllUser(&ansPacket);
	}
}
