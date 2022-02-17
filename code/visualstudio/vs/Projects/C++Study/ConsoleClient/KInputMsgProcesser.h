////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KInputMsgProcesser.h
//  Creator     : HuaFei
//  Create Date : 2020
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../include/InputKeyDefine.h"

class KInputMsgProcesser
{
private:
	HWND m_hWnd = NULL;

private:
	KInputMsgProcesser() {};
	~KInputMsgProcesser() {};

public:
	static KInputMsgProcesser& Instance() {
		static KInputMsgProcesser s_inst;
		return s_inst;
	}

public:
	BOOL Init(Semantic::KVTable global);
	BOOL Uninit();

	BOOL MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	BOOL ProcessKeyboardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ProcessMouseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL OnKeyDown(Core::EInputKey eKey);
	BOOL OnKeyUp(Core::EInputKey eKey);

	BOOL OnMouseDown(Core::EInputKey eKey);
	BOOL OnMouseUp(Core::EInputKey eKey);
	BOOL OnMouseMove();
	BOOL OnMouseWheel(int nDelta);
};
