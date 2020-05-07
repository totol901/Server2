#pragma once

const int MAX_USER= 1000;

class UserManager : public Singleton < UserManager >
{
	friend Singleton;
private:
	std::vector<User*> notUsedUserPool_;
	std::unordered_map<oid_t, User*> userPool_;

	Lock						lock_;

public:
	UserManager()
		:lock_(L"UserManager")
	{
		for (int i = 0; i < MAX_USER; i++)
		{
			User* user = new User(nullptr);
			notUsedUserPool_.push_back(user);
		}
	}
	~UserManager() 
	{
		for (int i = 0; i < MAX_USER; i++)
		{
			SAFE_DELETE(notUsedUserPool_[i]);
		}
	}

	void insert(User* user)
	{
		SAFE_LOCK(lock_);
		oid_t key = user->session()->id();
		userPool_.insert(std::make_pair(key, user));
	}

	void insert(Session* session, User* user)
	{
		SAFE_LOCK(lock_);
		oid_t key = session->id();
		user->setSession(session);
		userPool_.insert(std::make_pair(key, user));
	}

	void remove(oid_t id)
	{
		SAFE_LOCK(lock_);
		User* user = at(id);
		user->setIsActive(false);
		userPool_.erase(id);
	}

	User* findNotActiveUserAndPushUserPool(Session* session)
	{
		SAFE_LOCK(lock_);
		User* user = findNotActiveUser();
		if (user)
		{
			user->setIsActive(true);
			insert(session, user);

			return user;
		}

		return nullptr;
	}

	User* findNotActiveUser()
	{
		SAFE_LOCK(lock_);
		for (auto iter : notUsedUserPool_)
		{
			if (!iter->isActive())
			{
				return iter;
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
