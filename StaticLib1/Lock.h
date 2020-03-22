/******************************************************************************
���ϸ�	: Lock.h
����		: Lock�� ����
�����	:
��ũ�θ� ����Ͽ� ����� ���� �Ǵ��� Lock �ɾ���
******************************************************************************/
#pragma once

//---------------------------------------------------------------------------//
class Lock
{
private:
	lock_t          mutex_;				//���� ���� ���ؽ�
	wstr_t			name_;				//���� �̸�
	size_t          lockId_;			//���� id

	threadId_t		threadId_;			//�� �ɸ� ������ id

	wstr_t			lockingFile_;		//���� �� ���� �̸� string
	int				lockingLine_;		//���� �� ���� ���� ��ȣ

	size_t			numberOfThreadUsing_;//���� ����ϴ� ������ ��

public:
	Lock(const WCHAR* name);
	~Lock();

	/****************************************************************************
	�Լ���	: lock
	����		: ������ lock�� �ɰ�, �����̸�, ���� ��ȣ ����
	���ϰ�	: ����
	�Ű�����	: LPCWSTR, int
	*****************************************************************************/
	void lock(LPCWSTR fileName, int lineNo);

	/****************************************************************************
	�Լ���	: unlock
	����		: ������ unlock�� ����
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void unlock();

	///numberOfThreadUsing_�� 1 ������
	void increaseCountNumberOfThreadUsing();
	///numberOfThreadUsing_�� 1 ����
	void decreaseCountNumberOfThreadUsing();

public:
	//set�Լ�
	void setThreadId(threadId_t id);
	void setNumberOfThreadUsing(const size_t& num);
	

	//get�Լ�
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
	Lock          *lock_;	//����� �ɷȴ��� �˻��� Lock Ŭ����

public:
	LockSafeScope(Lock *lock, LPCWSTR fileName, int lineNo);
	~LockSafeScope();
};

/****************************************************************************
��ũ�θ�	: SAFE_LOCK
����		: ������ ������ ���� �ǽ���
�Ű�����	: Lock*
*****************************************************************************/
#define SAFE_LOCK(lock)     LockSafeScope __lockSafe(&lock, _W(__FILE__), __LINE__);

//---------------------------------------------------------------------------//
// ����� Ž���� ����
class LockManager : public Singleton < LockManager >
{
private:
	size_t       idSeed_;	//id ������ ���� seed

public:
	LockManager();
	~LockManager();

	HRESULT init();
	void release();

	/****************************************************************************
	�Լ���	: searchLockCycle
	����		: ����� �ɸ� Lock�� ã�� ��������
	���ϰ�	: Lock*
	�Ű�����	: Lock*
	*****************************************************************************/
	Lock* searchLockCycle(Lock *newLock);

	/****************************************************************************
	�Լ���	: checkDeadLock
	����		: ����� �ɸ� Lock�� ��������
	���ϰ�	: Lock*
	�Ű�����	: Lock*
	*****************************************************************************/
	Lock* checkDeadLock(Lock *lock);

	/****************************************************************************
	�Լ���	: generalId
	����		: ���ο� id�� ��������
	���ϰ�	: size_t
	�Ű�����	: ����
	*****************************************************************************/
	size_t generalId();
};
