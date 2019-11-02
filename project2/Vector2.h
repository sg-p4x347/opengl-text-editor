#pragma once
//------------------------------------------------------
// A 2d point represented by 2 floating point components
struct Vector2
{
	Vector2() : Vector2::Vector2(0, 0) {}
	Vector2(float xIn, float yIn) : x(xIn), y(yIn) {}
	float x;
	float y;
};

