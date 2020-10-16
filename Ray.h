#pragma once

#include "Vector3.h"

class Ray
{
public:
	Ray(const Vector3& a, const Vector3& b)
		: mOrigin(a), mDirection(b)
	{
	}
	Vector3 Origin() const { return mOrigin; }
	Vector3 Direction() const { return mDirection; }
	Vector3 PointAtParameter(const float t) const { return mOrigin + t * mDirection; }
	
protected:
	Vector3 mOrigin;
	Vector3 mDirection;
	
};