

#include <iostream>
#include <tbb/tbb.h>
#include <limits>
#include <pstl/algorithm>
#include <fstream>
#include <cstddef>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "stb_image_write.h"
#include "HitTableList.h"

bool HitSphere(const Vector3& center, float radius, const Ray& r)
{
	Vector3 oc = r.Origin() - center;
	float a = r.Direction() * r.Direction();
	float b = 2.0f * (oc * r.Direction());
	float c = (oc * oc) - radius * radius;
	float discrimnant = b * b - 4 * a * c;
	return discrimnant > 0;
}

Vector3 Color(const Ray& r)
{
	if(HitSphere(Vector3{0,0,-1}, 0.5f, r))
	{
		return Vector3::UnitX;
	}
	
	Vector3 unitDirection = r.Direction().Normalized();
	float t = 0.5f * (unitDirection.Y() + 1.0f);
	return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.5f, 0.7f, 1.0f);
}

Vector3 Color(const Ray& r, HitTable& world)
{	
	HitRecord rec;
	if(world.Hit(r, 0, (std::numeric_limits<float>::max)(), rec))
	{
		return 0.5f * Vector3(rec.Normal.X() + 1, rec.Normal.Y() + 1, rec.Normal.Z() + 1);
	}
	else
	{
		Vector3 unitDirection = r.Direction().Normalized();
		float t = 0.5f * (unitDirection.Y() + 1.0f);
		return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{	
	std::ofstream out("Test.ppm");

	if(out.good())
	{
		int x = 2000;
		int y = 1000;

		uint8_t* pixels = new uint8_t[x * y * 3];		

		Vector3 lowerLeft{ -2,-1,-1 };
		Vector3 horizontal{ 4,0,0 };
		Vector3 vertical{ 0,2,0 };
		Vector3 origin{ 0,0,0 };


		auto a = std::make_shared <Sphere>(Vector3(0, 0, -1), 0.5f);
		auto b = std::make_shared <Sphere>(Vector3(0, -100.5f, -1), 100.f);
		std::vector<std::shared_ptr<HitTable>> list;
		list.push_back(b);
		list.push_back(a);
		

		HitTableList world(list);			

		int index = 0;
		for(int j = y - 1; j>=0;--j)
		{
			for(int i = 0; i < x;++i)
			{
				float u = float(i) / float(x);
				float v = float(j) / float(y);

				Ray r(origin, lowerLeft + u * horizontal + v * vertical);

				Vector3 p = r.PointAtParameter(2.0f);

				Vector3 col = Color(r, world);

				int ir = int(255.99 * col[0]);
				int ig = int(255.99 * col[1]);
				int ib = int(255.99 * col[2]);

				pixels[index++] = ir;
				pixels[index++] = ig;
				pixels[index++] = ib;
			}
		}
		

		

		stbi_write_png("screenshot.png", x, y, 3, pixels, x * 3);
	}

	
	return 0;
}