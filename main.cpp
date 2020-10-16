

#include <iostream>
#include <tbb/tbb.h>
#include <pstl/algorithm>
#include <fstream>
#include "Vector3.h"
#include "Ray.h"
#include "stb_image_write.h"

Vector3 Color(const Ray& r)
{
	Vector3 unitDirection = r.Direction().Normalized();
	float t = 0.5f * (unitDirection.Y() + 1.0f);
	return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.5f, 0.7f, 1.0f);
}

int main()
{
	int x = 200;
	int y = 100;

	

	std::ofstream out("Test.ppm");

	if(out.good())
	{
		int x = 1280;
		int y = 640;		

		out << "P3\n" << x << " " << y << "\n255\n";

		Vector3 lowerLeft{ -2,-1,-1 };
		Vector3 horizontal{ 4,0,0 };
		Vector3 vertical{ 0,2,0 };
		Vector3 origin{ 0,0,0 };

		for(int j = y - 1 ; j >=0;--j)
		{
			for(int i = 0; i < x; ++i)
			{
				float u = float(i) / float(x);
				float v = float(j) / float(y);
				Ray r(origin, lowerLeft + u * horizontal + v * vertical);
				Vector3 col = Color(r);
				int ir = int(255.99 * col[0]);
				int ig = int(255.99 * col[1]);
				int ib = int(255.99 * col[2]);
				out << ir << " " << ig << " " << ib << "\n";
			}
		}
		
	}

	Vector3 a{ 1,2,3 };
	Vector3 b{ 4,5,6 };

	a += b;	

	std::cout << a.X() << std::endl;
	std::cout << a.Y() << std::endl;
	std::cout << a.Z() << std::endl;

	std::cout << "Length : " << a.Length() << std::endl;
	std::cout << "Length : " << a.SquareLength() << std::endl;

	a.Normalize();

	std::cout << a.X() << std::endl;
	std::cout << a.Y() << std::endl;
	std::cout << a.Z() << std::endl;
	
	return 0;
}