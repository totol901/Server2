#pragma once

const float USER_TICK_TIME = 0.25;
const float USER_SPEED = 12.0f;
const int USER_MAX_HP = 100;

enum E_USER_STATE
{
	E_IDEAL,
	E_MOVE,
};

class User : public GameObject
{
private:
	Session* session_;

	E_USER_STATE state_;
	Vector3 position_;
	Quaternion direction_;
	float speed_;
	bool isRedTeam_;

	int maxHP_;
	int currentHP_;

public:
	User(Session* session)
		:session_(session), 
		speed_(USER_SPEED),
		isRedTeam_(false), 
		maxHP_(USER_MAX_HP),
		currentHP_(USER_MAX_HP),
		state_(E_IDEAL)
	{}
	~User()
	{}

	Vector3 position() { return position_; }
	Quaternion direction() { return direction_; }
	Session* session() { return session_; }
	Int32 state() { return state_; }
	bool isRedTeam() { return isRedTeam_; }

	void setRedTeam(){isRedTeam_ = true;}
	void setBlueTeam(){isRedTeam_ = false;}
	
public:
	void tick()
	{
		//position_ = position_ + direction_ * (speed_ * USER_TICK_TIME);
	}
};
