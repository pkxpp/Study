// Input.h: interface for the Input class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUT_H__16F70131_066A_41D4_BBF7_C7ABC9B37197__INCLUDED_)
#define AFX_INPUT_H__16F70131_066A_41D4_BBF7_C7ABC9B37197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////
#define KEY_UP				0x0001
#define	KEY_RIGHT			0x0002
#define KEY_DOWN			0x0004
#define KEY_LEFT			0x0008
#define KEY_DIRECTION		0x000f
#define KEY_FIRE			0x0010
#define KEY_SETEXPLODE		0x0020
/////////////////////////////
class Input  
{
public:
	Input();
	virtual ~Input();
	BOOL GetKey( WORD& input1, WORD& input2 );

};

#endif // !defined(AFX_INPUT_H__16F70131_066A_41D4_BBF7_C7ABC9B37197__INCLUDED_)
