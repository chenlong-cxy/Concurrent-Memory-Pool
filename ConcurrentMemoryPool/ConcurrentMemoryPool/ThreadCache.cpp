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

	if (!_freeLists[index].Empty()) //�����������ϻ����ڴ��
	{
		return _freeLists[index].Pop();
	}
	else //����������Ϊ�գ���Ҫ��CentralCache����
	{
		return FetchFromCentralCache(index, alignSize);
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(ptr);
	assert(size <= MAX_BYTES);
	size_t index = SizeClass::Index(size);
	//�Ҷ�Ӧ����������Ͱ�����������
	_freeLists[index].Push(ptr);
}