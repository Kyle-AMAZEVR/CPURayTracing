#pragma once

// no calculation supported
class Point2
{
public:
	Point2(float x, float y)
		: X(x), Y(y)
	{}
	Point2() = default;
	float X = 0;
	float Y = 0;
};