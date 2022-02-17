/*
 *  File   : KG_InterlockedVariable.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/12/29 18:09:14
 *  Brief  : 
 *
 *  $Id: $
 */

#ifndef __KG_INTERLOCKEDVARIABLE_H__
#define __KG_INTERLOCKEDVARIABLE_H__

#ifdef _WIN32

#ifndef _WINDOWS_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif
#include <windows.h>
#endif

// 若工程设置打开了/Wp64，则下面的代码可能会报无意义的警告，在这里关掉
#pragma warning(push)
#pragma warning(disable:4311)  // 'type cast' : pointer truncation from 'void *' to 'LONG'
#pragma warning(disable:4312)  // 'type cast' : conversion from 'LONG' to 'PVOID' of greater size

#else

#include <cstdio>

#endif // _WIN32

//------------------------------------------------------------------------------

/*
  实现说明：
  
  1. long在64位Windows和Linux之间具有不同的size，因此我们尽量避免使用long。
  
  2. 之前的版本仅暴露了3个函数，分别为：KG_InterlockedExchangeAdd, KG_InterlockedIncrement, 
     KG_InterlockedIncrement，都以long为参数类型。我们继续维持这3个函数以兼容历史代码。
  
  3. 提供一组新函数，会视情况加上不同的后缀代表参数类型，例如Int, Pointer, SizeT。
  
  4. InterlockedExchangePointer在_M_IX86下是用宏实现的，但atlconv.h中undef了宏并实现了同名的inline函数，
     这个函数的参数定义中没有volatile，因此我们在实现中类型转换时也去掉了volatile，以适应和atlconv.h共用的情况。

  5. InterlockedExchangeAdd64, InterlockedIncrement64, InterlockedDecrement64在MSDN中说明的可用平台为
     （Minimum supported client: Vista, Minimum supported server: Server 2003），符合我们Win64版本的要求。
  
  6. Linux下都用gcc的Atomic Builtins函数实现，参考http://gcc.gnu.org/onlinedocs/gcc-4.1.1/gcc/Atomic-Builtins.html。

  7. 请尽量使用新函数（带类型后缀的）。
*/

inline int KG3D_InterlockedExchangeInt(
	int volatile *Target, 
	int Value)
{
#ifdef _WIN32
	return (int)InterlockedExchange((long volatile*)Target, (long)Value);
#else
	return __sync_lock_test_and_set(Target, Value);
#endif
}

inline void* KG3D_InterlockedExchangePointer(
	void* volatile *Target,
	void* Value)
{
#ifdef _WIN32
	return InterlockedExchangePointer((void**)Target, (void*)Value);
#else
	return __sync_lock_test_and_set(Target, Value);
#endif
}

inline size_t KG3D_InterlockedExchangeSizeT(
	size_t volatile *Target,
	size_t Value)
{
#ifdef _WIN32
	return (size_t)InterlockedExchangePointer((void**)Target, (void*)Value);
#else
	return __sync_lock_test_and_set(Target, Value);
#endif
}

inline int KG3D_InterlockedExchangeAddInt(
	int volatile *Addend,
	int Value)
{
#ifdef _WIN32
	return (int)InterlockedExchangeAdd((long volatile*)Addend, (long)Value);
#else
	return __sync_fetch_and_add(Addend, Value);
#endif	
}

inline size_t KG3D_InterlockedExchangeAddSizeT(
	size_t volatile *Addend, 
	size_t Value)
{
#ifdef _WIN32
 #ifdef _WIN64_0
	return (size_t)InterlockedExchangeAdd64((long long volatile*)Addend, (long long)Value);
 #else
	return (size_t)InterlockedExchangeAdd((long volatile*)Addend, (long)Value);
 #endif
#else
	return __sync_fetch_and_add(Addend, Value);
#endif	
}

inline int KG3D_InterlockedExchangeSubInt(
	int volatile *Addend,
	int Value)
{
#ifdef _WIN32
	return (int)InterlockedExchangeAdd((long volatile*)Addend, -((long)Value));
#else
	return __sync_fetch_and_sub(Addend, Value);
#endif	
}

inline size_t KG3D_InterlockedExchangeSubSizeT(
	size_t volatile *Addend,
	size_t Value)
{
#ifdef _WIN32
 #ifdef _WIN64_0
	return (size_t)InterlockedExchangeAdd64((long long volatile*)Addend, -((long long)Value));
 #else
	return (size_t)InterlockedExchangeAdd((long volatile*)Addend, -((long)Value));
 #endif
#else
	return __sync_fetch_and_sub(Addend, Value);
#endif	
}

inline int KG3D_InterlockedCompareExchangeInt(
	int volatile *Destination,
	int ExChange,
	int Comperand)
{
#ifdef _WIN32
	return (int)InterlockedCompareExchange((long volatile*)Destination, (long)ExChange, (long)Comperand);
#else
	return __sync_val_compare_and_swap(Destination, Comperand, ExChange);
#endif
}

inline void* KG3D_InterlockedCompareExchangePointer(
	void* volatile *Destination,
	void* ExChange,
	void* Comperand)
{
#ifdef _WIN32
	return InterlockedCompareExchangePointer(Destination, ExChange, Comperand);
#else
	return __sync_val_compare_and_swap(Destination, Comperand, ExChange);
#endif
}

inline size_t KG3D_InterlockedCompareExchangeSizeT(
	size_t volatile *Destination,
	size_t ExChange,
	size_t Comperand)
{
#ifdef _WIN32
	return (size_t)InterlockedCompareExchangePointer((void* volatile *)Destination, (void*)ExChange, (void*)Comperand);
#else
	return __sync_val_compare_and_swap(Destination, Comperand, ExChange);
#endif
}

inline int KG3D_InterlockedIncrementInt(
	int volatile *lpAddend)
{
#ifdef _WIN32
	return (int)InterlockedIncrement((long volatile*)lpAddend);
#else
	return __sync_add_and_fetch(lpAddend, 1);
#endif
}

inline size_t KG3D_InterlockedIncrementSizeT(
	size_t volatile *lpAddend)
{
#ifdef _WIN32
 #ifdef _WIN64_0
	return (size_t)InterlockedIncrement64((long long volatile*)lpAddend);
 #else
	return (size_t)InterlockedIncrement((long volatile*)lpAddend);
 #endif
#else
	return __sync_add_and_fetch(lpAddend, 1);
#endif
}

inline int KG3D_InterlockedDecrementInt(
	int volatile *lpAddend)
{
#ifdef _WIN32
	return (int)InterlockedDecrement((long volatile*)lpAddend);
#else
	return __sync_sub_and_fetch(lpAddend, 1);
#endif
}

inline size_t KG3D_InterlockedDecrementSizeT(
	size_t volatile *lpAddend)
{
#ifdef _WIN32
 #ifdef _WIN64_0
	return (size_t)InterlockedDecrement64((long long volatile*)lpAddend);
 #else
	return (size_t)InterlockedDecrement((long volatile*)lpAddend);
 #endif
#else
	return __sync_sub_and_fetch(lpAddend, 1);
#endif
}

//------------------------------------------------------------------------------

#ifdef _WIN32

#define KG3D_InterlockedExchangeAdd  InterlockedExchangeAdd
#define KG3D_InterlockedIncrement    InterlockedIncrement
#define KG3D_InterlockedDecrement    InterlockedDecrement

#else

inline long KG3D_InterlockedExchangeAdd(volatile long *plValue, long lAddValue)
{
	return __sync_fetch_and_add(plValue, lAddValue);
}
inline long KG3D_InterlockedIncrement(volatile long *plValue)
{
	return __sync_add_and_fetch(plValue, 1);
}
inline long KG3D_InterlockedDecrement(volatile long *plValue)
{
	return __sync_sub_and_fetch(plValue, 1);
}

#endif

//------------------------------------------------------------------------------

#ifdef _WIN32
// 恢复警告设置
#pragma warning(pop)
#endif

#endif /* __KG_INTERLOCKEDVARIABLE_H__ */
