// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

/*----------------------------------------------------------------------------
	Low level includes.
----------------------------------------------------------------------------*/
#include <locale>
#include <windows.h>
#include <functional>
#include "Public/HAL/Platform.h"
//#include "ProfilingDebugging/UMemoryDefines.h"
//#include "Misc/CoreMiscDefines.h"
//#include "Misc/CoreDefines.h"


//#include "Public/Windows/WIndowsPlatform.h"

#define CORE_API
#define STDCALL		__stdcall
#define FORCEINLINE __forceinline

#define TSAN_SAFE
#define TSAN_BEFORE(Addr)
#define TSAN_AFTER(Addr)
#define TSAN_ATOMIC(Type) Type

// Prefetch
#define PLATFORM_CACHE_LINE_SIZE	64

// Unsigned base types.
typedef unsigned char 		uint8;		// 8-bit  unsigned.
typedef unsigned short int	uint16;		// 16-bit unsigned.
typedef unsigned int		uint32;		// 32-bit unsigned.
typedef unsigned long long	uint64;		// 64-bit unsigned.

// Signed base types.
typedef	signed char			int8;		// 8-bit  signed.
typedef signed short int	int16;		// 16-bit signed.
typedef signed int	 		int32;		// 32-bit signed.
typedef signed long long	int64;		// 64-bit signed.

typedef wchar_t				TCHAR;

#ifdef _WIN64
typedef unsigned __int64	SIZE_T;
typedef __int64				SSIZE_T;
#else
typedef unsigned long		SIZE_T;
typedef long				SSIZE_T;
#endif

//
//template<typename T32BITS, typename T64BITS, int PointerSize>
//struct SelectIntPointerType
//{
//	// nothing here are is it an error if the partial specializations fail
//};
//
//template<typename T32BITS, typename T64BITS>
//struct SelectIntPointerType<T32BITS, T64BITS, 8>
//{
//	typedef T64BITS TIntPointer; // select the 64 bit type
//};
//
//template<typename T32BITS, typename T64BITS>
//struct SelectIntPointerType<T32BITS, T64BITS, 4>
//{
//	typedef T32BITS TIntPointer; // select the 32 bit type
//};
//
//typedef SelectIntPointerType<uint32, uint64, sizeof(void*)>::TIntPointer UPTRINT;	// unsigned int the same size as a pointer


enum EThreadPriority
{
	TPri_Normal,
	TPri_AboveNormal,
	TPri_BelowNormal,
	TPri_Highest,
	TPri_Lowest,
	TPri_SlightlyBelowNormal,
	TPri_TimeCritical
};

/**
	 * Return false if this is an invalid TLS slot
	 * @param SlotIndex the TLS index to check
	 * @return true if this looks like a valid slot
	 */
static FORCEINLINE bool IsValidTlsSlot(uint32 SlotIndex)
{
	return SlotIndex != 0xFFFFFFFF;
}

/**
	 * Allocates a thread local store slot.
	 *
	 * @return The index of the allocated slot.
	 */
static FORCEINLINE uint32 AllocTlsSlot(void)
{
	return ::TlsAlloc();
}

static FORCEINLINE void SetTlsValue(uint32 SlotIndex, void* Value)
{
	::TlsSetValue(SlotIndex, Value);
}

/**
 * Reads the value stored at the specified TLS slot.
 *
 * @param SlotIndex The index of the slot to read.
 * @return The value stored in the slot.
 */
static FORCEINLINE void* GetTlsValue(uint32 SlotIndex)
{
	return ::TlsGetValue(SlotIndex);
}

/**
* Frees a previously allocated TLS slot
*
* @param SlotIndex the TLS index to store it in
*/
static FORCEINLINE void FreeTlsSlot(uint32 SlotIndex)
{
	::TlsFree(SlotIndex);
}

/**
 * TRemoveReference<type> will remove any references from a type.
 */
template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

/**
 * Forward will cast a reference to an rvalue reference.
 * This is UE's equivalent of std::forward.
 */
template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}

/**
 * MoveTemp will cast a reference to an rvalue reference.
 * This is UE's equivalent of std::move except that it will not compile when passed an rvalue or
 * const object, because we would prefer to be informed when MoveTemp will have no effect.
 */
template <typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
    typedef typename TRemoveReference<T>::Type CastType;

    // Validate that we're not being passed an rvalue or a const object - the former is redundant, the latter is almost certainly a mistake
    //static_assert(TIsLValueReferenceType<T>::Value, "MoveTemp called on an rvalue");
    //static_assert(!TAreTypesEqual<CastType&, const CastType&>::Value, "MoveTemp called on a const object");

    return (CastType&&)Obj;
}