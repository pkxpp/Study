#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>

#define SF_D3D_VERSION 11
#include <WinSock2.h>
#include <windows.h>
#include <process.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#define USE_SCALE_FORM		0
#ifndef DWORD_MAX
#define DWORD_MAX	0xffffffffUL
#endif
#include <Shlwapi.h>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <queue>

#ifndef RET_OK
#define RET_OK		1
#endif

#ifndef RET_FAIL
#define RET_FAIL	0
#endif

#ifndef SUCCESS_BREAK_CODE
#define SUCCESS_BREAK_CODE(condition, code)	if (condition) { (code) = RET_OK; break; }
#endif
#ifndef SUCCESS_BREAK
#define SUCCESS_BREAK(condition)			if (condition) break
#endif

#ifndef _DEBUG

#ifndef CHECK_ERROR
#define CHECK_ERROR(condition)	if (!(condition)) NULL
#endif

#else

#ifndef CHECK_ERROR
#define CHECK_ERROR(condition)		\
	if (!(condition))				\
		printf("CHECK_ERROR(%s) at line %d in %s\n", #condition, __LINE__, __FUNCTION__)
#endif

#endif
