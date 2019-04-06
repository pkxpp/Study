// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



#define KGLOG_PROCESS_ERROR(Condition) \
	do  \
{   \
	if (!(Condition))       \
{                       \
	/*KGLogPrintf(        \
	//KGLOG_DEBUG,    \
	//"KGLOG_PROCESS_ERROR(%s) at line %d in %s\n", #Condition, __LINE__, KG_FUNCTION  \
	//); */                 \
	goto Exit0;         \
	}                       \
	} while (false)


#define KGLOG_OUTPUT_ERROR(Condition) \
	do  \
{   \
	if (!(Condition))       \
{                       \
	/*KGLogPrintf(        \
	KGLOG_DEBUG,    \
	"KGLOG_PROCESS_ERROR(%s) at line %d in %s\n", #Condition, __LINE__, KG_FUNCTION  \
	); */                 \
	}                       \
	} while (false)

#define KGLOG_PROCESS_SUCCESS(Condition) \
	do  \
{   \
	if (Condition)          \
{                       \
	/*KGLogPrintf(        \
	KGLOG_DEBUG,    \
	"KGLOG_PROCESS_SUCCESS(%s) at line %d in %s\n", #Condition, __LINE__, KG_FUNCTION  \
	); */                 \
	goto Exit1;         \
	}                       \
	} while (false)

#define KGLOG_PROCESS_ERROR_RET_CODE(Condition, Code) \
	do  \
{   \
	if (!(Condition))       \
{                       \
	/*KGLogPrintf(        \
	KGLOG_DEBUG,    \
	"KGLOG_PROCESS_ERROR_RET_CODE(%s, %d) at line %d in %s\n", \
#Condition, Code, __LINE__, KG_FUNCTION                    \
	);   */               \
	nResult = Code;     \
	goto Exit0;         \
	}                       \
	} while (false)

#define KGLOG_PROCESS_ERROR_RET_COM_CODE(Condition, Code) \
	do  \
{   \
	if (!(Condition))       \
{                       \
	/*KGLogPrintf(        \
	KGLOG_DEBUG,    \
	"KGLOG_PROCESS_ERROR_RET_CODE(%s, %d) at line %d in %s\n", \
#Condition, Code, __LINE__, KG_FUNCTION                    \
	);      */            \
	hrResult = Code;    \
	goto Exit0;         \
	}                       \
	} while (false)


#define KGLOG_COM_PROCESS_ERROR(Condition) \
	do  \
{   \
	if (FAILED(Condition))  \
{                       \
	/*KGLogPrintf(        \
	KGLOG_DEBUG,    \
	"KGLOG_COM_PROCESS_ERROR(0x%X) at line %d in %s\n", Condition, __LINE__, KG_FUNCTION  \
	);*/                  \
	goto Exit0;         \
	}                       \
	} while (false)


#define KGLOG_COM_PROCESS_SUCCESS(Condition)   \
	do  \
{   \
	if (SUCCEEDED(Condition))   \
{                           \
	/*KGLogPrintf(            \
	KGLOG_DEBUG,        \
	"KGLOG_COM_PROCESS_SUCCESS(0x%X) at line %d in %s\n", Condition, __LINE__, KG_FUNCTION  \
	);*/                      \
	goto Exit1;             \
	}                           \
	} while (false)


// KG_COM_PROCESS_ERROR_RETURN_ERROR
#define KGLOG_COM_PROC_ERR_RET_ERR(Condition)  \
	do  \
{   \
	if (FAILED(Condition))      \
{                           \
	/*KGLogPrintf(            \
	KGLOG_DEBUG,        \
	"KGLOG_COM_PROC_ERR_RET_ERR(0x%X) at line %d in %s\n", Condition, __LINE__, KG_FUNCTION  \
	);     */                 \
	hrResult = Condition;   \
	goto Exit0;             \
	}                           \
	} while (false)


#define KGLOG_COM_PROC_ERROR_RET_CODE(Condition, Code)     \
	do  \
{   \
	if (FAILED(Condition))      \
{                           \
	/*KGLogPrintf(            \
	KGLOG_DEBUG,        \
	"KGLOG_COM_PROC_ERROR_RET_CODE(0x%X, 0x%X) at line %d in %s\n", Condition, Code, __LINE__, KG_FUNCTION  \
	);  */                    \
	hrResult = Code;        \
	goto Exit0;             \
	}                           \
	} while (false)

#define KGLOG_CHECK_ERROR(Condition) \
	do  \
{   \
	if (!(Condition))       \
{                       \
	/*KGLogPrintf(        \
	KGLOG_DEBUG,    \
	"KGLOG_CHECK_ERROR(%s) at line %d in %s\n", #Condition, __LINE__, KG_FUNCTION  \
	);  */                \
	}                       \
	} while (false)

#define KGLOG_COM_CHECK_ERROR(Condition) \
	do  \
{   \
	if (FAILED(Condition))       \
{                       \
	/*KGLogPrintf(        \
	KGLOG_DEBUG,    \
	"KGLOG_COM_CHECK_ERROR(%s) at line %d in %s\n", #Condition, __LINE__, KG_FUNCTION  \
	);   */               \
	}                       \
	} while (false)



#define KG_PROCESS_ERROR(Condition) \
	do  \
	{   \
	if (!(Condition))   \
	goto Exit0;     \
	} while (false)


#define KG_PROCESS_SUCCESS(Condition) \
	do  \
	{   \
	if (Condition)      \
	goto Exit1;     \
	} while (false)



#define KG_PROCESS_ERROR_RET_CODE(Condition, Code) \
	do  \
	{   \
	if (!(Condition))       \
		{                       \
		nResult = Code;     \
		goto Exit0;         \
		}                       \
	} while (false)

#define KG_PROCESS_ERROR_RET_COM_CODE(Condition, Code) \
	do  \
	{   \
	if (!(Condition))       \
		{                       \
		hrResult = Code;     \
		goto Exit0;         \
		}                       \
	} while (false)


#define KG_COM_PROCESS_ERROR(Condition) \
	do  \
	{   \
	if (FAILED(Condition))  \
	goto Exit0;         \
	} while (false)


#define KG_COM_PROCESS_SUCCESS(Condition)   \
	do  \
	{   \
	if (SUCCEEDED(Condition))   \
	goto Exit1;             \
	} while (false)


// KG_COM_PROCESS_ERROR_RETURN_ERROR
#define KG_COM_PROC_ERR_RET_ERR(Condition)  \
	do  \
	{   \
	if (FAILED(Condition))      \
		{                           \
		hrResult = Condition;   \
		goto Exit0;             \
		}   \
	} while (false)


#define KG_COM_PROC_ERROR_RET_CODE(Condition, Code)     \
	do  \
	{   \
	if (FAILED(Condition))  \
		{                       \
		hrResult = Code;    \
		goto Exit0;         \
		}                       \
	} while (false)


#define KG_COM_RELEASE(pInterface) \
	do  \
	{   \
	if (pInterface)                 \
		{                               \
		(pInterface)->Release();    \
		(pInterface) = NULL;        \
		}                               \
	} while (false)


#define KG_DELETE_ARRAY(pArray)     \
	do  \
	{   \
	if (pArray)                 \
		{                           \
		delete [](pArray);      \
		(pArray) = NULL;        \
		}                           \
	} while (false)


#define KG_DELETE(p)    \
	do  \
	{   \
	if (p)              \
		{                   \
		delete (p);     \
		(p) = NULL;     \
		}                   \
	} while (false)

#define KG_CHECK_ERROR(Condition) \
	do  \
	{   \
	if (!(Condition))       \
		{                       \
		}                       \
	} while (false)


#define KG_COM_CHECK_ERROR(Condition) \
	do  \
	{   \
	if (FAILED(Condition))       \
		{                       \
		}                       \
	} while (false)

// TODO: reference additional headers your program requires here



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