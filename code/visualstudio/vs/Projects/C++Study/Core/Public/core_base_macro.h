////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : core_base_macro.h
//  Creator     : HuaFei
//  Create Date : 2020
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "log/KLog.h"
#include <cassert>

typedef unsigned int UINT;
typedef unsigned char byte;
typedef unsigned char BYTE;

#ifdef WIN32
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef int BOOL;

#else
/* The ISO C99 standard specifies that in C++ implementations number limits
	macros should only be defined if explicitly requested.  */
#define __STDC_LIMIT_MACROS

#include <limits.h>
#include <stdint.h>

	//typedef unsigned int   uintptr_t;

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef unsigned long  ULONG;
typedef unsigned short USHORT;
typedef unsigned char  UCHAR;
typedef int   BOOL;
typedef long  LONG;
typedef void* HANDLE;

#ifdef __APPLE__
typedef int32_t HRESULT;
#elif defined(ANDROID)
typedef int32_t HRESULT;
#else
typedef long HRESULT;
#endif

#define FALSE   0
#define TRUE    1

typedef struct tagRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, * PRECT, * NPRECT, * LPRECT;

typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, * PPOINT, * NPPOINT, * LPPOINT;

typedef struct tagSIZE
{
	LONG        cx;
	LONG        cy;
} SIZE, * PSIZE, * LPSIZE;

typedef const RECT* LPCRECT;

typedef SIZE               SIZEL;
typedef SIZE* PSIZEL, * LPSIZEL;

#endif



#ifdef __cplusplus
#	undef  NULL
#	define NULL    0
#else
#	ifndef NULL
#	define NULL    ((void *)0)
#	endif
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

#ifndef	countof
#define countof(array)					(sizeof(array)/sizeof(array[0]))
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(a)	if (a) {free(a); (a)=NULL;}
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(a)	if (a) {(a)->Release(); (a)=NULL;}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(a)	if (a) {delete (a); (a)=NULL;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a)	if (a) {delete[] (a); (a)=NULL;}
#endif

#define COMBO_2SHORT(a, b)	((int)(((short)((a) & 0xffff)) | ((int)((short)((b) & 0xffff))) << 16))
#define LOSHORT(l)			((short)((l) & 0xffff))
#define HISHORT(l)			((short)((l) >> 16))

#define g_StrCpyLen(dest, src, size)	\
		{								\
		strncpy(dest, src, size);		\
		dest[size - 1] = '\0';			\
		}

#define g_StrPrintf(dest, size, format, args)	\
		{								\
		snprintf(dest, size - 1, format, args);		\
		dest[size - 1] = '\0';			\
		}

#define g_StrLower(str)									\
		{												\
			char c;										\
			for (int i = 0; 0 != (c = str[i]); i++)		\
			{											\
				if (c >= 'A' && c <= 'Z')				\
					str[i] = c + ('a' - 'A');			\
			}											\
		}

#ifdef _MSC_VER
#define TEMP_DISABLE_WARNING(warningCode, expression)   \
        __pragma(warning(push))                             \
        __pragma(warning(disable:warningCode))              \
        expression                                          \
        __pragma(warning(pop))
#else
#define TEMP_DISABLE_WARNING(warningCode, expression)   \
        expression
#endif

// _MSC_VER warning C4127: conditional expression is constant
// use this macro instead "WHILE_FALSE_NO_WARNING", you can enjoy the level 4 warning ^_^
#define WHILE_FALSE_NO_WARNING  \
    TEMP_DISABLE_WARNING(4127, while (false))

#define KG_USE_ARGUMENT(arg) (arg)

#ifdef KG_PROCESS_ERROR
#undef KG_PROCESS_ERROR
#endif
#define KG_PROCESS_ERROR(Condition) \
    do  \
    {   \
        if (!(Condition))   \
            goto Exit0;     \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_SUCCESS
#undef KG_PROCESS_SUCCESS
#endif
#define KG_PROCESS_SUCCESS(Condition) \
    do  \
    {   \
        if (Condition)      \
            goto Exit1;     \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_RET
#undef KG_PROCESS_RET
#endif
#define KG_PROCESS_RET(Condition) \
	do  \
	{   \
		if (!(Condition))       \
			return;         \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_RET_FALSE
#undef KG_PROCESS_RET_FALSE
#endif
#define KG_PROCESS_RET_FALSE(Condition) \
	do  \
	{   \
		if (!(Condition))       \
			return FALSE;       \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_RET_ZERO
#undef KG_PROCESS_RET_ZERO
#endif
#define KG_PROCESS_RET_ZERO(Condition) \
	do  \
	{   \
		if (!(Condition))       \
			return 0;       \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_RET_CODE
#undef KG_PROCESS_RET_CODE
#endif
#define KG_PROCESS_RET_CODE(Condition, Code) \
	do  \
	{   \
		if (!(Condition))       \
			return Code;       \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_RET_NULL
#undef KG_PROCESS_RET_NULL
#endif
#define KG_PROCESS_RET_NULL(Condition) \
	do  \
	{   \
		if (!(Condition))       \
			return NULL;        \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROCESS_RET
#undef KG_COM_PROCESS_RET
#endif
#define KG_COM_PROCESS_RET(Condition) \
	do  \
	{   \
		if (FAILED(Condition))       \
			return;         \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROCESS_RET_FALSE
#undef KG_COM_PROCESS_RET_FALSE
#endif
#define KG_COM_PROCESS_RET_FALSE(Condition) \
	do  \
	{   \
		if (FAILED(Condition))       \
			return FALSE;       \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROCESS_RET_ZERO
#undef KG_COM_PROCESS_RET_ZERO
#endif
#define KG_COM_PROCESS_RET_ZERO(Condition) \
	do  \
	{   \
		if (FAILED(Condition))       \
			return 0;       \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROCESS_RET_NULL
#undef KG_COM_PROCESS_RET_NULL
#endif
#define KG_COM_PROCESS_RET_NULL(Condition) \
	do  \
	{   \
		if (FAILED(Condition))       \
			return NULL;        \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROCESS_RET_CODE
#undef KG_COM_PROCESS_RET_CODE
#endif
#define KG_COM_PROCESS_RET_CODE(Condition, Code)     \
    do  \
    {   \
        if (FAILED(Condition))  \
        {                       \
            return Code;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_ERROR_RET_CODE
#undef KG_PROCESS_ERROR_RET_CODE
#endif
#define KG_PROCESS_ERROR_RET_CODE(Condition, Code) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
            nResult = Code;     \
            goto Exit0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_PROCESS_ERROR_RET_COM_CODE
#undef KG_PROCESS_ERROR_RET_COM_CODE
#endif
#define KG_PROCESS_ERROR_RET_COM_CODE(Condition, Code) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
            hrResult = Code;     \
            goto Exit0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROCESS_ERROR
#undef KG_COM_PROCESS_ERROR
#endif
#define KG_COM_PROCESS_ERROR(Condition) \
    do  \
    {   \
        if (FAILED(Condition))  \
            goto Exit0;         \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROCESS_SUCCESS
#undef KG_COM_PROCESS_SUCCESS
#endif
#define KG_COM_PROCESS_SUCCESS(Condition)   \
    do  \
    {   \
        if (SUCCEEDED(Condition))   \
            goto Exit1;             \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROC_ERR_RET_ERR
#undef KG_COM_PROC_ERR_RET_ERR
#endif
#define KG_COM_PROC_ERR_RET_ERR(Condition)  \
    do  \
    {   \
        if (FAILED(Condition))      \
        {                           \
            hrResult = Condition;   \
            goto Exit0;             \
        }   \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_COM_PROC_ERROR_RET_CODE
#undef KG_COM_PROC_ERROR_RET_CODE
#endif
#define KG_COM_PROC_ERROR_RET_CODE(Condition, Code)     \
    do  \
    {   \
        if (FAILED(Condition))  \
        {                       \
            hrResult = Code;    \
            goto Exit0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_COM_RELEASE
#undef KG_COM_RELEASE
#endif
#define KG_COM_RELEASE(pInterface) \
    do  \
    {   \
        if (pInterface)                 \
        {                               \
            (pInterface)->Release();    \
            (pInterface) = NULL;        \
        }                               \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_DELETE_ARRAY
#undef KG_DELETE_ARRAY
#endif
#define KG_DELETE_ARRAY(pArray)     \
    do  \
    {   \
        if (pArray)                 \
        {                           \
            delete [](pArray);      \
            (pArray) = NULL;        \
        }                           \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_DELETE
#undef KG_DELETE
#endif
#define KG_DELETE(p)    \
    do  \
    {   \
        if (p)              \
        {                   \
            delete (p);     \
            (p) = NULL;     \
        }                   \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_FREE
#undef KG_FREE
#endif
#define KG_FREE(p)    \
    do  \
    {   \
        if (p)              \
        {                   \
            free(p);        \
            (p) = NULL;     \
        }                   \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_CHECK_ERROR
#undef KG_CHECK_ERROR
#endif
#define KG_CHECK_ERROR(Condition) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
			//LOGE("KG_CHECK_ERROR(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__);	\
        }                       \
    } WHILE_FALSE_NO_WARNING

#ifdef KG_COM_CHECK_ERROR
#undef KG_COM_CHECK_ERROR
#endif
#define KG_COM_CHECK_ERROR(Condition) \
    do  \
    {   \
        if (FAILED(Condition))       \
        {                       \
			//LOGE("KG_COM_CHECK_ERROR(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__);	\
        }                       \
    } WHILE_FALSE_NO_WARNING

#ifdef KGLOG_ASSERT_EXIT
#undef KGLOG_ASSERT_EXIT
#endif
#define KGLOG_ASSERT_EXIT KGLOG_PROCESS_ERROR

#ifdef KG_PROCESS_SUCCESS
#undef KG_PROCESS_SUCCESS
#endif
#define KG_PROCESS_SUCCESS(Condition) \
	do	\
	{	\
		if (Condition)	\
		goto Exit1;	\
	} while (false)

#ifdef KG_SUCCESS_RET_NULL
#undef KG_SUCCESS_RET_NULL
#endif
#define KG_SUCCESS_RET_NULL(Condition) \
	do  \
	{   \
		if (Condition)       \
			return NULL;        \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_SUCCESS_RET_ZERO
#undef KG_SUCCESS_RET_ZERO
#endif
#define KG_SUCCESS_RET_ZERO(Condition) \
	do  \
	{   \
		if (Condition)       \
			return 0;        \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_SUCCESS_RET_TRUE
#undef KG_SUCCESS_RET_TRUE
#endif
#define KG_SUCCESS_RET_TRUE(Condition) \
	do  \
	{   \
		if (Condition)       \
			return TRUE;        \
	} WHILE_FALSE_NO_WARNING

#ifdef KG_SUCCESS_RET_CODE
#undef KG_SUCCESS_RET_CODE
#endif
#define KG_SUCCESS_RET_CODE(Condition, Code) \
	do  \
	{   \
		if (Condition)       \
			return Code;        \
	} WHILE_FALSE_NO_WARNING

#ifdef KGLOG_PROCESS_ERROR
#undef KGLOG_PROCESS_ERROR
#endif
#define KGLOG_PROCESS_ERROR(Condition) \
	do  \
	{   \
		if (!(Condition))       \
		{                       \
			//LOGE("KGLOG_PROCESS_ERROR[%s][%s][%d](%s)",__FILE__, __FUNCTION__, __LINE__, #Condition); \
			goto Exit0;         \
		}                       \
	} while (false)

#define HTTP_ERROR(Condition) \
	do  \
	{   \
		if (!(Condition))       \
			{                       \
			printf("HTTP_ERROR(%s)", #Condition); \
			goto Exit0;         \
			}                       \
	} while (false)

#ifndef KGLOG_CHECK_ERROR
#define KGLOG_CHECK_ERROR(condition)		\
	do  \
	{   \
		if (!(condition)) 				\
			printf("CHECK_ERROR(%s) at line %d in %s\n", #condition, __LINE__, __FUNCTION__);	\
	} while (false)
#endif

#define KGLOG_PROCESS_RET(Condition) \
	do  \
	{   \
		if (!(Condition))	\
		{	\
			printf("KGLOG_PROCESS_RET(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__);  \
			return;         \
		}	\
	} WHILE_FALSE_NO_WARNING

#define KGLOG_PROCESS_RET_FALSE(Condition) \
	do  \
	{   \
		if (!(Condition))       \
		{                       \
			printf("KGLOG_PROCESS_RET_FALSE(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__);  \
			return FALSE;         \
		}                       \
	} WHILE_FALSE_NO_WARNING

#define KGLOG_PROCESS_RET_NULL(Condition) \
	do  \
	{   \
		if (!(Condition))       \
		{                       \
			printf("KGLOG_PROCESS_RET_NULL(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__);   \
			return NULL;         \
		}                       \
	} WHILE_FALSE_NO_WARNING

#define KGLOG_PROCESS_RET_ZERO(Condition) \
	do  \
	{   \
		if (!(Condition))       \
		{                       \
			printf("KGLOG_PROCESS_RET_ZERO(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__);   \
			return 0;         \
		}                       \
	} WHILE_FALSE_NO_WARNING

#define KGLOG_COM_PROCESS_RET_FALSE(Condition) \
	do  \
    {   \
	    if (FAILED(Condition))  \
        {                       \
	        printf("KGLOG_COM_PROCESS_RET_FALSE(0x%X) at line %d in %s\n", (Condition), __LINE__, __FUNCTION__);    \
	        return FALSE;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#define KGLOG_COM_PROCESS_RET_ZERO(Condition) \
	do  \
    {   \
	    if (FAILED(Condition))  \
        {                       \
	        printf("KGLOG_COM_PROCESS_RET_ZERO(0x%X) at line %d in %s\n", (Condition), __LINE__, __FUNCTION__);    \
	        return 0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#define KGLOG_COM_PROCESS_RET_NULL(Condition) \
	do  \
    {   \
	    if (FAILED(Condition))  \
        {                       \
	        printf("KGLOG_COM_PROCESS_RET_NULL(0x%X) at line %d in %s\n", (Condition), __LINE__, __FUNCTION__);    \
	        return NULL;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#define KGLOG_COM_PROCESS_RET(Condition) \
	do  \
	{   \
		if (FAILED(Condition))	\
		{	\
			printf("KGLOG_COM_PROCESS_RET(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__);  \
			return;         \
		}	\
	} WHILE_FALSE_NO_WARNING

#ifndef KG_ASSERT_EXIT
#ifdef _DEBUG
#define KG_ASSERT_EXIT(Condition) \
    do  \
    {   \
        assert(Condition);  \
        if (!(Condition))   \
            goto Exit0;     \
    } WHILE_FALSE_NO_WARNING
#else
#define KG_ASSERT_EXIT(Condition)  \
    do  \
    {   \
        if (!(Condition))       \
            goto Exit0;         \
    } WHILE_FALSE_NO_WARNING
#endif
#endif

#ifndef _DEBUG

#ifndef ERROR_BREAK
#define ERROR_BREAK(condition)				if (!(condition)) break
#endif

#else

#ifndef ERROR_BREAK
#define ERROR_BREAK(condition)		\
	if (!(condition))				\
	{								\
		printf("ERROR_BREAK(%s) at line %d in %s\n", #condition, __LINE__, __FUNCTION__);		\
		break;						\
	} NULL
#endif

#endif

#ifdef _DEBUG
#define KASSERT(x)	assert(x)
#else
#define KASSERT(x)
#endif

#ifndef ASSERT
#define ASSERT assert
#endif

#define KG_COM_ASSERT_EXIT KG_COM_PROCESS_ERROR

#define KGLOG_COM_PROCESS_ERROR(hr) KGLOG_PROCESS_ERROR(hr == 0)

// ANDROID and IOS need this header
#if defined(ANDROID) || defined(__APPLE__)
#include <unistd.h>
#endif

#if !defined(WIN32)
#define KSLEEP(X)  usleep((X) * 1000)
#else
#define KSLEEP(X)  ::Sleep(X)
#endif

//#ifdef WIN32
////typedef _W64 unsigned int   UINT_PTR, *PUINT_PTR;
//typedef unsigned  __int64 UINT64;
//typedef __int64 INT64;
//#else
//typedef uintptr_t UINT_PTR, * PUINT_PTR;
//typedef uint64_t UINT64;
//typedef int64_t INT64;
//#endif

#if !defined(WIN32) && !defined(_WIN64_0)
#define ZeroMemory(address, size) memset(address, 0, size)
#endif

#ifndef MAX
#define MAX(l, r)			((l) > (r) ? (l) : (r))		
#endif

#ifndef MIN
#define MIN(l, r)			((l) > (r) ? (r) : (l))		
#endif

//#include "KG_COMTypes.h"
