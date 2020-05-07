#pragma once

const float USER_TICK_TIME = 0.25;
const float USER_SPEED = 12.0f;
const int USER_MAX_HP = 100;

class User : public GameObject
{
private:
	Session* session_;

	Vector3 position_;
	Quaternion direction_;
	float speed_;
	bool isRedTeam_;

	int maxHP_;
	int currentHP_;

	bool isDead_;

	time_t responeTimeStart_;

	time_t responeEndTime_;

	bool isActive_;

public:
	User(Session* session)
		:session_(session), 
		speed_(USER_SPEED),
		isRedTeam_(false), 
		maxHP_(USER_MAX_HP),
		currentHP_(USER_MAX_HP),
		isDead_(false),
		responeEndTime_(0),
		responeTimeStart_(0),
		isActive_(false)
	{}
	~User()
	{}

	void setMaxHP()
	{
		currentHP_ = maxHP_;
	}

	bool isEndInvincible()
{
		if (NOW_TICK() - responeEndTime_ > 3)
		{
			return true;
		}
		return false;
	}

	void setSession(Session* session) { session_ = session; }
	bool isActive() { return isActive_; }
	void setIsActive(bool isActive){isActive_ = isActive;}
	void setResponeEndTime(time_t time){responeEndTime_ = time;}
	void setResponeTimeStart(time_t time) { responeTimeStart_ = time; }
	time_t responeTimeStart() { return responeTimeStart_; }
	bool isDead() { return isDead_; }
	void setIsDead(bool isdead) { isDead_ = isdead; }
	int currentHP() { return currentHP_; }
	void setCurrentHP(int currentHP) { currentHP_ = currentHP; }

	Vector3 position() { return position_; }
	void setPostion(Vector3& pos) { position_ = pos; }
	Quaternion direction() { return direction_; }
	void setDirection(Quaternion& quat) { direction_ = quat; }
	Session* session() { return session_; }

	bool isRedTeam() { return isRedTeam_; }

	void setRedTeam() {isRedTeam_ = true;}
	void setBlueTeam() {isRedTeam_ = false;}
	
public:
	void tick()
	{
		//position_ = position_ + direction_ * (speed_ * USER_TICK_TIME);
	}
};
