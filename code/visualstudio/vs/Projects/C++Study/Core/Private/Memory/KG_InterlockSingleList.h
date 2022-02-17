/*
 *  File   : KG_InterlockSingleList.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/12/29 11:58:10
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __KG_INTERLOCKSINGLELIST_H__
#define __KG_INTERLOCKSINGLELIST_H__

 /*
 A. 用法：参考MSDN中InitializeSListHead, InterlockedPushEntrySList, InterlockedPopEntrySList和InterlockedFlushSList的说明。

 B. 对齐要求：
 KG_SLIST_HEADER 需要以两倍指针大小对齐，最好使用下面的分配方式，如果自己分配需要保证对齐。

 分配：
   PKG_SLIST_HEADER KG_AllocateSListHead();
		+
   void KG_InitializeSListHead(PKG_SLIST_HEADER ListHead);
	 or
   PKG_SLIST_HEADER KG_AllocInitSListHead();

 释放：
   void KG_UninitializeSListHead(PKG_SLIST_HEADER ListHead);
		+
   void KG_FreeSListHead(PKG_SLIST_HEADER header)
	 or
   void KG_UninitFreeSListHead(PKG_SLIST_HEADER ListHead);

 C. 实现说明：
 (1) Win64中若CPU不支持cmpxchg16b指令（只有最早的几款AMD x64 CPU是这样的，基本不会碰到），则退化为加锁实现，
	 从而避免ListItem结构体需要对齐到MEMORY_ALLOCATION_ALIGNMENT边界的要求；
 (2) Linux x64下用gcc的Atomic Builtins来实现，必须在支持cmpxchg16b指令的CPU上跑（我们能碰到的服务器应该都可以）；
 */

#ifdef _WIN32

#ifndef _WINDOWS_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif
#include <windows.h>
#endif
#include <malloc.h>  // for _aligned_malloc/_aligned_free

#ifdef _WIN64_0

typedef struct _KG_SLIST_ENTRY
{
	struct _KG_SLIST_ENTRY* Next;
} KG_SLIST_ENTRY, * PKG_SLIST_ENTRY;

// windows X64虚拟内存空间最大8TB，只需要用43位整数表示,
// 也就是说指针的高(64 - 43 = 21)都是0，我们用其中的16位表示Sequence
typedef union _KG_SLIST_HEADER
{
	volatile LONGLONG ll;
	struct {
		volatile ULONGLONG Next : 48;
		volatile ULONGLONG Sequence : 16;
	};
} KG_SLIST_HEADER, * PKG_SLIST_HEADER;

inline void KG_InitializeSListHead(
	PKG_SLIST_HEADER ListHead
)
{
	ListHead->ll = 0;
}

inline void KG_UninitializeSListHead(
	PKG_SLIST_HEADER ListHead
)
{
	ListHead->ll = 0;
}

inline PKG_SLIST_ENTRY KG_InterlockedFlushSList(
	PKG_SLIST_HEADER ListHead
)
{
	KG_SLIST_HEADER OldHead, NewHead;
	for (; ; )
	{
		OldHead.ll = ListHead->ll;
		if (OldHead.Next == NULL)
		{
			break;
		}

		NewHead.Next = NULL;
		NewHead.Sequence = OldHead.Sequence + 1;

		if (InterlockedCompareExchange64(
			&(ListHead->ll), NewHead.ll, OldHead.ll) == OldHead.ll)
		{
			break;
		}
	}
	return (PKG_SLIST_ENTRY)OldHead.Next;
}

inline PKG_SLIST_ENTRY KG_InterlockedPushEntrySList(
	PKG_SLIST_HEADER ListHead,
	PKG_SLIST_ENTRY ListEntry
)
{
	KG_SLIST_HEADER OldHead, NewHead;
	for (; ; )
	{
		OldHead.ll = ListHead->ll;

		NewHead.Next = (ULONGLONG)ListEntry;
		NewHead.Sequence = OldHead.Sequence + 1;

		ListEntry->Next = (PKG_SLIST_ENTRY)OldHead.Next;

		if (InterlockedCompareExchange64(
			&(ListHead->ll), NewHead.ll, OldHead.ll) == OldHead.ll)
		{
			break;
		}
	}
	return (PKG_SLIST_ENTRY)OldHead.Next;
}

inline PKG_SLIST_ENTRY KG_InterlockedPopEntrySList(
	PKG_SLIST_HEADER ListHead
)
{
	KG_SLIST_HEADER OldHead, NewHead;
	for (; ; )
	{
		OldHead.ll = ListHead->ll;
		if (OldHead.Next == NULL)
		{
			break;
		}

		NewHead.Next = (ULONGLONG)(((PKG_SLIST_ENTRY)OldHead.Next)->Next);
		NewHead.Sequence = OldHead.Sequence + 1;

		if (InterlockedCompareExchange64(
			&(ListHead->ll), NewHead.ll, OldHead.ll) == OldHead.ll)
		{
			break;
		}
	}
	return (PKG_SLIST_ENTRY)OldHead.Next;
}

#else /* _WIN64_0 */

#define KG_SLIST_ENTRY   SLIST_ENTRY
#define PKG_SLIST_ENTRY  PSLIST_ENTRY
#define KG_SLIST_HEADER  SLIST_HEADER
#define PKG_SLIST_HEADER PSLIST_HEADER

#define KG_InitializeSListHead InitializeSListHead
inline void KG_UninitializeSListHead(PKG_SLIST_HEADER ListHead) { }
#define KG_InterlockedFlushSList InterlockedFlushSList
#define KG_InterlockedPushEntrySList InterlockedPushEntrySList
#define KG_InterlockedPopEntrySList InterlockedPopEntrySList

#endif /* _WIN64_0 */

inline PKG_SLIST_HEADER KG_AllocateSListHead()
{
	return (PKG_SLIST_HEADER)_aligned_malloc(
		sizeof(KG_SLIST_HEADER),
		MEMORY_ALLOCATION_ALIGNMENT);
}

inline void KG_FreeSListHead(
	PKG_SLIST_HEADER ListHead
)
{
	_aligned_free(ListHead);
}


#else /* _WIN32 */


#if defined(_DEBUG) && defined(WIN32)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif  // for memalign


typedef struct _KG_SLIST_ENTRY
{
	struct _KG_SLIST_ENTRY* Next;
} KG_SLIST_ENTRY, * PKG_SLIST_ENTRY;

struct _KG_SLIST_HEADER
{
	PKG_SLIST_ENTRY Next;
	char* Sequence;
} __attribute__((aligned(sizeof(void*) * 2)));
typedef struct _KG_SLIST_HEADER KG_SLIST_HEADER, * PKG_SLIST_HEADER;

#ifdef __x86_64__
typedef __int128_t dptr_int;
#else
typedef long long dptr_int;
#endif

inline void KG_InitializeSListHead(
	PKG_SLIST_HEADER ListHead
)
{
	if (!ListHead)
		return;

	ListHead->Next = NULL;
	ListHead->Sequence = NULL;
}

inline void KG_UninitializeSListHead(
	PKG_SLIST_HEADER ListHead
)
{
	(void)ListHead;
}

inline PKG_SLIST_ENTRY KG_InterlockedFlushSList(
	PKG_SLIST_HEADER ListHead
)
{
	KG_SLIST_HEADER OldHead, NewHead;
	for (; ; )
	{
		OldHead.Next = ListHead->Next;
		OldHead.Sequence = ListHead->Sequence;
		if (OldHead.Next == NULL)
		{
			break;
		}

		NewHead.Next = NULL;
		NewHead.Sequence = OldHead.Sequence + 1;

		if (__sync_bool_compare_and_swap(
			(dptr_int*)ListHead,
			*((dptr_int*)&OldHead),
			*((dptr_int*)&NewHead)))
		{
			break;
		}
	}
	return OldHead.Next;
}

inline PKG_SLIST_ENTRY KG_InterlockedPushEntrySList(
	PKG_SLIST_HEADER ListHead,
	PKG_SLIST_ENTRY ListEntry
)
{
	KG_SLIST_HEADER OldHead, NewHead;
	for (; ; )
	{
		OldHead.Next = ListHead->Next;
		OldHead.Sequence = ListHead->Sequence;

		NewHead.Next = ListEntry;
		NewHead.Sequence = OldHead.Sequence + 1;

		ListEntry->Next = OldHead.Next;

		if (__sync_bool_compare_and_swap(
			(dptr_int*)ListHead,
			*((dptr_int*)&OldHead),
			*((dptr_int*)&NewHead)))
		{
			break;
		}
	}
	return OldHead.Next;
}

inline PKG_SLIST_ENTRY KG_InterlockedPopEntrySList(
	PKG_SLIST_HEADER ListHead
)
{
	KG_SLIST_HEADER OldHead, NewHead;
	for (; ; )
	{
		OldHead.Next = ListHead->Next;
		OldHead.Sequence = ListHead->Sequence;
		if (OldHead.Next == NULL)
		{
			break;
		}

		NewHead.Next = OldHead.Next->Next;
		NewHead.Sequence = OldHead.Sequence + 1;

		if (__sync_bool_compare_and_swap(
			(dptr_int*)ListHead,
			*((dptr_int*)&OldHead),
			*((dptr_int*)&NewHead)))
		{
			break;
		}
	}
	return OldHead.Next;
}


#if defined(_DEBUG) && defined(WIN32)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif

inline PKG_SLIST_HEADER KG_AllocateSListHead()
{
	PKG_SLIST_HEADER p = 0;

#if defined(__APPLE__)
	posix_memalign((void**)&p, sizeof(void*) * 2,
		sizeof(KG_SLIST_HEADER));
#elif defined(ANDROID)
	return (PKG_SLIST_HEADER)memalign(
		sizeof(void*) * 2,
		sizeof(KG_SLIST_HEADER));
#elif defined(__linux__)
	posix_memalign((void**)&p, sizeof(void*) * 2,
		sizeof(KG_SLIST_HEADER));
#else
	return (PKG_SLIST_HEADER)memalign(
		sizeof(void*) * 2,
		sizeof(KG_SLIST_HEADER));
#endif


	//#if defined(__APPLE__) | defined(__linux__)
	//    posix_memalign((void **)&p, sizeof(void*) * 2,
	//                   sizeof(KG_SLIST_HEADER));
	//    //posix_memalign(<#void **#>, size_t, <#size_t#>)
	//#else
	//	return (PKG_SLIST_HEADER)memalign(
	//		sizeof(void*) * 2,
	//		sizeof(KG_SLIST_HEADER));
	//#endif
	return p;
}

inline void KG_FreeSListHead(
	PKG_SLIST_HEADER ListHead
)
{
	free(ListHead);
}

#endif /* _WIN32 */


inline PKG_SLIST_HEADER KG_AllocInitSListHead()
{
	PKG_SLIST_HEADER ListHead = KG_AllocateSListHead();
	if (ListHead == NULL)
		return NULL;
	KG_InitializeSListHead(ListHead);
	return ListHead;
}

inline void KG_UninitFreeSListHead(
	PKG_SLIST_HEADER ListHead
)
{
	KG_UninitializeSListHead(ListHead);
	KG_FreeSListHead(ListHead);
}

//------------------------------------------------------------------------------

#ifndef CONTAINING_RECORD
#define CONTAINING_RECORD(address, type, field) ((type *)( \
	(char *)(address) - \
	(ptrdiff_t)(&((type *)0)->field)))
#endif

//------------------------------------------------------------------------------

#endif /* __KG_INTERLOCKSINGLELIST_H__ */
