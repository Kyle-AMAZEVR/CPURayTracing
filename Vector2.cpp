
#include "Vector2.h"
#include <cmath>
#include <immintrin.h>

Vector2::Vector2(float x, float y)
{
	mData = _mm_setr_ps(x, y, 0, 0);
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	this->mData = _mm_add_ps(this->mData, rhs.mData);
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs)
{
	this->mData = _mm_sub_ps(this->mData, rhs.mData);
	return *this;
}
Vector2& Vector2::operator*=(const float multiplier)
{
	this->mData = _mm_mul_ps(this->mData, _mm_load_ps1(&multiplier));
	return *this;
}

Vector2& Vector2::operator/=(const float divisor)
{
	if (divisor != 0)
	{
		this->mData = _mm_div_ps(this->mData, _mm_load_ps1(&divisor));
	}
	return *this;
}

float Vector2::Length() const
{	
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->mData, this->mData, 0b00110001)));
}


float Vector2::SquareLength() const
{
	return _mm_cvtss_f32(_mm_dp_ps(mData, mData, 0b00110011));
}


void Vector2::Normalize()
{
	mData = _mm_mul_ps(mData, _mm_rsqrt_ps(_mm_dp_ps(mData, mData, 0b00110011)));
}

Vector2 Vector2::Normalized() const
{
	return _mm_mul_ps(mData, _mm_rsqrt_ps(_mm_dp_ps(mData, mData, 0b00110011)));
}


Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return _mm_add_ps(a.mData, b.mData);
}

Vector2 operator-(const Vector2& a, const Vector2& b)
{
	return _mm_sub_ps(a.mData, b.mData);
}

Vector2 operator*(float a, const Vector2& b)
{
	return _mm_mul_ps(b.mData, _mm_load_ps1(&a));
}

Vector2 operator*(const Vector2& b, float a)
{
	return _mm_mul_ps(b.mData, _mm_load_ps1(&a));
}

// dot product
float operator*(const Vector2& a, const Vector2& b)
{
	return _mm_cvtss_f32(_mm_dp_ps(a.mData, b.mData, 0b00110011));
}

Vector2 operator/(const Vector2& a, float b)
{
	return _mm_div_ps(a.mData, _mm_load_ps1(&b));
}


const Vector2 Vector2::UnitX{ 1,0 };
const Vector2 Vector2::UnitY{ 0,1 };
const Vector2 Vector2::Zero{ 0,0 };

