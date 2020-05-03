#include "stdafx.h"
#include "UserMap.h"

UserMap::UserMap()
	:lock_(L"UserMap")
{
	responePoints[RED_TEAM] = Vector3(-3, 0, 30);
	responePoints[BLUE_TEAM] = Vector3(13, 0, -5);
}

UserMap::~UserMap()
{
}

void UserMap::addRedTeam(User* user)
{
	SAFE_LOCK(lock_);
	user->setPostion(responePoints[RED_TEAM]);
	redTeamArr.push_back(user);
}

void UserMap::addBlueTeam(User* user)
{
	SAFE_LOCK(lock_);
	user->setPostion(responePoints[BLUE_TEAM]);
	blueTreamArr.push_back(user);
}

void UserMap::sendAnsJoinMapPacket(User* user, Session* session)
{
	SAFE_LOCK(lock_);

	PK_C_ANS_JOIN_MAP ansPacket;
	ansPacket.name_ = user->name();
	ansPacket.oid_ = user->session()->id();
	ansPacket.isRedTeam_ = user->isRedTeam();
	ansPacket.posX_ = user->position().x_;
	ansPacket.posY_ = user->position().y_;
	ansPacket.posZ_ = user->position().z_;
	ansPacket.quatX_ = user->direction().x_;
	ansPacket.quatY_ = user->direction().y_;
	ansPacket.quatZ_ = user->direction().z_;
	ansPacket.quatW_ = user->direction().w_;

	session->sendPacket(&ansPacket);
}

void UserMap::addUser(User* user)
{
	SAFE_LOCK(lock_);

	if (redTeamArr.size() < blueTreamArr.size())
	{
		user->setRedTeam();
		addRedTeam(user);
	}
	else
	{
		user->setBlueTeam();
		addBlueTeam(user);
	}
}

void UserMap::eraseUser(User* user)
{
	SAFE_LOCK(lock_);

	for (auto iter = redTeamArr.begin(); iter != redTeamArr.end(); )
	{
		if (*iter == user)
		{
			redTeamArr.erase(iter);
			return;
		}
		else
		{
			iter++;
		}
	}

	for (auto iter = blueTreamArr.begin(); iter != blueTreamArr.end(); )
	{
		if (*iter == user)
		{
			blueTreamArr.erase(iter);
			return;
		}
		else
		{
			iter++;
		}
	}
}

void UserMap::sendUsersDataToSession(Session* session)
{
	SAFE_LOCK(lock_);
	for (int i = 0; i < redTeamArr.size(); i++)
	{
		sendAnsJoinMapPacket(redTeamArr[i], session);
	}

	for (int i = 0; i < blueTreamArr.size(); i++)
	{
		sendAnsJoinMapPacket(blueTreamArr[i], session);
	}
}

void UserMap::tick()
{
	SAFE_LOCK(lock_);
	for (auto iter : redTeamArr)
	{
		iter->tick();
	}

	for (auto iter : blueTreamArr)
	{
		iter->tick();
	}
}
