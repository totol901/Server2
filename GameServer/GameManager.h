#pragma once

class GameManager : public Singleton<GameManager>
{
private:
	UserMap* userMap_;

public:
	GameManager();
	~GameManager();

	UserMap* userMap() { return userMap_; }
	void addUserIntoMap(User* newUser);
	void sendUserList(Session* session);

	void tick();
};
