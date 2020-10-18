#pragma once


#include "HitTable.h"

class Sphere : public HitTable
{
public:
	Sphere(){}

	Sphere(const Vector3& center, float r)
		: Center(center), Radius(r)
	{}

	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& result) const override;

	Vector3 Center;
	float Radius;
};
