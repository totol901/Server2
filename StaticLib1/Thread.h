/******************************************************************************
���ϸ�	: Thread.h
����		: Thread�� ����
�����	:
��ũ�θ� ����Ͽ� �����带 ������, Ŭ���� �ż��� process�� ����
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
	Lock					*lock_;			//�����忡�� ����ϴ� ��
	wstr_t					className_;		//�������� �̸�
	thread_t				*thread_;		//���� ������
	threadId_t				id_;			//�������� id
	
	Lock* m_pCurrentLock;	//���� �ɸ� ��
	Lock* m_pReqLock;		//��û�� ��

public:
	Thread(thread_t *thread, wstr_t className);
	Thread(Lock* const pLock, thread_t *thread, wstr_t className);
	~Thread();

public:
	//set�Լ�
	void setLock(Lock *lock);

	//get�Լ�
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
	// HACK: hash_map / unordered_map ���� get�� �Ҷ�, ���̺귯������ ���� index ������ ����.
	// HACK: �׷� ������ ������ map���� �����̳� ��ü�� �Ѵ�.
#ifdef THREAD_POOL_HASHMAP
	hash_map <threadId_t, Thread*> threadPool_;
#else //THREAD_POOL_HASHMAP
	std::map <threadId_t, Thread *> threadPool_;
#endif //THREAD_POOL_HASHMAP

	ThreadManager();
	~ThreadManager();

public:
	///�ʱ�ȭ �Լ�
	HRESULT init();
	///���� �Լ�
	void release();

	/****************************************************************************
	�Լ���	: put
	����		: thread*�� �Ŵ��� �����̳ʿ� ����
	���ϰ�	: ����
	�Ű�����	: Thread*
	*****************************************************************************/
	void put(Thread *thread);

	/****************************************************************************
	�Լ���	: remove
	����		: �ش� thread id�� thread�� �����̳ʿ��� ����
	���ϰ�	: ����
	�Ű�����	: threadId_t
	*****************************************************************************/
	void remove(threadId_t id);

	/****************************************************************************
	�Լ���	: at
	����		: �����̳ʾ��� �ش� thread id�� thread*�� ����
	���ϰ�	: Thread*
	�Ű�����	: threadId_t
	*****************************************************************************/
	Thread *at(threadId_t id);
};
