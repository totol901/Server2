/******************************************************************************
파일명	: Lock.h
목적		: Lock를 관리
사용방식	:
매크로를 사용하여 데드락 유무 판단후 Lock 걸어줌
******************************************************************************/
#pragma once

//---------------------------------------------------------------------------//
class Lock
{
private:
	lock_t          mutex_;				//락을 위한 뮤텍스
	wstr_t			name_;				//락의 이름
	size_t          lockId_;			//락의 id

	threadId_t		threadId_;			//락 걸린 스레드 id

	wstr_t			lockingFile_;		//락을 건 파일 이름 string
	int				lockingLine_;		//락을 건 파일 라인 번호

	size_t			numberOfThreadUsing_;//락을 사용하는 스래드 수

public:
	Lock(const WCHAR* name);
	~Lock();

	/****************************************************************************
	함수명	: lock
	설명		: 스레드 lock을 걸고, 파일이름, 라인 번호 저장
	리턴값	: 없음
	매개변수	: LPCWSTR, int
	*****************************************************************************/
	void lock(LPCWSTR fileName, int lineNo);

	/****************************************************************************
	함수명	: unlock
	설명		: 스레드 unlock을 해줌
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void unlock();

	///numberOfThreadUsing_을 1 더해줌
	void increaseCountNumberOfThreadUsing();
	///numberOfThreadUsing_을 1 빼줌
	void decreaseCountNumberOfThreadUsing();

public:
	//set함수
	void setThreadId(threadId_t id);
	void setNumberOfThreadUsing(const size_t& num);
	

	//get함수
	const size_t& numbeOfThreadUsing() const;
	const WCHAR* name();
	size_t lockId() const;
	lock_t& mutex();
	threadId_t threadId() const;
};

//---------------------------------------------------------------------------//
class LockSafeScope
{
private:
	Lock          *lock_;	//데드락 걸렸는지 검사할 Lock 클래스

public:
	LockSafeScope(Lock *lock, LPCWSTR fileName, int lineNo);
	~LockSafeScope();
};

/****************************************************************************
매크로명	: SAFE_LOCK
설명		: 안전한 스레드 락을 실시함
매개변수	: Lock*
*****************************************************************************/
#define SAFE_LOCK(lock)     LockSafeScope __lockSafe(&lock, _W(__FILE__), __LINE__);

//---------------------------------------------------------------------------//
// 데드락 탐지를 위함
class LockManager : public Singleton < LockManager >
{
private:
	size_t       idSeed_;	//id 생성에 쓰일 seed

public:
	LockManager();
	~LockManager();

	HRESULT init();
	void release();

	/****************************************************************************
	함수명	: searchLockCycle
	설명		: 데드락 걸린 Lock을 찾아 리턴해줌
	리턴값	: Lock*
	매개변수	: Lock*
	*****************************************************************************/
	Lock* searchLockCycle(Lock *newLock);

	/****************************************************************************
	함수명	: checkDeadLock
	설명		: 데드락 걸린 Lock을 리턴해줌
	리턴값	: Lock*
	매개변수	: Lock*
	*****************************************************************************/
	Lock* checkDeadLock(Lock *lock);

	/****************************************************************************
	함수명	: generalId
	설명		: 새로운 id를 리턴해줌
	리턴값	: size_t
	매개변수	: 없음
	*****************************************************************************/
	size_t generalId();
};
