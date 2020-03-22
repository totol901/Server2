/******************************************************************************
파일명	: Thread.h
목적		: Thread를 관리
사용방식	:
매크로를 사용하여 스레드를 생성함, 클래스 매서드 process를 실행
******************************************************************************/
#pragma once

typedef std::function<void(void *)> ThreadFunction;
typedef void(*ThreadProcess)();

class Lock;

#define MAKE_THREAD(className, process)	(new Thread(new thread_t(&className##::##process, this), L#className))
#define MAKE_LOCK_THREAD(ptLock, className, process)	\
(new Thread(ptLock, new thread_t(&className##::##process, this), L#className))

#define GET_CURRENT_THREAD_ID		std::this_thread::get_id

class Thread
{
private:
	Lock					*lock_;			//스래드에서 사용하는 락
	wstr_t					className_;		//스레드의 이름
	thread_t				*thread_;		//실재 스레드
	threadId_t				id_;			//스레드의 id
	
	Lock* m_pCurrentLock;	//현재 걸린 락
	Lock* m_pReqLock;		//요청한 락

public:
	Thread(thread_t *thread, wstr_t className);
	Thread(Lock* const pLock, thread_t *thread, wstr_t className);
	~Thread();

public:
	//set함수
	void setLock(Lock *lock);

	//get함수
	threadId_t id() const;
	wstr_t &className();
	Lock *lock();
};

//---------------------------------------------------------------------//
//#define THREAD_POOL_HASHMAP
class ThreadManager : public Singleton < ThreadManager >
{
	friend Singleton;
private:
	// HACK: hash_map / unordered_map 에서 get를 할때, 라이브러리에서 버켓 index 에러가 난다.
	// HACK: 그런 이유로 검증된 map으로 컨테이너 교체를 한다.
#ifdef THREAD_POOL_HASHMAP
	hash_map <threadId_t, Thread*> threadPool_;
#else //THREAD_POOL_HASHMAP
	std::map <threadId_t, Thread *> threadPool_;
#endif //THREAD_POOL_HASHMAP

	ThreadManager();
	~ThreadManager();

public:
	///초기화 함수
	HRESULT init();
	///해제 함수
	void release();

	/****************************************************************************
	함수명	: put
	설명		: thread*를 매니저 컨터이너에 넣음
	리턴값	: 없음
	매개변수	: Thread*
	*****************************************************************************/
	void put(Thread *thread);

	/****************************************************************************
	함수명	: remove
	설명		: 해당 thread id의 thread를 컨테이너에서 제거
	리턴값	: 없음
	매개변수	: threadId_t
	*****************************************************************************/
	void remove(threadId_t id);

	/****************************************************************************
	함수명	: at
	설명		: 컨테이너안의 해당 thread id의 thread*를 리턴
	리턴값	: Thread*
	매개변수	: threadId_t
	*****************************************************************************/
	Thread *at(threadId_t id);
};
