#include "ThreadCache.h"

void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize)
{
	return nullptr;
}

void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAX_BYTES);
	size_t alignSize = SizeClass::RoundUp(size);
	size_t index = SizeClass::Index(size);

	if (!_freeLists[index].Empty()) //该自由链表上还有内存块
	{
		return _freeLists[index].Pop();
	}
	else //该自由链表为空，需要向CentralCache申请
	{
		return FetchFromCentralCache(index, alignSize);
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(ptr);
	assert(size <= MAX_BYTES);
	size_t index = SizeClass::Index(size);
	//找对应的自由链表桶，将对象插入
	_freeLists[index].Push(ptr);
}