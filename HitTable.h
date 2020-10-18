#pragma once

#include "Vector3.h"
#include "Ray.h"


struct HitRecord
{	
	Vector3 P;
	Vector3 Normal;
	float T;
};

class HitTable
{
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& result) const = 0;
};