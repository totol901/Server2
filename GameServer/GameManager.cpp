#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager()
{
	userMap_ = new UserMap();
}

GameManager::~GameManager()
{
	SAFE_DELETE(userMap_);
}

void GameManager::addUserIntoMap(User* newUser)
{
	userMap_->addUser(newUser);
}

void GameManager::sendUserList(Session* session)
{
	userMap_->sendUsersDataToSession(session);
}

void GameManager::tick()
{
	userMap_->tick();
}
