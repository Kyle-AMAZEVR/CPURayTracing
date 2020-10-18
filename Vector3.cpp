
#include "Vector3.h"
#include <cmath>

Vector3::Vector3(float x, float y, float z)
{
	mData = _mm_setr_ps(x, y, z, 0);
}

Vector3::Vector3(float value[4])
{
	mData = _mm_loadu_ps(value);	
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
	this->mData = _mm_add_ps(this->mData, rhs.mData);
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
	this->mData = _mm_sub_ps(this->mData, rhs.mData);
	return *this;
}
Vector3& Vector3::operator*=(const float multiplier)
{	
	this->mData = _mm_mul_ps(this->mData, _mm_load_ps1(&multiplier));
	return *this;
}

Vector3& Vector3::operator/=(const float divisor)
{
	if(divisor != 0)
	{
		this->mData = _mm_div_ps(this->mData, _mm_load_ps1(&divisor));
	}
	return *this;
}

float Vector3::Length() const
{
	//const int mask = 0b01110001; // 1110 0001
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->mData, this->mData, 0b01110001)));
}


float Vector3::SquareLength() const
{	
	return _mm_cvtss_f32(_mm_dp_ps(mData, mData, 0b01110111));
}


void Vector3::Normalize()
{	
	mData = _mm_mul_ps(mData, _mm_rsqrt_ps(_mm_dp_ps(mData, mData, 0b01110111)));
}

Vector3 Vector3::Normalized() const
{
	return _mm_mul_ps(mData, _mm_rsqrt_ps(_mm_dp_ps(mData, mData, 0b01110111)));
}


Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return _mm_add_ps(a.mData, b.mData);
}

Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return _mm_sub_ps(a.mData, b.mData);
}

Vector3 operator*(float a, const Vector3& b)
{
	return _mm_mul_ps(b.mData, _mm_load_ps1(&a));
}

Vector3 operator*(const Vector3& b, float a)
{
	return _mm_mul_ps(b.mData, _mm_load_ps1(&a));
}

// dot product
float operator*(const Vector3& a, const Vector3& b)
{
	return _mm_cvtss_f32(_mm_dp_ps(a.mData, b.mData, 0b01110111));
}




const Vector3 Vector3::UnitX{ 1,0,0 };
const Vector3 Vector3::UnitY{ 0,1,0 };
const Vector3 Vector3::UnitZ{ 0,0,1 };

