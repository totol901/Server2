#include "stdafx.h"
#include "Thread.h"

//: 이후 초기화는 클래스의 어트리뷰트의 순서대로 초기화댐
Thread::Thread(thread_t *thread, wstr_t className)
	:className_(className),
	thread_(thread),
	id_(thread->get_id()),
	lock_(nullptr)
{
	THREADMANAGER.put(this);
}

Thread::Thread(Lock * const pLock, thread_t * thread, wstr_t className)
	:className_(className),
	thread_(thread),
	id_(thread->get_id())
{
	this->setLock(pLock);
	THREADMANAGER.put(this);
}

Thread::~Thread()
{
	//스래드 끝날때 까지 대기
	if (thread_->joinable())
	{
		thread_->join();
	}

	//그리고 삭제
	THREADMANAGER.remove(id_);
	SAFE_DELETE(thread_);
	
	//락 삭제
	/*
	//아래 함수 추가한 이유!
	여러 스래드가 void foo()라는 함수를 돌고 있다 가정한다
	foo()함수 내에 SAFE_LOCK매크로로 Lock을 걸어준다 가정한다
	
	*락을 따로 관리하지 않고 Thread 소멸자에서 lock delete를 해 주려 했다

	문제상황 
	case 1.
	1번 Thread가 종료되어 소멸자에서 lock을 해제함.
	2번 Thread가 SAFE_LOCK을 거는 중이었다.
	이경우 2번 Thread는 오류가 날 것이다.

	해결책=>
	Lock을 제일 마지막에 사용하는 Thread가 Lock을 해제시켜 줌
	*/
	if (lock_ && lock_->numbeOfThreadUsing() > 1)
	{
		lock_->decreaseCountNumberOfThreadUsing();
	}
	else
	{
		SAFE_DELETE(lock_);
	}
}

threadId_t Thread::id() const
{
	return id_;
}

wstr_t &Thread::className()
{
	return className_;
}

void Thread::setLock(Lock *lock)
{
	lock_ = lock;
	lock_->increaseCountNumberOfThreadUsing();
}

Lock* Thread::lock()
{
	return lock_;
}


//--------------------------------------------------//
ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
}

HRESULT ThreadManager::init()
{
	return S_OK;
}

void ThreadManager::release()
{
	std::vector<Thread*> tempVec;
	for (auto thread : threadPool_)
	{
		tempVec.push_back(thread.second);
	}
	
	for (int i = 0; i < tempVec.size(); i++)
	{
		SAFE_DELETE(tempVec[i]);
	}
}

void ThreadManager::put(Thread *thread)
{
	std::pair<threadId_t, Thread *> node(thread->id(), thread);
	threadPool_.insert(node);
	SLog(L"* create thread : id[0x%X] name[%s], pool size[%d]", thread->id(), thread->className().c_str(), threadPool_.size());
}

void ThreadManager::remove(threadId_t id)
{
	auto iter = threadPool_.find(id);
	if (iter == threadPool_.end())
	{
		return;
	}
	auto thread = iter->second;
	threadPool_.erase(iter);
}

Thread* ThreadManager::at(threadId_t id)
{
	if (threadPool_.empty())
	{
		return nullptr;
	}
	auto iter = threadPool_.find(id);
	if (iter == threadPool_.end())
	{
		return nullptr;
	}
	auto thread = iter->second;
	return thread;
}
