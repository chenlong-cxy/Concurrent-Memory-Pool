#include "Common.h"

class ThreadCache
{
public:
	//������ͷ��ڴ����
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

	//�����Ļ����ȡ����
	void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize);
private:
	FreeList _freeLists[NFREELIST];
};


// static���ֵ�ǰ�ļ��ɼ�
// TLS thread local storage
static __declspec(thread) ThreadCache* pTLSThreadCache = nullptr;