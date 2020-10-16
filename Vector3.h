#pragma once


#include <xmmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <mmintrin.h>

class Vector3
{
public:
	Vector3(float x, float y, float z);
	Vector3(float value[4]);

	inline float X() const { return mData.m128_f32[0]; }
	inline float Y() const { return mData.m128_f32[1]; }
	inline float Z() const { return mData.m128_f32[2]; }


	inline float Vector3::operator[](int i) const
	{
		return this->mData.m128_f32[i];
	}

	inline float& Vector3::operator[](int i)
	{
		return this->mData.m128_f32[i];
	}

	Vector3& operator+=(const Vector3& rhs);
	Vector3& operator-=(const Vector3& rhs);
	Vector3& operator*=(const float multiplier);

	float Length() const;
	float SquareLength() const;

	void Normalize();
	Vector3 Normalized() const;
protected:
	__m128 mData;
};