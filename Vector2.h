#pragma once


#include <xmmintrin.h>
#include <functional>

class Vector2
{
public:
	Vector2() = default;
	Vector2(float x, float y);	
	Vector2(const __m128& data) : mData(data)
	{}

	Vector2(const Vector2&) = default;
	Vector2(Vector2&&) = default;

	Vector2& operator=(const Vector2& rhs) = default;

	Vector2(__m128&& data)
	{
		mData = std::move(data);
	}

	inline float X() const { return mData.m128_f32[0]; }
	inline float Y() const { return mData.m128_f32[1]; }	


	inline float Vector2::operator[](int i) const
	{
		return this->mData.m128_f32[i];
	}

	inline float& Vector2::operator[](int i)
	{
		return this->mData.m128_f32[i];
	}

	friend Vector2 operator+(const Vector2& a, const Vector2& b);
	friend Vector2 operator-(const Vector2& a, const Vector2& b);
	friend Vector2 operator*(float a, const Vector2& b);
	friend Vector2 operator*(const Vector2& b, float a);
	friend Vector2 operator/(const Vector2& a, float b);
	// dot product
	friend float operator*(const Vector2& a, const Vector2& b);

	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	Vector2& operator*=(const float multiplier);
	Vector2& operator/=(const float divisor);

	float Length() const;
	float SquareLength() const;

	void Normalize();
	Vector2 Normalized() const;

	static const Vector2 UnitX;
	static const Vector2 UnitY;	
	static const Vector2 Zero;
protected:
	__m128 mData;
};


