
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