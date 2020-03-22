#include "stdafx.h"
#include "Lock.h"

//더하기 1~100까지 스레드는 5개
#ifdef _DEBUG
#define STERN_MODE
#endif

const int INVALID_LINE = -1;

Lock::Lock(const WCHAR *name)
	:lockId_(LockManager::getInstance().generalId()),
	name_(name),
	numberOfThreadUsing_(0)
{
	lockingFile_.clear();
	lockingLine_ = INVALID_LINE;
}

Lock::~Lock()
{
	name_.clear();
}

const size_t& Lock::numbeOfThreadUsing() const
{
	return numberOfThreadUsing_;
}

const WCHAR* Lock::name()
{
	return name_.c_str();
}

size_t Lock::lockId() const
{
	return lockId_;
}

lock_t& Lock::mutex()
{
	return mutex_;
}

void Lock::lock(LPCWSTR fileName, int lineNo)
{
	mutex_.lock();

	lockingFile_ = fileName;
	lockingLine_ = lineNo;
}

void Lock::unlock()
{
	mutex_.unlock();

	lockingFile_.clear();
	lockingLine_ = INVALID_LINE;
}

void Lock::increaseCountNumberOfThreadUsing()
{
	++numberOfThreadUsing_;
}

void Lock::decreaseCountNumberOfThreadUsing()
{
	--numberOfThreadUsing_;
}

void Lock::setThreadId(threadId_t id)
{
	threadId_ = id;
}

void Lock::setNumberOfThreadUsing(const size_t & num)
{
	numberOfThreadUsing_ = num;
}

threadId_t Lock::threadId() const
{
	return threadId_;
}

//-------------------------------------------------------//
// 생성자에서 락을 걸고, 스코프 빠져나가는 객체 해제시 락을 푼다.
// std::lock_guard<lock_t> guard(lock) 과 같은거지만, 
// 데드락 감지를 위해 직접 구현한다.
LockSafeScope::LockSafeScope(Lock *lock, LPCWSTR fileName, int lineNo)
{
	if (lock == nullptr)
	{
		return;
	}
	if (_shutdown == true)
	{
		return;
	}

	lock_ = lock;
	Lock *deadLock = LockManager::getInstance().checkDeadLock(lock_);
	if (deadLock != nullptr)
	{
#ifdef STERN_MODE
		SErrLog(L"! [%s]lock and [%s]lock is dead detecting!", deadLock->name(), lock->name());
#else
		//데드락 해제
		std::lock(lock_->mutex(), deadLock->mutex());
		SLog(L"!!! [%s]lock and [%s]lock is dead detecting!", deadLock->name(), lock->name());
#endif
		return;
	}

	lock_->lock(fileName, lineNo);
	lock->setThreadId(GET_CURRENT_THREAD_ID());
}

LockSafeScope::~LockSafeScope()
{
	if (!lock_)
	{
		return;
	}
	if (_shutdown == true)
	{
		return;
	}
	lock_->unlock();
	//lock_->setThreadId(nullptr);
}

//-------------------------------------------------------//
LockManager::LockManager()
	:idSeed_(0)
{
}

LockManager::~LockManager()
{
}

HRESULT LockManager::init()
{
	return S_OK;
}

void LockManager::release()
{
}

/*
http://www.npteam.net/849
http://www.boost.org/doc/libs/1_46_1/libs/graph/doc/astar_visitor.html
http://includestdio.tistory.com/m/post/12

http://www.bogotobogo.com/cplusplus/multithreaded4_cplusplus11B.php#nested_lock
락은 자신이 걸린 threadId를,
Thread에서는 걸고있는 Lock 번호가 있음.
이들을 따라가 보다가 자신이 걸려는 lock이 나오면 lock 순환
즉 데드락 처리.
*/
Lock* LockManager::searchLockCycle(Lock *newLock)
{
	//list 따라 lock 이름을 비교해 본다.?? <-뭐여?! 제작자의 주석이 좀 이상함 제외할 것

	//락 요청을 한 스래드를 ThreadManager를 통해 가져옴
	Thread *thread = ThreadManager::getInstance().at(GET_CURRENT_THREAD_ID());
	
	//락 요청한 스래드가 ThreadManager에 없다면 nullptr 리턴
	if (!thread)
	{
		return nullptr;
	}

	std::vector<Lock *> trace;	//데드락 추적을 위한 벡터
	trace.push_back(newLock);	//벡터의 0번째에 요청한 락을 넣어줌

	//데드락을 루프돌며 서치
	Lock *deadLock = nullptr;
	while (true)
	{
		Lock *threadLock = thread->lock();

		if (threadLock == nullptr)
		{
			break;
		}

		if (threadLock->lockId() == trace[0]->lockId())
		{
			deadLock = threadLock;
			break;
		}
		trace.push_back(threadLock);

		thread = ThreadManager::getInstance().at(threadLock->threadId());
		if (!thread)
		{
			break;
		}
	}
	trace.empty();

	return deadLock;
}

Lock* LockManager::checkDeadLock(Lock *newLock)
{
	Lock *deadLock = this->searchLockCycle(newLock);
	if (deadLock)
	{
		return deadLock;
	}

	return nullptr;
}

size_t LockManager::generalId()
{
	size_t id = idSeed_++;
	return id;
}
