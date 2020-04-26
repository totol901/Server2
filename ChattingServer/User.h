#pragma once

class User : public GameObject
{
private:
	Session *session_;
	float x_;
	float y_;
	float z_;

	float speed_;
	int hp_;

public:
	User(Session *session)
		:session_(session) {}
	~User() {}

	Session* session()
	{
		return session_;
	}

	void tick()
	{
	}
};
