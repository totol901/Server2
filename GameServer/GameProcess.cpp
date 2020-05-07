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
	INSERT_PACKET_PROCESS(C_TANK_RESPONE);
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
	User* newUser = USERMANAGER.findNotActiveUserAndPushUserPool(session);
	if(newUser)
	{
		newUser->setName(packet->name_);
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
		return;
	}

	session->onClose();
}

void GameProcess::C_MOVE_START_INPUT(Session* session, Packet* rowPacket)
{
	PK_C_MOVE_START_INPUT* packet = (PK_C_MOVE_START_INPUT*)rowPacket;

	User* user = USERMANAGER.at(session->id());
	if (user && !user->isDead())
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
	if (user && !user->isDead())
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
	if (user && !user->isDead() && user->isEndInvincible())
	{
		int HP = user->currentHP() - packet->demage_;
		user->setCurrentHP(HP);

		if (HP <= 0)
		{
			user->setIsDead(true);
			user->setResponeTimeStart(NOW_TICK());

			PK_C_TANK_DEAD deadPacket;
			deadPacket.oid_ = packet->oid_;
			deadPacket.demage_ = packet->demage_;
			
			USERMANAGER.sendMessageAllUser(&deadPacket);
		}
		else
		{
			PK_S_SHEEL_DEMAGE ansPacket;
			ansPacket.oid_ = packet->oid_;
			ansPacket.demage_ = packet->demage_;

			USERMANAGER.sendMessageAllUser(&ansPacket);
		}
	}
}

void GameProcess::C_TANK_RESPONE(Session* session, Packet* rowPacket)
{
	PK_C_TANK_RESPONE* packet = (PK_C_TANK_RESPONE*)rowPacket;

	User* user = USERMANAGER.at(packet->oid_);
	if (user && user->isDead())
	{
		time_t milsec = NOW_TICK() - user->responeTimeStart();

		if (milsec > 2)
		{
			user->setIsDead(false);
			user->setMaxHP();
			user->setResponeEndTime(NOW_TICK());

			PK_C_TANK_RESPONE_RESPONSE ansPacket;
			ansPacket.oid_ = packet->oid_;
			if (user->isRedTeam())
			{
				ansPacket.PosX_ = GAMEMANAGER.userMap()->responePoints[RED_TEAM].x_;
				ansPacket.PosY_ = GAMEMANAGER.userMap()->responePoints[RED_TEAM].y_;
				ansPacket.PosZ_ = GAMEMANAGER.userMap()->responePoints[RED_TEAM].z_;
				ansPacket.quatX_ = 0;
				ansPacket.quatY_ = 0;
				ansPacket.quatZ_ = 0;
				ansPacket.quatW_ = 1;
			}
			else
			{
				ansPacket.PosX_ = GAMEMANAGER.userMap()->responePoints[BLUE_TEAM].x_;
				ansPacket.PosY_ = GAMEMANAGER.userMap()->responePoints[BLUE_TEAM].y_;
				ansPacket.PosZ_ = GAMEMANAGER.userMap()->responePoints[BLUE_TEAM].z_;
				ansPacket.quatX_ = 0;
				ansPacket.quatY_ = 0.5;
				ansPacket.quatZ_ = 0;
				ansPacket.quatW_ = 1;
			}

			USERMANAGER.sendMessageAllUser(&ansPacket);
		}
		else
		{
			//치팅 중이거나 버그 생김 플레이어 킥해야함

		}
	}
}
