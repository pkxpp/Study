#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <iostream>

#if defined(_WIN32)
#include <DirectXMath.h>
#endif


#define SMALL_NUMBER			(1.e-8f)
#define KINDA_SMALL_NUMBER		(1.e-4f)
#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)

#undef PI
#define PI						(3.1415926535897932f)	/* Extra digits if needed: 3.1415926535897932384626433832795f */
#define SMALL_NUMBER			(1.e-8f)
#define KINDA_SMALL_NUMBER		(1.e-4f)
#define BIG_NUMBER				(3.4e+38f)
#define EULERS_NUMBER			(2.71828182845904523536f)

class KMathTest {
public:
    static float Sqrt(float Value) { return sqrtf(Value); }

    /** Computes absolute value in a generic way */
    template< class T >
    static constexpr inline T Abs(const T A)
    {
        return (A >= (T)0) ? A : -A;
    }

    /** Returns 1, 0, or -1 depending on relation of T to 0 */
    template< class T >
    static constexpr inline T Sign(const T A)
    {
        return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
    }

    /** Returns higher value in a generic way */
    template< class T >
    static constexpr inline T Max(const T A, const T B)
    {
        return (A >= B) ? A : B;
    }

    /** Returns lower value in a generic way */
    template< class T >
    static constexpr inline T Min(const T A, const T B)
    {
        return (A <= B) ? A : B;
    }

    static inline void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (INV_PI * 0.5f) * Value;
        if (Value >= 0.0f)
        {
            quotient = (float)((int)(quotient + 0.5f));
        }
        else
        {
            quotient = (float)((int)(quotient - 0.5f));
        }
        float y = Value - (2.0f * PI) * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;
        if (y > HALF_PI)
        {
            y = PI - y;
            sign = -1.0f;
        }
        else if (y < -HALF_PI)
        {
            y = -PI - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        // 11-degree minimax approximation
        *ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

        // 10-degree minimax approximation
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        *ScalarCos = sign * p;
    }

    /**
     * Converts degrees to radians.
     * @param	DegVal			Value in degrees.
     * @return					Value in radians.
     */
    template<class T>
    static inline auto DegreesToRadians(T const& DegVal) -> decltype(DegVal* (PI / 180.f))
    {
        return DegVal * (PI / 180.f);
    }

    /** Computes a fully accurate inverse square root */
    static  float InvSqrt(float F)
    {
        return 1.0f / sqrtf(F);
    }
};


/*!***************************************************************************
 2D floating point vector
 *****************************************************************************/
struct KVECTOR2Test
{
	float x;	/*!< x coordinate */
	float y;	/*!< y coordinate */
};

/*!***************************************************************************
	** KVec2Test 2 component vector
	****************************************************************************/
struct KVec2Test : public KVECTOR2Test
{
	/*!***************************************************************************
		** Constructors
		****************************************************************************/
		/*!***************************************************************************
			@Function			KVec2Test
			@Description		Blank constructor.
			*****************************************************************************/
	KVec2Test() { x = 0; y = 0; }
	/*!***************************************************************************
		@Function			KVec2Test
		@Input				fX	X component of vector
		@Input				fY	Y component of vector
		@Description		Simple constructor from 2 values.
		*****************************************************************************/
	KVec2Test(float fX, float fY) { x = fX; y = fY; }
	/*!***************************************************************************
		@Function			KVec2Test
		@Input				fValue	a component value
		@Description		Constructor from a single value.
		*****************************************************************************/
	KVec2Test(float fValue) { x = fValue; y = fValue; }
	/*!***************************************************************************
		@Function			KVec2Test
		@Input				pVec	an array
		@Description		Constructor from an array
		*****************************************************************************/
	KVec2Test(const float* pVec) { x = pVec[0]; y = pVec[1]; }
	/*!***************************************************************************
		@Function			KVec2Test
		@Input				v3Vec a Vec3
		@Description		Constructor from a Vec3
		*****************************************************************************/
		//	KVec2Test(const Vec3& v3Vec);
			/*!***************************************************************************
				** Operators
				****************************************************************************/
				/*!***************************************************************************
					@Function			+
					@Input				rhs another KVec2Test
					@Returns			result of addition
					@Description		componentwise addition operator for two Vec2s
					*****************************************************************************/
	KVec2Test operator+(const KVec2Test& rhs) const
	{
		KVec2Test out(*this);
		return out += rhs;
	}
	/*!***************************************************************************
		@Function			-
		@Input				rhs another KVec2Test
		@Returns			result of subtraction
		@Description		componentwise subtraction operator for two Vec2s
		****************************************************************************/
	KVec2Test operator-(const KVec2Test& rhs) const
	{
		KVec2Test out(*this);
		return out -= rhs;
	}

	/*!***************************************************************************
		@Function			+=
		@Input				rhs another KVec2Test
		@Returns			result of addition
		@Description		componentwise addition and assignment operator for two Vec2s
		****************************************************************************/
	KVec2Test& operator+=(const KVec2Test& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	/*!***************************************************************************
		@Function			-=
		@Input				rhs another KVec2Test
		@Returns			result of subtraction
		@Description		componentwise subtraction and assignment operator for two Vec2s
		****************************************************************************/
	KVec2Test& operator-=(const KVec2Test& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	/*!***************************************************************************
		@Function			-
		@Input				rhs another KVec2Test
		@Returns			result of negation
		@Description		negation operator for a KVec2Test
		****************************************************************************/
	friend KVec2Test operator- (const KVec2Test& rhs) { return KVec2Test(-rhs.x, -rhs.y); }

	/*!***************************************************************************
		@Function			*
		@Input				lhs scalar
		@Input				rhs a KVec2Test
		@Returns			result of negation
		@Description		negation operator for a KVec2Test
		****************************************************************************/
	friend KVec2Test operator*(const float lhs, const KVec2Test& rhs)
	{
		KVec2Test out(lhs);
		return out *= rhs;
	}

	/*!***************************************************************************
		@Function			/
		@Input				lhs scalar
		@Input				rhs a KVec2Test
		@Returns			result of division
		@Description		division operator for scalar and KVec2Test
		****************************************************************************/
	friend KVec2Test operator/(const float lhs, const KVec2Test& rhs)
	{
		KVec2Test out(lhs);
		return out /= rhs;
	}

	/*!***************************************************************************
		@Function			*
		@Input				rhs a scalar
		@Returns			result of multiplication
		@Description		componentwise multiplication by scalar for KVec2Test
		****************************************************************************/
	KVec2Test operator*(const float& rhs) const
	{
		KVec2Test out(*this);
		return out *= rhs;
	}

	/*!***************************************************************************
		@Function			*=
		@Input				rhs a scalar
		@Returns			result of multiplication and assignment
		@Description		componentwise multiplication and assignment by scalar for KVec2Test
		****************************************************************************/
	KVec2Test& operator*=(const float& rhs)
	{
		x = (float)(x * rhs);
		y = (float)(y * rhs);
		return *this;
	}

	/*!***************************************************************************
		@Function			*=
		@Input				rhs a KVec2Test
		@Returns			result of multiplication and assignment
		@Description		componentwise multiplication and assignment by KVec2Test for KVec2Test
		****************************************************************************/
	KVec2Test& operator*=(const KVec2Test& rhs)
	{
		x = (float)(x * rhs.x);
		y = (float)(y * rhs.y);
		return *this;
	}

	/*!***************************************************************************
		@Function			/
		@Input				rhs a scalar
		@Returns			result of division
		@Description		componentwise division by scalar for KVec2Test
		****************************************************************************/
	KVec2Test operator/(const float& rhs) const
	{
		KVec2Test out(*this);
		return out /= rhs;
	}

	/*!***************************************************************************
		@Function			/=
		@Input				rhs a scalar
		@Returns			result of division and assignment
		@Description		componentwise division and assignment by scalar for KVec2Test
		****************************************************************************/
	KVec2Test& operator/=(const float& rhs)
	{
        x = (float)(x / rhs);
		y = (float)(y/ rhs);
		return *this;
	}

	/*!***************************************************************************
		@Function			/=
		@Input				rhs a KVec2Test
		@Returns			result of division and assignment
		@Description		componentwise division and assignment by KVec2Test for KVec2Test
		****************************************************************************/
	KVec2Test& operator/=(const KVec2Test& rhs)
	{
		x = (float)(x / rhs.x);
		y = (float)(y / rhs.y);
		return *this;
	}
	// FUNCTIONS
	/*!***************************************************************************
		@Function			lenSqr
		@Returns			the square of the magnitude of the vector
		@Description		calculates the square of the magnitude of the vector
		****************************************************************************/
	double lenSqr() const
	{
		//return floatMUL(x,x)+floatMUL(y,y);
		return (double)x * (double)x + (double)y * (double)y;
	}

	/*!***************************************************************************
		@Function			length
		@Returns			the of the magnitude of the vector
		@Description		calculates the magnitude of the vector
		****************************************************************************/
	float length() const;

	/*!***************************************************************************
		@Function			normalize
		@Returns			the normalized value of the vector
		@Description		normalizes the vector
		****************************************************************************/
	KVec2Test normalize()
	{
		float flen = length();
		if (flen)
		{
			*this /= flen;
		}
		return *this;
	}

	/*!***************************************************************************
		@Function			normalized
		@Returns			returns the normalized value of the vector
		@Description		returns a normalized vector of the same direction as this
		vector
		****************************************************************************/
	KVec2Test normalized() const
	{
		KVec2Test out(*this);
		return out.normalize();
	}

	/*!***************************************************************************
		@Function			rotated90
		@Returns			returns the vector rotated 90ï¾?		@Description		returns the vector rotated 90ï¾?		****************************************************************************/
	KVec2Test rotated90() const
	{
		return KVec2Test(-y, x);
	}

	KVec2Test rotated(const float& angle) const
	{
		return KVec2Test(x * cosf(angle) - y * sinf(angle),
			x * sinf(angle) + y * cosf(angle));
	}

	float cross(const KVec2Test& rhs) const
	{
		return (float)(x * rhs.y) - (float)(y * rhs.x);
	}

	/*!***************************************************************************
		@Function			dot
		@Input				rhs a single value
		@Returns			scalar product
		@Description		calculate the scalar product of two Vec3s
		****************************************************************************/
	float dot(const KVec2Test& rhs) const
	{
		return (float)(x * rhs.x) + (float)(y * rhs.y);
	}

	/*!***************************************************************************
		@Function			ptr
		@Returns			pointer
		@Description		returns a pointer to memory containing the values of the
		Vec3
		****************************************************************************/
	float* ptr() { return (float*)this; }


	int operator == (const KVec2Test& f2) const {
		if (
			this->x == f2.x &&
			this->y == f2.y
			)
			return 1;
		else
			return 0;
	}

	int operator != (const KVec2Test& f2) const {
		if (
			this->x != f2.x ||
			this->y != f2.y
			)
			return 1;
		else
			return 0;
	}

	static KVec2Test ZeroVector;

	/**
	 * Calculates the dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The dot product.
	 */
	inline static float DotProduct(const KVec2Test& A, const KVec2Test& B) {
		return A.dot(B);
	}

	/**
	 * Squared distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two 2D points.
	 */
	inline static float DistSquared(const KVec2Test& V1, const KVec2Test& V2) {
		return (V2.x - V1.x) * (V2.x - V1.x) + (V2.y - V1.y) * (V2.y - V1.y);
	}

	/**
	 * Distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The distance between two 2D points.
	 */
	inline static float Distance(const KVec2Test& V1, const KVec2Test& V2) {
		return KMathTest::Sqrt(KVec2Test::DistSquared(V1, V2));
	}

	/**
	 * Calculate the cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The cross product.
	 */
	inline static float CrossProduct(const KVec2Test& A, const KVec2Test& B) {
		return A.cross(B);
	}

	/**
	 * Returns a vector with the maximum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The max vector.
	 */
	inline static KVec2Test Max(const KVec2Test& A, const KVec2Test& B) {
		return KVec2Test(KMathTest::Max(A.x, B.x), KMathTest::Max(A.y, B.y));
	}

	/**
	 * Returns a vector with the minimum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The min vector.
	 */
	inline static KVec2Test Min(const KVec2Test& A, const KVec2Test& B) {
		return KVec2Test(KMathTest::Min(A.x, B.x), KMathTest::Min(A.y, B.y));
	}

	/**
	 * Checks for equality with error-tolerant comparison.
	 *
	 * @param V The vector to compare.
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within specified tolerance, otherwise false.
	 */
	bool Equals(const KVec2Test& V, float Tolerance = KINDA_SMALL_NUMBER) const {
		return KMathTest::Abs(x - V.x) <= Tolerance && KMathTest::Abs(y - V.y) <= Tolerance;
	}

	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 */
	void Set(float InX, float InY) {
		x = InX;
		y = InY;
	}

	/**
	 * Get the maximum value of the vector's components.
	 *
	 * @return The maximum value of the vector's components.
	 */
	float GetMax() const {
		return KMathTest::Max(x, y);
	}

	/**
	 * Get the maximum absolute value of the vector's components.
	 *
	 * @return The maximum absolute value of the vector's components.
	 */
	float GetAbsMax() const {
		return KMathTest::Max(KMathTest::Abs(x), KMathTest::Abs(y));
	}

	/**
	 * Get the minimum value of the vector's components.
	 *
	 * @return The minimum value of the vector's components.
	 */
	float GetMin() const {
		return KMathTest::Min(x, y);
	}

	/**
	 * Get the length (magnitude) of this vector.
	 *
	 * @return The length of this vector.
	 */
	float Size() const {
		return KMathTest::Sqrt(x * x + y * y);
	}

	/**
	 * Get the squared length of this vector.
	 *
	 * @return The squared length of this vector.
	 */
	float SizeSquared() const {
		return x * x + y * y;
	}

	/**
	 * Rotates around axis (0,0,1)
	 *
	 * @param AngleDeg Angle to rotate (in degrees)
	 * @return Rotated Vector
	 */
	KVec2Test GetRotated(float AngleDeg) const {
		// Based on FVector::RotateAngleAxis with Axis(0,0,1)

		float S, C;
        KMathTest::SinCos(&S, &C, KMathTest::DegreesToRadians(AngleDeg));

		const float OMC = 1.0f - C;

		return KVec2Test(
			C * x - S * y,
			S * x + C * y);
	}

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
	 * Returns zero vector if vector length is too small to safely normalize.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return A normalized copy of the vector if safe, (0,0) otherwise.
	 */
	KVec2Test GetSafeNormal(float Tolerance = SMALL_NUMBER) const {
		const float SquareSum = x * x + y * y;
		if (SquareSum > Tolerance)
		{
			const float Scale = KMathTest::InvSqrt(SquareSum);
			return KVec2Test(x * Scale, y * Scale);
		}
		return KVec2Test::ZeroVector;
	}

	/**
	 * Normalize this vector in-place if it is large enough, set it to (0,0) otherwise.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @see GetSafeNormal()
	 */
	void Normalize(float Tolerance = SMALL_NUMBER) {
		const float SquareSum = x * x + y * y;
		if (SquareSum > Tolerance)
		{
			const float Scale = KMathTest::InvSqrt(SquareSum);
			x *= Scale;
			y *= Scale;
			return;
		}
		x = 0.0f;
		y = 0.0f;
	}

	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if vector is in tolerance to zero, otherwise false.
	 */
	bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const {
		return KMathTest::Abs(x) <= Tolerance
			&& KMathTest::Abs(y) <= Tolerance;
	}

	/**
	 * Util to convert this vector into a unit direction vector and its original length.
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	void ToDirectionAndLength(KVec2Test& OutDir, float& OutLength) const {
		OutLength = Size();
		if (OutLength > SMALL_NUMBER)
		{
			float OneOverLength = 1.0f / OutLength;
			OutDir = KVec2Test(x * OneOverLength, y * OneOverLength);
		}
		else
		{
			OutDir = KVec2Test::ZeroVector;
		}
	}

	/**
	 * Checks whether all components of the vector are exactly zero.
	 *
	 * @return true if vector is exactly zero, otherwise false.
	 */
	bool IsZero() const {
		return x == 0.f && y == 0.f;
	}
};

void UVDXtoOpengl(KVec2Test& v);
