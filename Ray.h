#pragma once

#include "Vector3.h"

class Ray
{
public:
	Ray(const Vector3& a, const Vector3& b);

protected:
	Vector3 mOrigin;
	Vector3 mDirection;
	
};