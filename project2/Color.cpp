#include "Color.h"
Color::Color(double r, double g, double b)
{
	red = r;
	green = g;
	blue = b;
}

void Color::SetColor(double r, double g, double b)
{
	red = r;
	green = g;
	blue = b;
}

double Color::GetRed()
{
	return red;
}

double Color::GetGreen()
{
	return green;
}

double Color::GetBlue()
{
	return blue;
}

void Color::SetRed(double r)
{
	red = r;
}

void Color::SetGreen(double g)
{
	green = g;
}

void Color::SetBlue(double b)
{
	blue = b;
}
