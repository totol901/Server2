#pragma once

template <typename T>
class ThreadJobQueue
{
private:
	enum
	{
		WRITE_QUEUE,
		READ_QUEUE,
		MAX_QUEUE,
	};
	std::queue<T>		queue_[MAX_QUEUE];

	std::queue<T>		*writeQueue_;	//input �� ���� �����忡�� �Է��� ����
	std::queue<T>		*readQueue_;	//output ��. �� �����忡�� ����� ��

	Lock		        lock_;

public:
	ThreadJobQueue(const WCHAR* name)
		: lock_(name),
		writeQueue_(&queue_[WRITE_QUEUE]),
		readQueue_(&queue_[READ_QUEUE])
	{
	}
	~ThreadJobQueue()
	{
		//readQueue_->empty();
		//writeQueue_->empty();
	}

	/****************************************************************************
	�Լ���	: push
	����		: writeQueue_�� t�� �־���
	���ϰ�	: ����
	�Ű�����	: const T&
	*****************************************************************************/
	inline void push(const T &t)
	{
		SAFE_LOCK(lock_);
		writeQueue_->push(t);
	}

	/****************************************************************************
	�Լ���	: pop
	����		: readQueue_�� ù��° �����͸� pop��
	���ϰ�	: bool
	�Ű�����	: T&
	*****************************************************************************/
	inline bool pop(_Out_ T &t)
	{
		SAFE_LOCK(lock_);
		size_t size = this->size();
		if (size == 0)
		{
			return false;
		}
		if (readQueue_->empty())
		{
			this->swap();
		}
		t = readQueue_->front();
		readQueue_->pop();
		return true;
	}

	/****************************************************************************
	�Լ���	: swap
	����		: readQueue_�� writeQueue_�� ������
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	inline void swap()
	{
		SAFE_LOCK(lock_);
		if (writeQueue_ == &queue_[WRITE_QUEUE])
		{
			writeQueue_ = &queue_[READ_QUEUE];
			readQueue_ = &queue_[WRITE_QUEUE];
		}
		else
		{
			writeQueue_ = &queue_[WRITE_QUEUE];
			readQueue_ = &queue_[READ_QUEUE];
		}
	}

	/****************************************************************************
	�Լ���	: swap
	����		: readQueue_�� ������� Ȯ��
	���ϰ�	: bool
	�Ű�����	: ����
	*****************************************************************************/
	inline bool isEmpty() const { return readQueue_->empty(); }

	/****************************************************************************
	�Լ���	: size
	����		: WRITE_QUEUE + READ_QUEUE�� size�� ����
	���ϰ�	: size_t
	�Ű�����	: ����
	*****************************************************************************/
	inline size_t size()
	{
		SAFE_LOCK(lock_);
		size_t size = (size_t)(queue_[WRITE_QUEUE].size() + queue_[READ_QUEUE].size());
		return size;
	}
};