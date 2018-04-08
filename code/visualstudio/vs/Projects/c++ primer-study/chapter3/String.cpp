/**
 * string.cpp
 */
#include "String.h"

#include <cstring>
#include <cassert>
#include <iomanip>

using namespace std;

bool String::operator==( const String &rhs )
{
	if ( _size != rhs._size )
		return false;
	return strcmp( _string, rhs._string ) ? false : true;
}

bool
String::operator ==( const char *s)
{
	return strcmp( _string, s ) ? false : true;
}

String::String()
{
	_size = 0;
	_string = 0;
}

String::String(const char *str)
{
	if ( ! str )
	{
		_size = 0;
		_string = 0;
	}
	else
	{
		_size = strlen( str );
		_string = new char[ _size + 1 ];
		strcpy( _string, str );
	}
}

String::String(const String &rhs)
{
	_size = rhs._size;
	if( ! rhs._string )
		_string = 0;
	else
	{
		_string = new char[ _size + 1 ];
		strcpy( _string, rhs._string );
	}
}

String::~String() { delete [] _string; }

 String& String::operator = ( const char *s )
{
	if( !s )
	{
		_size = 0;
		delete [] _string;
		_string = 0;
	}
	else
	{
		_size = strlen( s);
		delete [] _string;
		_string = new char [ _size + 1 ];
		strcpy( _string, s );
	}
	return *this;
}

String&
String::operator=( const String &rhs )
{
	if ( this != &rhs )
	{
		delete [] _string;
		_size = rhs._size;
		if ( ! rhs._string )
			_string = 0;
		else
		{
			_string = new char [ _size + 1 ];
			strcpy( _string, rhs._string);
		}
	}
	return *this;
}

char& String::operator []( int elem )
{
	assert( elem >= 0 && elem < _size );
	return _string[elem];
}

int String::count(char ch ) const
{
	int nCnt = 0;
	for( int ix = 0; ix < _size; ix++)
	{
		if( ch == _string[ ix ])
			nCnt++;
	}
	return nCnt;
}

String String::operator+( const String &rhs ) const
{
	String newStr;
	newStr._size = _size + rhs._size;
	assert(newStr._size >= 0);
	if(newStr._size > 0)
	{
		newStr._string = new char[ newStr._size + 1 ];
		strcpy(newStr._string, _string);
		strcat(newStr._string, rhs._string);
	}
	return newStr;
}

istream&
operator>>( istream &io, String &s)
{
	//
	const int limit_string_size = 4096;
	char inBuf[ limit_string_size ];

	// setw()
	io >> setw( limit_string_size ) >> inBuf;
	s = inBuf;	// String:: operator=

	return io;
}

ostream&
operator<<(ostream& os, const String &s )
{
	return os << s.c_str();
}