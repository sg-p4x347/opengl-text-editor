#include "pch.h"
#include "Vector2.h"

Vector2 Vector2::operator-(const Vector2& right)
{
	return Vector2(x - right.x, y - right.y);
}
