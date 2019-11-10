#include "pch.h"
#include "Color.h"

Color::Color(double r, double g, double b, double a) : R(r), G(g), B(b), A(a)
{
}

bool Color::operator==(const Color& right)
{
	return R == right.R && G == right.G && B == right.B && A == right.A;
}
