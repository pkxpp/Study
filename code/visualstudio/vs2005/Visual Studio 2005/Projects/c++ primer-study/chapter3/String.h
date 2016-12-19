/**
 * string.h
 */
#ifndef __STRING_H__
#define __STRING_H__

#include "String.h"
#include <iostream>

using namespace std;


class String;

istream& operator>>( istream&, String& );
ostream& operator<<( ostream&, const String& );

class String {
public:
	//
	//
	//
	String();
	String( const String&);
	String( const char* );

	//
	~String();

	//
	String& operator=( const String& );
	String& operator=( const char * );

	//
	bool operator==( const String& );
	bool operator==( const char* );

	//
	char& operator[]( int );
	
	//
	String operator+( const String &rhs ) const;

	//
	int size() { return _size; }
	char* c_str() const{ return _string; }
	//
	int count ( char ch ) const;

private:
	int _size;
	char *_string;
};

#endif /* __STRING_H__ */