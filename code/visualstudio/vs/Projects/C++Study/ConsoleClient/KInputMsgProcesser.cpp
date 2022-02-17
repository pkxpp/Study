#include "stdafx.h"
#include "KInputMsgProcesser.h"
#include "../Core/Input/FInputManager.h"
#include "keyname.h"

using namespace Core;

BOOL KInputMsgProcesser::Init(Semantic::KVTable global)
{
	m_hWnd = (HWND)global.getHandle(Client::MAIN_WINDOW_HANDLE);
	KG_PROCESS_RET_FALSE(m_hWnd);

	return TRUE;
}

BOOL KInputMsgProcesser::Uninit()
{
	m_hWnd = NULL;

	return TRUE;
}

BOOL KInputMsgProcesser::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// 	case WM_ACTIVATEAPP:
		// 		ProcessKeyboardMsg(uMsg, wParam, lParam);
		// 		ProcessMouseMsg(uMsg, wParam, lParam);
		// 		break;
		// 	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		ProcessMouseMsg(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		ProcessKeyboardMsg(uMsg, wParam, lParam);
		break;
	default:
		break;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// KEYBOARD
BOOL KInputMsgProcesser::ProcessKeyboardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bProcessed = FALSE;
	BOOL bDown = FALSE;

	switch (uMsg)
	{
		//case WM_ACTIVATEAPP:
		//	return;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		bDown = TRUE;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		break;
	default:
		return FALSE;
	}

	EInputKey eInputKey1 = EInputKey::None;
	EInputKey eInputKey2 = EInputKey::None;

	int nVK = static_cast<int>(wParam);
	switch (nVK)
	{
	case VK_SHIFT:
	{
		nVK = static_cast<int>(MapVirtualKey((static_cast<UINT>(lParam) & 0x00ff0000) >> 16u, MAPVK_VSC_TO_VK_EX));
		if (nVK == VK_LSHIFT)
		{
			eInputKey1 = EInputKey::LEFT_SHIFT;
		}
		else if (nVK == VK_RSHIFT)
		{
			eInputKey1 = EInputKey::RIGHT_SHIFT;
		}
	}
	break;

	case VK_CONTROL:
	{
		eInputKey1 = (static_cast<UINT>(lParam) & 0x01000000) ? EInputKey::RIGHT_CONTROL : EInputKey::LEFT_CONTROL;
		eInputKey2 = eInputKey1;
	}
	break;
	case VK_MENU:
	{
		eInputKey1 = (static_cast<UINT>(lParam) & 0x01000000) ? EInputKey::RIGHT_ALT : EInputKey::LEFT_ALT;
		eInputKey2 = eInputKey1;
	}
	break;
	default:
	{
		if (nVK > EInputKey::None && nVK < EInputKey::KeyCount)
			eInputKey1 = (EInputKey)nVK;
	}
	break;
	}

	if (eInputKey1 != EInputKey::None)
		bProcessed = bDown ? OnKeyDown(eInputKey1) : OnKeyUp(eInputKey1);
	if (eInputKey2 != EInputKey::None)
		bProcessed |= bDown ? OnKeyDown(eInputKey2) : OnKeyUp(eInputKey2);

	return bProcessed;
}

BOOL KInputMsgProcesser::OnKeyDown(Core::EInputKey eKey)
{
	return Core::FInputManager::Instance().NotifyKeyDown(eKey);
}

BOOL KInputMsgProcesser::OnKeyUp(Core::EInputKey eKey)
{
	return Core::FInputManager::Instance().NotifyKeyUp(eKey);
}

//////////////////////////////////////////////////////////////////////////
// MOUSE
BOOL KInputMsgProcesser::ProcessMouseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bProcessed = FALSE;
	int nConvertedKey = 0;

	switch (uMsg)
	{
		//case WM_ACTIVATEAPP:
		//case WM_INPUT:
	case WM_MOUSEMOVE:
		bProcessed = OnMouseMove();
		break;
	case WM_LBUTTONDOWN:
		bProcessed = OnMouseDown(EInputKey::MouseLButton);
		break;
	case WM_LBUTTONUP:
		bProcessed = OnMouseUp(EInputKey::MouseLButton);
		break;
	case WM_RBUTTONDOWN:
		bProcessed = OnMouseDown(EInputKey::MouseRButton);
		break;
	case WM_RBUTTONUP:
		bProcessed = OnMouseUp(EInputKey::MouseRButton);
		break;
	case WM_MBUTTONDOWN:
		bProcessed = OnMouseDown(EInputKey::MouseMButton);
		break;
	case WM_MBUTTONUP:
		bProcessed = OnMouseUp(EInputKey::MouseMButton);
		break;
	case WM_MOUSEWHEEL:
	{
		int nDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		bProcessed = OnMouseWheel(nDelta);
	}
	break;
	// 	case WM_XBUTTONDOWN:
	// 		break;
	// 	case WM_XBUTTONUP:
	// 		break;
	// 	case WM_MOUSEHOVER:
	// 		bProcessed = OnMouse(VK_MOUSEHOVER, FALSE);
	// 		break;
	default:
		break;
	}

	return bProcessed;
}

BOOL KInputMsgProcesser::OnMouseDown(Core::EInputKey eKey)
{
	return Core::FInputManager::Instance().NotifyKeyDown(eKey);
}

BOOL KInputMsgProcesser::OnMouseUp(Core::EInputKey eKey)
{
	return Core::FInputManager::Instance().NotifyKeyUp(eKey);
}

BOOL KInputMsgProcesser::OnMouseMove()
{
	//

	return TRUE;
}

BOOL KInputMsgProcesser::OnMouseWheel(int nDelta)
{
	return Core::FInputManager::Instance().NotifyMouseWheel(nDelta);
}
