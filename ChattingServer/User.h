#pragma once

class User : public GameObject
{
private:
	Session *session_;

public:
	User(Session *session)
		:session_(session)
	{}
	~User() 
	{}

	Session* session()
	{
		return session_;
	}

	void tick()
	{
	}
};