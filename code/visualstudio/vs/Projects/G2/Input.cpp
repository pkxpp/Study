// Input.cpp: implementation of the Input class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Input.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Input::Input()
{

}

Input::~Input()
{

}
/////////////////////////
//GetKey State
//
//////////////////////
BOOL Input::GetKey( WORD& input1, WORD& input2 )
{
	#define KEYDOWN(key)	(buffer[key] & 0x80)
	unsigned char buffer[256];
	GetKeyboardState(buffer);
	// We have not read any data yet
	input1 = input2 = 0;

		if( KEYDOWN( VK_UP ) )
		input1 |= KEY_UP;
	else if( KEYDOWN( VK_DOWN ) )
		input1 |= KEY_DOWN;
	else if( KEYDOWN( VK_LEFT ) )
		input1 |= KEY_LEFT;
	else if( KEYDOWN( VK_RIGHT ) )
		input1 |= KEY_RIGHT;
	if( KEYDOWN( VK_SPACE ) )
		input1 |= KEY_FIRE;
	if(KEYDOWN(VK_RSHIFT))
		input1 |=KEY_SETEXPLODE;
	//Play 2
		if( KEYDOWN( 0x57 ) )//W
		input2 |= KEY_UP;
	else if( KEYDOWN( 0x53 ) )//S
		input2 |= KEY_DOWN;
	else if( KEYDOWN( 0x41 ) )//A
		input2 |= KEY_LEFT;
	else if( KEYDOWN( 0x44 ) )//D
		input2 |= KEY_RIGHT;
	if( KEYDOWN( 0x46 ) )//F
		input2 |= KEY_FIRE;
	if(KEYDOWN(VK_LSHIFT))
		input2 |=KEY_SETEXPLODE;
	return TRUE;
}
