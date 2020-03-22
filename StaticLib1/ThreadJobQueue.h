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

	std::queue<T>		*writeQueue_;	//input 용 여러 쓰레드에서 입력을 받음
	std::queue<T>		*readQueue_;	//output 용. 한 쓰레드에서 출력을 함

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
		readQueue_->empty();
		writeQueue_->empty();
	}

	/****************************************************************************
	함수명	: push
	설명		: writeQueue_에 t를 넣어줌
	리턴값	: 없음
	매개변수	: const T&
	*****************************************************************************/
	inline void push(const T &t)
	{
		SAFE_LOCK(lock_);
		writeQueue_->push(t);
	}

	/****************************************************************************
	함수명	: pop
	설명		: readQueue_의 첫번째 데이터를 pop함
	리턴값	: bool
	매개변수	: T&
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
	함수명	: swap
	설명		: readQueue_와 writeQueue_를 스왑함
	리턴값	: 없음
	매개변수	: 없음
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
	함수명	: swap
	설명		: readQueue_가 비었는지 확인
	리턴값	: bool
	매개변수	: 없음
	*****************************************************************************/
	inline bool isEmpty() const { return readQueue_->empty(); }

	/****************************************************************************
	함수명	: size
	설명		: WRITE_QUEUE + READ_QUEUE의 size를 리턴
	리턴값	: size_t
	매개변수	: 없음
	*****************************************************************************/
	inline size_t size()
	{
		SAFE_LOCK(lock_);
		size_t size = (size_t)(queue_[WRITE_QUEUE].size() + queue_[READ_QUEUE].size());
		return size;
	}
};