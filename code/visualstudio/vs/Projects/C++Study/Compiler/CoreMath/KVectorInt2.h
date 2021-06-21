#pragma once

#include <stdio.h>

struct KVectorInt2
{
	int x = 0;
	int y = 0;

	KVectorInt2() : x(0), y(0) {}
	KVectorInt2(int nX, int nY) { x = nX; y = nY; }
	KVectorInt2(int nVal) { x = y = nVal; }
	KVectorInt2 operator+(const KVectorInt2& rhs) const
	{
		KVectorInt2 out(*this);
		return out += rhs;
	}
	KVectorInt2 operator-(const KVectorInt2& rhs) const
	{
		KVectorInt2 out(*this);
		return out -= rhs;
	}
	KVectorInt2& operator+=(const KVectorInt2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	KVectorInt2& operator-=(const KVectorInt2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	friend KVectorInt2 operator- (const KVectorInt2& rhs) { return KVectorInt2(-rhs.x, -rhs.y); }
	KVectorInt2 operator*(const int& rhs) const
	{
		KVectorInt2 out(*this);
		return out *= rhs;
	}
	KVectorInt2& operator*=(const int& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	KVectorInt2 operator/(const int& rhs) const
	{
		KVectorInt2 out(*this);
		return out /= rhs;
	}
	KVectorInt2& operator/=(const int& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	bool operator == (const KVectorInt2& f2) const {
		return (this->x == f2.x && this->y == f2.y);
	}
	bool operator != (const KVectorInt2& f2) const {
		return (this->x != f2.x || this->y != f2.y);
	}
	bool IsZero() const {
		return x == 0 && y == 0;
	}
	void Reset(int nX = 0, int nY = 0)
	{
		x = nX;
		y = nY;
	}
	int Distance(const KVectorInt2& other) const;

	static KVectorInt2 ZeroVector;
};
