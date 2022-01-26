#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <cassert>
using std::cout;
using std::endl;

static const size_t MAX_BYTES = 256 * 1024;
static const size_t NFREELIST = 208;

static void*& ObjNext(void* obj)
{
	return *(void**)obj;
}

//�����зֺõ�С�������������
class FreeList
{
public:
	void Push(void* obj)
	{
		assert(obj);
		//ͷ��
		ObjNext(obj) = _freeList;
		_freeList = obj;
	}
	void* Pop()
	{
		assert(_freeList);
		//ͷɾ
		void* obj = _freeList;
		_freeList = ObjNext(obj);
		return obj;
	}
	bool Empty()
	{
		return _freeList == nullptr;
	}
private:
	void* _freeList = nullptr;
};

//��������С�Ķ���ӳ�����
class SizeClass
{
public:
	// ������������10%���ҵ�����Ƭ�˷�
	// [1,128]					8byte����	    freelist[0,16)
	// [128+1,1024]				16byte����	    freelist[16,72)
	// [1024+1,8*1024]			128byte����	    freelist[72,128)
	// [8*1024+1,64*1024]		1024byte����     freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte����   freelist[184,208)

	static inline size_t _RoundUp(size_t size, size_t alignNum)
	{
		return ((size + alignNum - 1)&~(alignNum - 1));
	}
	static inline size_t RoundUp(size_t size)
	{
		if (size <= 128)
		{
			return _RoundUp(size, 8);
		}
		else if (size <= 1024)
		{
			return _RoundUp(size, 16);
		}
		else if (size <= 8 * 1024)
		{
			return _RoundUp(size, 128);
		}
		else if (size <= 64 * 1024)
		{
			return _RoundUp(size, 1024);
		}
		else if (size <= 256 * 1024)
		{
			return _RoundUp(size, 8 * 1024);
		}
		else
		{
			assert(false);
			return -1;
		}
	}
	static inline size_t _Index(size_t size, size_t align_shift)
	{
		return ((size + (1 << align_shift) - 1) >> align_shift) - 1;
	}
	//����ӳ�����һ����������Ͱ
	static inline size_t Index(size_t size)
	{
		//ÿ�������ж��ٸ�����Ͱ
		static int group_array[4] = { 16, 56, 56, 56 };
		if (size <= 128)
		{
			return _Index(size, 3);
		}
		else if (size <= 1024)
		{
			return _Index(size - 128, 4) + group_array[0];
		}
		else if (size <= 8 * 1024)
		{
			return _Index(size - 1024, 7) + group_array[0] + group_array[1];
		}
		else if (size <= 64 * 1024)
		{
			return _Index(size - 8 * 1024, 10) + group_array[0] + group_array[1] + group_array[2];
		}
		else if (size <= 256 * 1024)
		{
			return _Index(size - 64 * 1024, 13) + group_array[0] + group_array[1] + group_array[2] + group_array[3];
		}
		else
		{
			assert(false);
			return -1;
		}
	}
};