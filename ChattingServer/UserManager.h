#pragma once

class UserManager : public Singleton < UserManager >
{
	friend Singleton;
private:
	std::unordered_map<oid_t, User*> userPool_;

	Lock						lock_;

public:
	UserManager()
		:lock_(L"UserManager")
	{}
	~UserManager() {}

	void insert(User *user)
	{
		SAFE_LOCK(lock_);
		oid_t key = user->session()->id();
		userPool_.insert(std::make_pair(key, user));
	}

	void remove(oid_t id)
	{
		SAFE_LOCK(lock_);
		userPool_.erase(id);
	}

	User* findName(string name)
	{
		SAFE_LOCK(lock_);
		for (auto iter : userPool_)
		{
			if (iter.second->name().compare(name) == 0)
			{
				return iter.second;
			}
		}

		return nullptr;
	}

	User* at(oid_t id)
	{
		SAFE_LOCK(lock_);
		auto itr = userPool_.find(id);
		if (itr == userPool_.end()) 
		{
			return nullptr;
		}
		return itr->second;
	}

	size_t size()
	{
		SAFE_LOCK(lock_);
		return userPool_.size();
	}

	void sendMessageAllUser(Packet* paket)
	{
		SAFE_LOCK(lock_);
		for (auto iter = userPool_.begin();
			iter != userPool_.end(); iter++)
		{
			iter->second->session()->sendPacket(paket);
		}
	}
};

class UserTempStorage
{
private:
	unsigned int userTempSeedNum_;
	std::unordered_map<unsigned int, string> userTempPool_;

public:
	UserTempStorage()
		:userTempSeedNum_(0)
	{}
	~UserTempStorage() {}

};
