#include "stdafx.h"
#include "Thread.h"

//: ���� �ʱ�ȭ�� Ŭ������ ��Ʈ����Ʈ�� ������� �ʱ�ȭ��
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
	//������ ������ ���� ���
	if (thread_->joinable())
	{
		thread_->join();
	}

	//�׸��� ����
	THREADMANAGER.remove(id_);
	SAFE_DELETE(thread_);
	
	//�� ����
	/*
	//�Ʒ� �Լ� �߰��� ����!
	���� �����尡 void foo()��� �Լ��� ���� �ִ� �����Ѵ�
	foo()�Լ� ���� SAFE_LOCK��ũ�η� Lock�� �ɾ��ش� �����Ѵ�
	
	*���� ���� �������� �ʰ� Thread �Ҹ��ڿ��� lock delete�� �� �ַ� �ߴ�

	������Ȳ 
	case 1.
	1�� Thread�� ����Ǿ� �Ҹ��ڿ��� lock�� ������.
	2�� Thread�� SAFE_LOCK�� �Ŵ� ���̾���.
	�̰�� 2�� Thread�� ������ �� ���̴�.

	�ذ�å=>
	Lock�� ���� �������� ����ϴ� Thread�� Lock�� �������� ��
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
