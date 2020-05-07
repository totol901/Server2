#pragma once

enum
{
	RED_TEAM,
	BLUE_TEAM,
	MAX_TEAM
};

class UserMap
{
private:
	std::vector<User*> redTeamArr;
	std::vector<User*> blueTreamArr;

	Lock lock_;

private:
	void addRedTeam(User* user);
	void addBlueTeam(User* user);
	void sendAnsJoinMapPacket(User* user, Session* session);
	
public:
	UserMap();
	~UserMap();

	Vector3 responePoints[MAX_TEAM];

	void addUser(User* user);
	void eraseUser(User* user);

	void sendUsersDataToSession(Session* session);

	void tick();
};

