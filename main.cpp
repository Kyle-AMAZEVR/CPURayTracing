

#include <iostream>
#include <tbb/tbb.h>
#include <limits>
#include <pstl/algorithm>
#include <fstream>
#include <cmath>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "stb_image_write.h"
#include "HitTableList.h"
#include "Hammersley.h"
#include <chrono>

#include "Regular.h"

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

Vector3 Color(const Ray& r, const HitTable& world)
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

Vector3 Color(const Ray& r, const HitTable& world, Sampler& sampler)
{
	HitRecord rec;
	if (world.Hit(r, 0.01f, (std::numeric_limits<float>::max)(), rec))
	{
		Vector3 target = rec.P + rec.Normal + sampler.SampleSphere();
		const Ray newRay{ rec.P, target - rec.P };
		return 0.7f * Color(newRay, world, sampler);
	}
	else
	{
		Vector3 unitDirection = r.Direction().Normalized();
		float t = 0.5f * (unitDirection.Y() + 1.0f);
		return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.5f, 0.7f, 1.0f);
	}
}


const float PI = 3.141592f;

void SampleHemiSphere(const float e)
{
	int size = 100;
	for(int i = 0; i < size;i++)
	{
		//float cosPhi = std::cos(2.0f * PI * samples[i])
	}
}

void FillPixels(int beginX, int endX, int beginY, int endY, uint8_t* pixels, const HitTableList& world)
{
	Vector3 lowerLeft{ -2,-1,-1 };
	Vector3 horizontal{ 4,0,0 };
	Vector3 vertical{ 0,2,0 };
	Vector3 origin{ 0,0,0 };

	const int x = 2000;
	const int y = 1000;

	Hammersley sampler(50);
	sampler.MapSamplesToSphere();

	// start index
	int index = x * (y - endY) * 3;
	
	for (int j = endY - 1; j >= beginY; --j)
	{
		for (int i = beginX; i < endX; ++i)
		{
			float u = float(i) / float(x);
			float v = float(j) / float(y);

			Ray r(origin, lowerLeft + u * horizontal + v * vertical);

			Vector3 p = r.PointAtParameter(2.0f);

			Vector3 col = Color(r, world, sampler);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			pixels[index++] = ir;
			pixels[index++] = ig;
			pixels[index++] = ib;
		}
	}

}

int main()
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

	auto start = std::chrono::high_resolution_clock::now();

	tbb::parallel_invoke(
		[&pixels, &world]()
		{
			FillPixels(0, 2000, 0, 500, pixels, world);
		},
		[&pixels, &world]()
		{
			FillPixels(0, 2000, 500, 1000, pixels, world);
		});		

	auto end = std::chrono::high_resolution_clock::now();

	auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Took : " << miliseconds.count() <<"(m"<< std::endl;

	stbi_write_png("screenshot.png", x, y, 3, pixels, x * 3);

	
	return 0;
}