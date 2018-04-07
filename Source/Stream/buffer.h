#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>
#include <climits>

#include "CoreMinimal.h"

#include "BufferPolicies.h"

template<typename T, size_t buffsize = 1, typename AddPolicy = AddBlocking, typename RemovePolicy = RemoveBlocking>
class Buffer;

using VideoBuffer = Buffer<TArray<FColor>, 30, RemoveOldElements, RemoveBlocking>;
//using AudioBuffer = Buffer<int16, std::numeric_limits<uint64>::max()>;

/*
class Buffer represents a
thread safe buffer for producer 
consumer scenario usage.
*/
template<typename T, size_t buffsize, typename AddPolicy, typename RemovePolicy>
class Buffer
{
	static_assert( buffsize != 0, "Buffersize must be more than 0");

public:
	/*
	Buffer<T>::GetInstance() creates
	buffer of T elements. T must be 
	TArray<FColor> in case of video buffer.
	*/
	static Buffer& GetInstance() {
		static Buffer instance;
		return instance;
	}

	void add(T& num) {
		UE_LOG(LogTemp, Log, TEXT("add, size = %d"), buffer_.size());
		AddPolicy obj;
		obj(mu, cond, buffer_, buffsize, num);
		return;
	}

	T remove() {
		UE_LOG(LogTemp, Log, TEXT("remove, size = %d"), buffer_.size());
		RemovePolicy obj;
		T res = obj(mu, cond, buffer_);
		return res;
	}

private:
	Buffer() = default;
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	std::mutex mu;
	std::condition_variable cond;

	std::deque<T> buffer_;
	const size_t size_ = buffsize;
};

