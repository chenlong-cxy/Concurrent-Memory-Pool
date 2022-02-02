#include "Common.h"

class ThreadCache
{
public:
	//申请和释放内存对象
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

	//从中心缓存获取对象
	void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize);
private:
	FreeList _freeLists[NFREELIST];
};


// static保持当前文件可见
// TLS thread local storage
static __declspec(thread) ThreadCache* pTLSThreadCache = nullptr;