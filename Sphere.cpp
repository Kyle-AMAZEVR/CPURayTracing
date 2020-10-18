

#include "Sphere.h"

bool Sphere::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
	Vector3 oc = r.Origin() - Center;
	float a = r.Direction() * r.Direction();
	float b = oc * r.Direction();
	float c = (oc * oc) - Radius * Radius;
	float discriminant = b * b - a * c;

	if(discriminant > 0)
	{
		float temp = (-b - sqrt(b * b - a * c)) /a ;
		if(temp < tMax && temp > tMin)
		{
			rec.T = temp;
			rec.P = r.PointAtParameter(rec.T);
			rec.Normal = (rec.P - Center) / Radius;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if(temp < tMax && temp > tMin)
		{
			rec.T = temp;
			rec.P = r.PointAtParameter(rec.T);
			rec.Normal = (rec.P - Center) / Radius;
			return true;
		}
	}
	
	return false;
}
